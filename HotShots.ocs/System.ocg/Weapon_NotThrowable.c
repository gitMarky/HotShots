/**
 Prevent weapons from being thrown.
 */

#appendto Firestone
#appendto Dynamite
#appendto Shovel
#appendto GrappleBow

public func ControlThrow(object clonk, int x, int y)
{
	return true;
}