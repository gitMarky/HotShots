#appendto Clonk

func Death()
{
	Log("Clonk died: active crew is %v, clonk is %v", TurnManager()->GetActiveCrew(), this);

	// cancel the turn
	if (TurnManager()->GetActiveCrew() == this)
	{
		TurnActionCountdown()->FinishActions();
	}

	_inherited(...);
}