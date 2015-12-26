/**
 Turn countdown. Counts down the time until the turn ends. 
 @author Marky
*/

#include Library_Countdown

public func DisplayMessage(int player, int counter)
{
	if (counter)
	{
		CustomMessage(Format("$Countdown$", counter), nil, player);
	}
	else
	{
		CustomMessage("", nil, player);
	}
}

public func OnCountdownStarted()
{
	TurnManager()->RegisterTurnEndBlocker(this);
	EnableCursorOnly();
}

func EnableCursorOnly()
{
	var player = TurnManager()->GetActivePlayer();
	var cursor = GetCursor(player);
	for (var i = 0; i < GetCrewCount(player); i++)
	{
		var crew = GetCrew(player, i);
		
		// disable non-cursor crew
		crew->SetCrewEnabled(crew == cursor);
	}
}

/**
 Callback that happens when the countdown ends.
 */
func OnCountdownEnded()
{
	TurnManager()->RemoveTurnEndBlocker(this);
}


/**
 Use this to access the action countdown object from other objects.
 @return object A {@link Turn_ActionCountdown#index action countdown} object,
         or {@c nil} if it does not exist.
 */
global func TurnActionCountdown()
{
	var manager = FindObject(Find_ID(Turn_ActionCountdown));
	
	return manager;
}

func DefaultCountdown(){ return 20;}