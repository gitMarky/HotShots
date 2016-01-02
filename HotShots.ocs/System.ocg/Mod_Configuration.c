#appendto Environment_Configuration

public func ReleaseCrew(object crew, bool instant)
{
	if (Goal()->IsProtectedCrew(crew)) return;
	
	_inherited(crew, instant, ...);
}

// remove item configuration
public func CanConfigureSpawnPoints()
{
	return false;
}
