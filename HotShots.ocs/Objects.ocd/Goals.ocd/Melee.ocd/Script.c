/** Idea:
 - each player controls multiple characters
 - once all characters are eleminated the player is eliminated

 @title Elimination Goal
 @author Marky
 */

#include Goal_Melee

func MakeHostileToAll(int newplr, int team)
{
	// If the player is in a team, don't change hostility.
	if (team) return;

	// Otherwise, make all other players enemies.
	for (var i = 0; i < GetPlayerCount(); i++)
	{
		var plr = GetPlayerByIndex(i);
		if (plr == newplr) continue;
		
		SetHostility(newplr, plr, true, true);
		SetHostility(plr, newplr, true, true);
	}
}

protected func InitializePlayer(int newplr, int x, int y, object base, int team)
{
	MakeHostileToAll(newplr, team);
	return inherited(newplr, x, y, base, team, ...);
}

private func CheckTeamHostile(int plr1, int plr2)
{
	var team1 = GetPlayerTeam(plr1);
	if (team1 != GetPlayerTeam(plr2))
		return true;
	if (team1)
		return false;
	return Hostile(plr1, plr2);
}

public func IsFulfilled()
{
	// If Teams.txt-Teams still need to be chosen, the goal cannot be fulfilled.
	if (GetPlayerTeam(GetPlayerByIndex()) == -1) return;

	for (var i = 0; i < GetPlayerCount(); i++)
	{
		var plr = GetPlayerByIndex(i);
		// Compare with other players.
		for (var j = i + 1; j < GetPlayerCount(); j++)
		{
			var plr2cmp = GetPlayerByIndex(j);
			// Still enemy players out there?
			if (CheckTeamHostile(plr, plr2cmp) ) return false;
		}
	}
	
	// No enemy players, goal fulfilled.
	return false; // was: true;
}

local Name = "$Name$";
