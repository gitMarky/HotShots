#appendto Clonk

func Death()
{
	// last clonk? Reactivate the saved crew
	if (GetCrewCount(GetOwner()) <= 1)
	{
		Goal()->GetProtectedCrew(GetOwner())->SetCrewStatus(GetOwner(), true);
	}

	Log("Clonk died: active crew is %v, clonk is %v", TurnManager()->GetActiveCrew(), this);

	// cancel the turn
	if (TurnManager()->GetActiveCrew() == this)
	{
		TurnActionCountdown()->FinishActions();
	}

	_inherited(...);
}