/**
 Modified dynamite: useable as a weapon
 */

#appendto Sword

public func ControlUse(object clonk, int x, int y)
{
	var result = _inherited(clonk, x, y);	

	TurnActionCountdown()->FinishActions();
	TurnFinishCountdown()->StartCountdown();

	return result;
}


func SwordDamage(int shield) // does slightly more damage than usual
{
	return ((100-shield)*15*1000 / 100);
}

public func RemoveOnActionPhaseEnd()
{
	// do not remove immediately
	if (Contained() && GetEffect("IntWeaponCharge", Contained()))
	{
		ScheduleCall(this, this.RemoveObject, 36);
		return true;
	}

	return _inherited();
}
