/**
 Modified firestone: useable as a weapon
 */

#appendto Firestone

local fAiming;


public func GetCarryMode() { return CARRY_HandBack; }

public func GetCarrySpecial(clonk) { if(fAiming) return "pos_hand2"; }

/* +++++++++++ Controls ++++++++++++++ */

// holding callbacks are made
public func HoldingEnabled() { return true; }

local animation_set;

func Initialize()
{
	animation_set = {
		AimMode        = AIM_Position, // The aiming animation is done by adjusting the animation position to fit the angle
		AnimationAim   = "BowAimArms",
		AnimationLoad  = "BowLoadArms",
		LoadTime       = 20,
		AnimationShoot = nil,
		ShootTime      = 20,
		TurnType       = 1,
		WalkSpeed      = 84,
		WalkBack       = 56,
		AnimationReplacements = [
			["Walk", "BowWalk"],
			["Walk_Position", 20],
			["Stand", "BowStand"],
			["Jump", "BowJump"],
			["KneelDown", "BowKneel"]
		],
	};
}

public func GetAnimationSet() { return animation_set; }

public func RejectUse(object clonk)
{
	// if the clonk doesn't have an action where he can use it's hands do nothing
	return !clonk->HasHandAction();
}

public func ControlUseStart(object clonk, int x, int y)
{	
	// Start aiming
	fAiming = 1;

	//PlayAnimation("Draw", 6, Anim_Linear(0, 0, GetAnimationLength("Draw"), animation_set["LoadTime"], ANIM_Hold), Anim_Const(1000));

	clonk->StartLoad(this);

	ControlUseHolding(clonk, x, y);

	return true;
}

// Callback from the clonk when loading is finished
public func FinishedLoading(object clonk)
{
	clonk->~StartAim(this);

	// charge 20% default
	AddChargeGui();
	ChargeGuiSetMaxCharge(200);
	ChargeGuiDoCharge(40);
	return true;
}

// Update the angle on mouse movement
public func ControlUseHolding(object clonk, int x, int y)
{
	// Save new angle
	var angle = Angle(0,0,x,y);
	angle = Normalize(angle,-180);

	if(angle >  160) angle =  160;
	if(angle < -160) angle = -160;

	clonk->SetAimPosition(angle);

	// increase charge
	
	if (GetChargeGui())
	{
		ChargeGuiDoCharge(1);
		// TODO: add graphics interface
		clonk->Message("Charge %d%", ChargeGuiGetChargePercent());
	}

	return true;
}

// Stopping says the clonk to stop with aiming (he will go on untill he has finished loading and aiming at the given angle)
public func ControlUseStop(object clonk, int x, int y)
{
	clonk->StopAim();
	return true;
}

// Callback from the clonk, when he actually has stopped aiming
public func FinishedAiming(object clonk, int angle)
{
	if (!GetChargeGui()) return;

	// actions on the projectile
	Launch(clonk, angle, ChargeGuiGetChargePercent());

	// actions on the clonk
	clonk->StartShoot(this);
	Reset(clonk);
	
	// notify round management
	TurnActionCountdown()->FinishActions();
	return true;
}

public func ControlUseCancel(object clonk, int x, int y)
{
	clonk->CancelAiming(this);
	return true;
}

/* ++++++++ Launch and damage functions ++++++++ */


public func Launch(object shooter, int angle, int str)
{
	// shoot
	Exit();

	var arm_radius = 5;
	SetPosition(GetX() + Sin(angle, arm_radius), GetY() - 2 - Cos(angle, arm_radius));

	var velocity = 65 * str / 100;

	var xdir = Sin(angle, +velocity);
	var ydir = Cos(angle, -velocity);
	SetXDir(xdir);
	SetYDir(ydir);
	SetR(angle);
	SetRDir(BoundBy(xdir, -1, 1) * str / 10);

	// Shooter controls the projectile for correct kill tracing.
	SetController(shooter->GetController());

	AddEffect("HitCheck", this, 1, 1, nil, nil, shooter);
	return;
}


public func HitObject(object obj)
{
	if (WeaponCanHit(obj))
	{
		obj->~OnProjectileHit(this);
	}
		
	return Hit2();
}

public func Hit2()
{
	TurnFinishCountdown()->StartCountdown();
	_inherited(...);
}


/* ++++++++ Animation functions ++++++++ */

public func Reset(clonk)
{
	fAiming = 0;

	clonk->StopAnimation(clonk->GetRootAnimation(11));
	StopAnimation(GetRootAnimation(6));
}

/* ++++++++ Helper functions ++++++++ */

private func ClonkAimLimit(object clonk, int angle)
{
	angle = Normalize(angle,-180);
	if(Abs(angle) > 160) return false;
	if(clonk->GetDir() == 1 && angle < 0) return false;
	if(clonk->GetDir() == 0 && angle > 0) return false;
	return true;
}
