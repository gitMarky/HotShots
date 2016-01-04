/**
 Handles damage and other things once the turn has ended.
 @author Marky
*/

static const TURN_Delay_Focus = 20;
static const TURN_Delay_Handler = 72; // 2 seconds
static const TURN_Damage_InLiquid = 5;
static const TURN_Damage_OnFire = 5;

public func Start()
{
	TurnManager()->RegisterTurnEndBlocker(this);
	
	var effect = AddEffect("IntTurnEnd", this, 1, 1, this);
	effect.queue = [];
	
	for (var i = 0; i < GetPlayerCount(); i++)
	{
		var player = GetPlayerByIndex(i);
		
		for (var c = 0; c < GetCrewCount(player); c++)
		{
			var crew = GetCrew(player, c);
			
			PushBack(effect.queue, crew);
		}
	}
	
	Log("Effect queue is %v", effect.queue);
}

func FxIntTurnEndTimer(object target, proplist effect, int timer)
{
	// delayed execution?
	if (effect.delay)
	{
		effect.delay--;
		return FX_OK;
	}

	if (!effect.queue) return FX_OK;
	
	// done?
	if (GetLength(effect.queue) == 0)
	{
		TurnManager()->RemoveTurnEndBlocker(this);
		return FX_Execute_Kill;
	}
	
	// do the actual stuff
	var crew = PopFront(effect.queue);
	
	HandleCrewInLiquid(crew, effect);
	HandleCrewOnFire(crew, effect);
}

func HandleCrewInLiquid(object crew, proplist effect)
{
	if (crew->GBackLiquid())
	{
		DamageCrew(crew, effect, TURN_Damage_InLiquid);
	}
}

func HandleCrewOnFire(object crew, proplist effect)
{
	if (crew->OnFire())
	{
		crew.TurnEnd_OnFire++;
		DamageCrew(crew, effect, TURN_Damage_OnFire);
		
		// crew burns for 5 turns total
		if (crew.TurnEnd_OnFire >= 5)
		{
			crew.TurnEnd_OnFire = 0;
			crew->Extinguish();
		}
	}
}

func DamageCrew(object crew, proplist effect, int amount)
{
	TurnManager()->ShowEvent(crew);
	ScheduleCall(crew, crew.DoEnergy, TURN_Delay_Focus, 0, -amount);
	DelayExecution(effect, TURN_Delay_Handler + TURN_Delay_Focus);
}

func DelayExecution(proplist effect, int amount)
{
	effect.delay = Max(effect.delay, amount);
}

/**
 Use this to access the handler object from other objects.
 @return object A {@link Turn_EndHandler#index selection countdown} object,
         or {@c nil} if it does not exist.
 */
global func TurnEndHandler()
{
	var manager = FindObject(Find_ID(Turn_EndHandler));
	
	return manager;
}
