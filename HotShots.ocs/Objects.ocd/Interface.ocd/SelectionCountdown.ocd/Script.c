/**
 Turn countdown. Counts down the time until the turn starts. 
 @author Marky
*/

#include Library_Countdown


public func DisplayMessage(int player, int counter)
{
	if (counter)
	{
		var active_player = TurnManager()->GetActivePlayer();
		CustomMessage(Format("$Countdown$", GetPlayerName(active_player), counter), nil, player);
	}
	else
	{
		CustomMessage("", nil, player);
	}
}


public func OnCountdownStarted()
{
	TurnManager()->RegisterTurnEndBlocker(this);
	EnableCrew();
}


func EnableCrew()
{	
	var player = TurnManager()->GetActivePlayer();
	var protected_crew = Goal()->GetProtectedCrew(player);
	if (protected_crew) protected_crew->SetCrewEnabled(false);
	
	var cursor = false;

	for (var i = 0; i < GetCrewCount(player); i++)
	{
		var crew = GetCrew(player, i);
		
		// enable non-protected crew
		if (crew != protected_crew)
		{
			crew->SetCrewEnabled(true);
			
			if (!cursor)
			{
				cursor = true;
				SetCursor(player, crew);
			}
		}
	}
}


/**
 Callback that happens when the countdown ends.
 */
func OnCountdownEnded()
{
	TurnActionCountdown()->StartCountdown();
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


func DefaultCountdown(){ return 10;}