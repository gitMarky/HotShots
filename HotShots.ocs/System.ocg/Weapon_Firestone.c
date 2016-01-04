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
		LoadTime       = 20,
		ShootTime      = 20,
		TurnType       = 1,
		WalkSpeed      = 84,
		WalkBack       = 56,
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

	clonk->StartLoad(this);

	ControlUseHolding(clonk, x, y);
	
	TurnManager()->ShowEvent(clonk);

	return true;
}

// Callback from the clonk when loading is finished
public func FinishedLoading(object clonk)
{
	clonk->~StartAim(this);

	// charge 40% default
	AddChargeGui();
	ChargeGuiSetMaxCharge(200);
	ChargeGuiDoCharge(80);
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
		clonk->Message("Strength %d%", ChargeGuiGetChargePercent());
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

public func RemoveOnActionPhaseEnd()
{
	if (fAiming && Contained())
	{
		Contained()->CancelAiming(this);
	}

	return _inherited();
}

/* ++++++++ Launch and damage functions ++++++++ */


public func Launch(object shooter, int angle, int str)
{
	TurnManager()->ShowEvent(this);

	var throw_speed = 650 * str / 100;
	ForceThrow(shooter, this, angle, throw_speed, animation_set.ShootTime);

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
