/**
 Modified shovel: useable as a weapon
 */

#appendto Shovel

public func RemoveOnActionPhaseEnd()
{
	if (Contained()) ControlUseStop(Contained());

	return _inherited();
}

