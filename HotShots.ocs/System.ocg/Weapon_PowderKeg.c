/**
 Modified poweder keg: useable as a weapon
 */

#appendto PowderKeg

//public func RemoveOnActionPhaseEnd()
//{
//	if (Contained()) ControlUseStop(Contained());
//
//	return _inherited();
//}

public func Incineration()
{
	// notify round management
	TurnActionCountdown()->FinishActions();
	TurnManager()->ShowEvent(this);
	_inherited();
}