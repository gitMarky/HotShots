/**
 Turn countdown. Counts down the time until the turn starts. 
 @author Marky
*/

#include Library_Countdown

public func Initialize()
{
	TurnManager()->RegisterTurnStartBlocker(this);
	StartCountdown(10);
}

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

/**
 Callback that happens when the countdown ends.
 */
func OnCountdownEnded()
{
	TurnManager()->RemoveTurnStartBlocker(this);
}
