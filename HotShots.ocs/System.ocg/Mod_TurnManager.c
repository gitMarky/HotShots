/**
 Handles the way the turn manager behaves
 */

#appendto Environment_TurnManager

local last_player_index = -1;
local active_player_index; // this player is currently playing
local active_crew; // this crew is currently being controlled


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
	return active_crew;
}


public func SetActiveCrew(object crew)
{
	active_crew = crew;
}


/**
 Callback from this object, when the turn is reset and before a new turn starts.
 */
func OnTurnReset(int turn_number)
{
	_inherited(turn_number);
	
	Log("Turn Reset");
	
	// forget the active crew
	SetActiveCrew(nil);

	// cycle the active player when the round is reset
	active_player_index++;
	
	if (active_player_index >= GetPlayerCount())
	{
		active_player_index = 0;
		Log("Exceeded player count, active player will be 0");
	}
	
	// do not use the same player twice in a row if one player was eliminated
	if (active_player_index == last_player_index && GetPlayerCount() > 1)
	{
		active_player_index++;
		Log("Active player was already active last turn - next player is %d", active_player_index);
	}
	
	// skip eliminated players
	while(Goal()->IsPlayerEliminated(GetPlayerByIndex(active_player_index)) && active_player_index < GetPlayerCount())
	{
		Log("Player %d is eliminated, skipping", active_player_index);
		active_player_index++;
	}
	
	Log("Turn was reset: %d - Active player will be number %d: %s", turn_number, active_player_index, GetPlayerName(GetPlayerByIndex(active_player_index)));
}

func OnTurnStart(int turn_number)
{
	_inherited(turn_number);

	Log("Turn Started: %d", turn_number);
	last_player_index = active_player_index;
	TurnSelectionCountdown()->StartCountdown();
}

func OnTurnEnd(int turn_number)
{
	_inherited(turn_number);
	Log("Turn Ended: %d", turn_number);
}

func OnRoundReset(int round_number)
{
	active_player_index = 0;
	last_player_index = 0;

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
 