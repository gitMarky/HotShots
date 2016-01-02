#appendto Clonk

func Death()
{
	// last clonk? Reactivate the saved crew
	if (GetCrewCount(GetOwner()) <= 1)
	{
		Goal()->GetProtectedCrew(GetOwner())->SetCrewStatus(GetOwner(), true);
	}

	// cancel the turn
	if (TurnManager()->GetActiveCrew() == this)
	{
		TurnActionCountdown()->FinishActions();
	}

	_inherited(...);
}