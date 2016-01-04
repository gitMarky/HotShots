/**
 Modified dynamite: useable as a weapon
 */

#appendto Dynamite

public func Fuse()
{
	// notify round management
	TurnFinishCountdown()->BlockTurnEnd();
	_inherited();
}

public func Fusing()
{
	TurnManager()->ShowEvent(this);
	_inherited();
}	
	
public func DoExplode()
{
	// notify round management
	TurnActionCountdown()->FinishActions();
	TurnFinishCountdown()->StartCountdown();

	_inherited();
}

public func RemoveOnActionPhaseEnd()
{
	// do not remove fusing objects
	if (Contained() && GetAction() == "Fuse")
	{
		return true;
	}

	return _inherited();
}

