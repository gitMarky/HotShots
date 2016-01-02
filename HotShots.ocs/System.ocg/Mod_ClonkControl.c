/**
 Custom clonk control
 */
 
#appendto Library_ClonkControl


public func ObjectControl(int plr, int ctrl, int x, int y, int strength, bool repeat, bool release)
{
	// no controls while crew selection is active
	if (TurnSelectionCountdown()->IsActive())
	{
		if(ctrl == CON_Interact && release)
		{
			TurnSelectionCountdown()->Abort();
			return true;
		}

		return false;
	}
	
	return _inherited(plr, ctrl, x, y, strength, repeat, release, ...);
}

public func ObjectCommand(string command, object target, int tx, int ty, object target2)
{
	if (command == "Drop") // prevent dropping items altogether
	{
		return true;
	}

	return _inherited(command, target, tx, ty, target2);
}