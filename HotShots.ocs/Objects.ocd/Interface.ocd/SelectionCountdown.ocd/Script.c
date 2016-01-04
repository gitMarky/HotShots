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
	ZoomIncludeAllCrew();
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
				SetPlrView(player, crew);
				TurnManager()->ShowEvent(crew);
			}
		}
	}
}

func ZoomIncludeAllCrew()
{
	var player = TurnManager()->GetActivePlayer();
	var protected_crew = Goal()->GetProtectedCrew(player);
	if (protected_crew) protected_crew->SetCrewEnabled(false);
	
	var cursor = GetCursor(player);
	
	var range_x, range_y;
	var range_min = 300;

	for (var i = 0; i < GetCrewCount(player); i++)
	{
		var crew = GetCrew(player, i);
		
		// enable non-protected crew
		if (crew == protected_crew) continue;
		
		var diff_x = Abs(cursor->GetX() - crew->GetX());
		var diff_y = Abs(cursor->GetY() - crew->GetY());

		Log("Differences: %d %d", diff_x, diff_y);

		if (diff_x > range_x) range_x = diff_x;
		if (diff_y > range_y) range_y = diff_y;
	}
	
	range_x += range_min;
	range_y += range_min;

	Log("Setting zoom to ranges: %d %d", range_x, range_y);

	SetPlayerZoomByViewRange(player, range_x, range_y, PLRZOOM_Set);
	SetPlayerZoomByViewRange(player, range_x, range_y, PLRZOOM_LimitMin);
	SetPlayerZoomByViewRange(player, range_x, range_y, PLRZOOM_LimitMax);
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