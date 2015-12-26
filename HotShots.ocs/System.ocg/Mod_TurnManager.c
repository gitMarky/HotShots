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
		FatalError("Called when active player is %d, but only %d is allowed.", active_player_index, GetPlayerCount() - 1);
	}

	return GetPlayerByIndex(active_player_index);
}

/**
 Callback from this object, when the turn is reset and before a new turn starts.
 */
func OnTurnReset(int turn_number)
{
	_inherited(turn_number);

	Log("TurnManager: OnTurnReset");

	// cycle the active player when the round is reset
	active_player_index++;
	
	if (active_player_index >= GetPlayerCount())
	{
		active_player_index = 0;
	}
}

func OnTurnStart(int turn_number)
{
	_inherited(turn_number);

	Log("TurnManager: OnTurnStart");

	// create selection countdown
	CreateObject(Turn_SelectionCountdown);
}


func OnRoundReset(int round_number)
{
	Log("TurnManager: OnRoundReset");

	RegisterTurnStartBlocker(RoundManager());
}

/**
 Callback from the round object, when the round starts.
 */
func OnRoundStart(int round_number)
{
	Log("TurnManager: OnRoundStart");

	RemoveTurnStartBlocker(RoundManager());
}
 