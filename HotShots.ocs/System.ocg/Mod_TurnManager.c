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
func OnTurnReset()
{
	// cycle the active player when the round is reset
	active_player_index++;
	
	if (active_player_index >= GetPlayerCount())
	{
		active_player_index = 0;
	}
}