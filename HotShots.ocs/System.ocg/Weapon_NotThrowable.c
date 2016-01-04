/**
 Prevent weapons from being thrown.
 */

#appendto Firestone
#appendto Dynamite

public func ControlThrow(object clonk, int x, int y)
{
	return true;
}