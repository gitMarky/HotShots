#appendto Clonk


func Damage(int change, int cause, int by_player)
{
	TurnFinishCountdown()->ProlongCountdown();
	GuiPlayerHealthDisplay()->UpdatePlayerDisplays();
	_inherited(change, cause, by_player);
}

func Construction()
{
	var effect = AddEffect("IntDamageDisplay", this, 1, 7, this);

	effect.energy_old = GetEnergy();
	effect.energy_new = GetEnergy();
	_inherited(...);
}

func FxIntDamageDisplayTimer(object target, proplist effect, int timer)
{
	// update energy
	effect.energy_new = target->GetEnergy();

	// slowly adjust energy
	if (effect.energy_old >= effect.energy_new)
	{
		effect.energy_old += BoundBy(effect.energy_new - effect.energy_old, -1, 1);
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
		effect.dummy->Message("<c %x>%d</c>|", target->GetColor(), effect.energy_old);
	}
}