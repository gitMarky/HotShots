/**
 Modified boom pack: useable as a weapon
 */

#appendto Boompack

//public func RemoveOnActionPhaseEnd()
//{
//	if (Contained()) ControlUseStop(Contained());
//
//	return _inherited();
//}

func Launch(int angle, object clonk)
{
	TurnManager()->ShowEvent(this);
	return _inherited(angle, clonk);
}

func JumpOff(object clonk, int speed)
{
	TurnActionCountdown()->FinishActions();
	TurnManager()->ShowEvent(this);
	return _inherited(clonk, speed);
}
