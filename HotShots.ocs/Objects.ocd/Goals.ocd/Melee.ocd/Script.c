/** Idea:
 - each player controls multiple characters
 - once all characters are eleminated the player is eliminated

 @title Elimination Goal
 @author Marky
 */


#include Goal_Melee

local crew_count = 2;

local player_protected_crew = [];
local player_health_max = [];
local player_health_cur = [];

func Initialize()
{
	RoundManager()->RegisterRoundEndBlocker(this);
	
	_inherited(...);
}


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
	SaveFirstCrew(newplr);
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

global func Goal()
{
	return FindObject(Find_ID(Goal_Elimination));
}

func OnRoundStart(int round)
{
	CreatePlayerCrews();
	DeterminePlayerHealthMax();
	GuiPlayerHealthDisplay()->Display();
}

func OnRoundEnd(int round)
{
	GuiPlayerHealthDisplay()->Hide();
	RemovePlayerCrews();
}

func OnRoundReset(int round)
{
	player_health_max = [];
	player_health_cur = [];
	
	_inherited(round);
}

func NotifyHUD()
{
	for (var i = 0; i < GetPlayerCount(); i++)
	{
		DeterminePlayerHealth(GetPlayerByIndex(i));
	}

	GuiPlayerHealthDisplay()->UpdatePlayerDisplays();
	_inherited();
}

local Name = "$Name$";

func SaveFirstCrew(int player)
{
	var crew = GetCrew(player);	
	var crew_container = crew->Contained();

	if (!crew_container) crew_container = CreateObject(Dummy);

	crew_container->SetPosition(LandscapeWidth() / 2, LandscapeHeight() / 2);
	crew->Enter(crew_container);

	player_protected_crew[player] = crew;
}

func GetProtectedCrew(int player)
{
	return player_protected_crew[player];
}

func IsProtectedCrew(object crew)
{
	return IsValueInArray(player_protected_crew, crew);
}

func CreatePlayerCrews()
{
	for (var i = 0; i < GetPlayerCount(); i++)
	{
		CreatePlayerCrew(GetPlayerByIndex(i));
	}
}

func CreatePlayerCrew(int player)
{
	for (var i = 0; i < crew_count; i++)
	{
		var pos = Goal_DeathMatch->FindRelaunchPos(player);

		var crew = CreateObject(Clonk, 0, 0, player);
		crew->MakeCrewMember(player);
		crew->DoEnergy(100000);
		crew->SetPosition(pos[0], pos[1]);
		
		if (i == 0) SetCursor(player, crew);
	}
}

func RemovePlayerCrews()
{
	for (var i = 0; i < GetPlayerCount(); i++)
	{
		RemovePlayerCrew(GetPlayerByIndex(i));
	}
}

func RemovePlayerCrew(int player)
{
	for (var i = 0; i < GetCrewCount(player); i++)
	{
		var crew = GetCrew(player, i);
		
		if (!IsProtectedCrew(crew)) crew->RemoveObject();
	}
}

func DeterminePlayerHealthMax()
{
	for (var i = 0; i < GetPlayerCount(); i++)
	{
		var player = GetPlayerByIndex(i);
		var health_max = 0;
		for (var c = 0; c < GetCrewCount(player); c++)
		{
			var crew = GetCrew(player, c);
			// Health
			var health_phys = crew->~GetMaxEnergy();
	
			if (health_phys && !Goal()->IsProtectedCrew(crew))
			{
				health_max += health_phys;
			}
		}

		player_health_max[player] = health_max;
		player_health_cur[player] = health_max;
	}
}

func DeterminePlayerHealth(int player)
{
	var health_cur = 0;
	for (var c = 0; c < GetCrewCount(player); c++)
	{
		var crew = GetCrew(player, c);
		// Health

		if (!IsProtectedCrew(crew))
		{
			health_cur += crew->GetEnergy();
		}
	}

	player_health_cur[player] = health_cur;
}

func GetPlayerHealth(int player)
{
	return player_health_cur[player];
}

func GetPlayerHealthMax(int player)
{
	return player_health_max[player];
}