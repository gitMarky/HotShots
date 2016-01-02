/**
 Prevent weapons from being thrown.
 */

#appendto Firestone

public func ControlThrow(object clonk, int x, int y)
{
	return true;
}