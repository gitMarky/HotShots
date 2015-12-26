/**
 Turn countdown. Counts down the time until the turn starts. 
 @author Marky
*/

#include Library_Countdown

public func Initialize()
{
	TurnManager()->RegisterTurnEndBlocker(this);
	StartCountdown(10);
}

public func DisplayMessage(int player, int counter)
{
	if (counter)
	{
		CustomMessage(Format("$Countdown$", GetPlayerName(player), counter), nil, player);
	}
	else
	{
		CustomMessage("", nil, player);
	}
}

/**
 Callback that happens when the countdown ends.
 */
func OnCountdownEnded()
{
	CreateObject(Turn_ActionCountdown);
	TurnManager()->RemoveTurnEndBlocker(this);
}



/**
 Use this to access the selection countdown object from other objects.
 @return object A {@link Turn_SelectionCountdown#index selection countdown} object,
         or {@c nil} if it does not exist.
 */
global func TurnSelectionCountdown()
{
	var manager = FindObject(Find_ID(Turn_SelectionCountdown));
	
	return manager;
}