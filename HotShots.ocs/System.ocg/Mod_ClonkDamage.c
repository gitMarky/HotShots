#appendto Clonk

func Construction()
{
	var effect = AddEffect("IntDamageDisplay", this, 1, 3, this);

	effect.energy_old = GetEnergy();
	effect.energy_new = GetEnergy();
	effect.energy_dis = GetEnergy();
	_inherited(...);
}

func FxIntDamageDisplayTimer(object target, proplist effect, int timer)
{
	// update energy
	effect.energy_old = effect.energy_new;
	effect.energy_new = target->GetEnergy();
	
	// cast damage counter
	var damage = effect.energy_new - effect.energy_old;
	if (damage)
	{
		Goal()->DeterminePlayerHealth(target->GetOwner());
		FxIntDamageDisplayCastMessage(damage);
	}

	// slowly adjust displayed energy
	if (effect.energy_dis != effect.energy_new)
	{
		effect.energy_dis += BoundBy(effect.energy_new - effect.energy_dis, -1, 1);
	}
	
	// effect dummy
	if (!effect.dummy)
	{
		effect.dummy = CreateObject(Dummy);

		effect.dummy.ActMap =
		{
			Hang = {
				Prototype = Action,
				Name = "Hang",
				Procedure = DFA_ATTACH,
				Delay = 0,
				NextAction = "Hang",
			}
		};

		var wdt = GetID()->GetDefWidth() + 10;
		var hgt = GetID()->GetDefHeight();

		effect.dummy->SetShape(-wdt / 2, -hgt / 2, wdt, hgt);
		effect.dummy.Visibility = VIS_All;
	}
	else
	{
		if (effect.dummy->ActIdle()) effect.dummy->SetAction("Hang", this);
	}

	// display above clonk
	if (target->Contained())
	{
		effect.dummy->Message("");
	}
	else
	{
		effect.dummy->Message("<c %x>%d</c>|", target->GetColor(), effect.energy_dis);
	}
}

protected func FxIntDamageDisplayCastMessage(int damage)
{
	var counter = CreateObject(Dummy);
	counter->SetShape(-1, -1, 3, 3);
	counter->AddVertex(0, -4);
	counter->AddEffect("IntDamageMessage", counter, 1, 1, nil, Clonk);
	counter->FadeOut(100, true);
	counter.DamageMessage = Format("%d", damage);
	counter.Hit = Clonk.FxIntDamageMessageHit;
	counter.Visibility = VIS_All;
	counter->SetObjectLayer(counter);
	counter->SetCategory(C4D_Object);

	var speed = RandomX(15, 25);
	var angle = RandomX(20, 40) * (-1 + 2 * Random(2));
	counter->SetSpeed(Sin(angle, speed), Cos(angle, -speed));
}

protected func FxIntDamageMessageTimer(object target, proplist effect, int timer)
{
	var alpha = GetRGBaValue(target->GetClrModulation(), RGBA_ALPHA);
	target->Message("<c %x>%s</c>", RGBa(255, 255, 255, alpha), target.DamageMessage);
}

protected func FxIntDamageMessageHit(int dx, int dy)
{
	// Bounce!
	if (dy > 1) SetYDir(dy / -2, 100);
	SetXDir(dx / 2, 100);
}
