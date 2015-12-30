/**
 Handles the way the turn manager behaves
 */

#appendto Environment_TurnManager


local active_player_index; // this player is currently playing


/**
 Tells which player number is currently active.
 @return the active player number.
 */
public func GetActivePlayer()
{
	if (active_player_index >= GetPlayerCount())
	{
		FatalError(Format("Called when active player is %d, but only %d is allowed.", active_player_index, GetPlayerCount() - 1));
	}

	return GetPlayerByIndex(active_player_index);
}


/**
 Tells which crew member is currently active.
 @return the crew member object.
 */
public func GetActiveCrew()
{
	return GetCursor(GetActivePlayer());
}


/**
 Callback from this object, when the turn is reset and before a new turn starts.
 */
func OnTurnReset(int turn_number)
{
	_inherited(turn_number);

	// cycle the active player when the round is reset
	active_player_index++;
	
	if (active_player_index >= GetPlayerCount())
	{
		active_player_index = 0;
	}
	
	Log("Turn was reset: %d - Active player will be number %d: %s", turn_number, active_player_index, GetPlayerName(GetPlayerByIndex(active_player_index)));
}

func OnTurnStart(int turn_number)
{
	_inherited(turn_number);

	Log("Turn Started: %d", turn_number);
	TurnSelectionCountdown()->StartCountdown();
}

func OnTurnEnd(int turn_number)
{
	_inherited(turn_number);
	Log("Turn Ended: %d", turn_number);
}

func OnRoundReset(int round_number)
{
	Log("Round Reset: %d", round_number);
	RegisterTurnStartBlocker(RoundManager());
}


/**
 Callback from the round object, when the round starts.
 */
func OnRoundStart(int round_number)
{
	Log("Round Start: %d", round_number);
	RemoveTurnStartBlocker(RoundManager());
}
 