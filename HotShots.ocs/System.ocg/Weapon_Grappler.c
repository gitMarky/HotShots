/**
 Modified grappler: useable as a weapon
 */

#appendto GrappleBow

public func RemoveOnActionPhaseEnd()
{
	if (Contained()) ControlUseStop(Contained());

	return _inherited();
}

