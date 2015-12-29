/**
 Invisible countdown. Delays the turn end until some action (projectile hit, objects flying)
 has ended. 
 @author Marky
*/

#include Library_Countdown

func DisplayMessage(int player, int counter)
{
	// override: do nothing
	Log("Finish countdown: %d", counter);
}

public func BlockTurnEnd()
{
	TurnManager()->RegisterTurnEndBlocker(this);
}

public func OnCountdownStarted()
{
}

/**
 Callback that happens when the countdown ends.
 */
func OnCountdownEnded()
{
	TurnManager()->RemoveTurnEndBlocker(this);
}


/**
 Use this to access the selection countdown object from other objects.
 @return object A {@link Turn_SelectionCountdown#index selection countdown} object,
         or {@c nil} if it does not exist.
 */
global func TurnFinishCountdown()
{
	var manager = FindObject(Find_ID(Turn_FinishCountdown));
	
	return manager;
}

func DefaultCountdown(){ return 3;}

func ProlongCountdown()
{
	Log("Prolonged countdown");
	if (IsActive()) StartCountdown();
}