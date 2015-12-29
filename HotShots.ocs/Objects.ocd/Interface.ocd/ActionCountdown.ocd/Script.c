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
	FinishActions();
}

func FinishActions()
{
	TurnFinishCountdown()->BlockTurnEnd();
	TurnManager()->RemoveTurnEndBlocker(this);
	DisableInventory();
	DisableCrew();
	Abort();
}	

func DisableInventory()
{
	// remove all inventory items, so that nothing is left in the next turn
	var crew = TurnManager()->GetActiveCrew();
	if (crew)
	{
		for (var item in FindObjects(Find_Container(crew)))
		{
			// TODO: possibly add the item to team resources again
			if (item) item->RemoveObject();
		}
	}
}

func DisableCrew()
{
	var crew = TurnManager()->GetActiveCrew();
	if (crew) crew->SetCrewEnabled(false);
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