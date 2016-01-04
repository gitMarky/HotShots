/**
 Modified dynamite: useable as a weapon
 */

#appendto Dynamite

public func Fuse()
{
	// notify round management
	//TurnActionCountdown()->FinishActions();

	_inherited();
}

public func OnFuseFinished()
{
	// notify round management
	TurnActionCountdown()->FinishActions();
	ScheduleCall(TurnFinishCountdown(), TurnFinishCountdown().StartCountdown, 1);

	_inherited();
}

public func RemoveOnActionPhaseEnd()
{
	// do not remove fusing objects
	if (Contained() && GetAction() == "Fuse")
	{
		return true;
	}
	
	return false;
}

