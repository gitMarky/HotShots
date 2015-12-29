
/**
 Starts a countdown with this many seconds.
 @par seconds the number of seconds.
 */
func StartCountdown(int seconds)
{
	if (!seconds) seconds = DefaultCountdown();

	var effect = AddEffect("IntCountdown", this, 1, 36, this);
	SetCountdown(effect, seconds);
	OnCountdownStarted();
}

/**
 Sets the countdown for this object.
 @par effect the countdown effect
 @par value the countdown counter, in seconds
 */
func SetCountdown(proplist effect, int value)
{
	effect.counter = value;
	effect.counter_set = true;
}

/**
 Override this. By default this displays a red
 message with the countdown number for a player.
 @par player the player index
 @par counter the current counter value.
 */
func DisplayMessage(int player, int counter)
{
	if (counter)
	{
		CustomMessage(Format("@%d...", counter), nil, player, 0, 0, RGB(200, 0, 0));
	}
	else
	{
		CustomMessage("", nil, player);
	}
}

/**
 Callback that happens when the countdown starts.
 */
func OnCountdownStarted()
{
}

/**
 Callback that happens when the countdown ends.
 */
func OnCountdownEnded()
{
}

public func FxIntCountdownTimer(object target, proplist effect, int time)
{
	if (!effect.counter_set) return FX_OK;

	for (var i = 0;  i < GetPlayerCount(C4PT_User); ++i)
	{
		var player = GetPlayerByIndex(i, C4PT_User);

		DisplayMessage(player, effect.counter);
	}
	
	// remove?
	if (effect.counter <= 0)
	{
		return FX_Execute_Kill;
	}

	--effect.counter;
	return FX_OK;
}

private func FxIntCountdownStop(object target, proplist effect, int reason, bool temporary)
{
	if (temporary) return;

	OnCountdownEnded();
}

func DefaultCountdown(){ return 0;}

func IsActive()
{
	return GetEffect("IntCountdown", this);
}


/**
 Aborts the countdown. This causes the same callbacks
 as if the countdown had ended normally.
 */
func Abort()
{
	var effect = IsActive();
	if (effect)
	{
		SetCountdown(effect, 0);
	}
}