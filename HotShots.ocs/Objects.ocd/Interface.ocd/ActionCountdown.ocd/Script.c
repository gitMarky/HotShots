/**
 Turn countdown. Counts down the time until the turn ends. 
 @author Marky
*/

#include Library_Countdown

local actions_finished = false;

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
	actions_finished = false;
	TurnManager()->RegisterTurnEndBlocker(this);
	EnableCursorOnly();
	EnableInventory();
}


func EnableCursorOnly()
{
	var player = TurnManager()->GetActivePlayer();
	var cursor = GetCursor(player);
	SetPlrView(player, cursor);
	TurnManager()->SetActiveCrew(cursor);
	TurnManager()->ShowEvent(cursor);
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
	if (!TurnFinishCountdown()->IsActive()) TurnFinishCountdown()->StartCountdown();
}


func FinishActions()
{
	if (!actions_finished)
	{
		actions_finished = true;

		TurnFinishCountdown()->BlockTurnEnd();
		TurnManager()->RemoveTurnEndBlocker(this);
		DisableInventory();
		DisableCrew();
		Abort();
	}
}


func EnableInventory()
{
	var crew = TurnManager()->GetActiveCrew();
	//if (crew) crew->CreateContents(Firestone);
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
			var should_remove = !item->~RemoveOnActionPhaseEnd();
			
			if (should_remove && item)
			{
				item->RemoveObject();
			}
		}
	}
}


func DisableCrew()
{
	var crew = TurnManager()->GetActiveCrew();
	if (crew)
	{
		crew->SetCrewEnabled(false);
		crew->TryCancelMenu();
	}
}


public func FxIntCountdownTimer(object target, proplist effect, int time)
{
	var crew = TurnManager()->GetActiveCrew();

	if (crew && crew->~GetMenu())
	{
		if (crew->GetMenu()->~Unclosable() 	// pause countdown if an uncloseable menu is opened
		 || crew->GetMenu()->~IsContentMenu()) // pause countdown if the conent menu is opened
		 	return FX_OK;
	}

	return _inherited(target, effect, time);
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