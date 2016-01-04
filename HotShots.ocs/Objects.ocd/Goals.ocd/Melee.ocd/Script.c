/** Idea:
 - each player controls multiple characters
 - once all characters are eleminated the player is eliminated

 @title Elimination Goal
 @author Marky
 */


#include Goal_Melee

static const GOAL_Elimination_CrewFactor_Default = 70000;
static const GOAL_Elimination_CrewFactor_Max = 35000;

local max_rounds = 1;
local crew_count = 1;

local player_protected_crew = [];
local player_health_max = [];
local player_health_cur = [];
local player_material = [];

local saved_starting_positions = [];

func Initialize()
{
	RoundManager()->RegisterRoundEndBlocker(this);
	TurnManager()->RegisterTurnStartBlocker(this);
	
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
	if (FrameCounter() < 1) crew_count = DetermineCrewCount(GOAL_Elimination_CrewFactor_Default);

	MakeHostileToAll(newplr, team);
	SaveFirstCrew(newplr);
	InitMaterial(newplr);
	return inherited(newplr, x, y, base, team, ...);
}


private func CheckTeamHostile(int plr1, int plr2)
{
	// eliminated players count as not hostile
	if (IsPlayerEliminated(plr1)) return false;
	if (IsPlayerEliminated(plr2)) return false;

	// check teams
	var team1 = GetPlayerTeam(plr1);
	if (team1 != GetPlayerTeam(plr2)) return true;
	if (team1) return false;
	
	// no teams? check hostility directly
	return Hostile(plr1, plr2);
}


public func IsFulfilled()
{
	// never fulfillable - we have a custom evaluation
	return false;
}


protected func AllGoalsFulfilled()
{
	// We're done. Play some sound
	Sound("Fanfare", true);

	AddEffect("IntGoalDone", nil, 1, 30, nil, Library_Goal);
}


global func Goal()
{
	return FindObject(Find_ID(Goal_Elimination));
}

func OnRoundStart(int round)
{
	ScanStartingPositions();

	CreatePlayerCrews();
	DeterminePlayerHealthMax();
	GuiPlayerHealthDisplay()->Display();
	
	TurnManager()->RemoveTurnStartBlocker(this);
}

func OnRoundEnd(int round)
{
	GuiPlayerHealthDisplay()->Hide();
	RemovePlayerCrews();
	EnableSavedCrews();
	
	if (round >= max_rounds)
	{
		RoundManager()->RegisterRoundStartBlocker(this);
		AllGoalsFulfilled();
		return;
	}	
}

func OnRoundReset(int round)
{
	player_health_max = [];
	player_health_cur = [];
	
	RoundManager()->RegisterRoundEndBlocker(this);
	
	for (var i = 0; i < GetPlayerCount(); i++)
	{
		var player = GetPlayerByIndex(i);
		
		FocusCrew(player, GetProtectedCrew(player));
	}

	_inherited(round);
}

func OnTurnEnd(int turn)
{
	// check for fulfilment
	for (var i = 0; i < GetPlayerCount(); i++)
	{
		var plr = GetPlayerByIndex(i);
		// Compare with other players.
		for (var j = i + 1; j < GetPlayerCount(); j++)
		{
			var plr2cmp = GetPlayerByIndex(j);
			// Still enemy players out there?
			if (CheckTeamHostile(plr, plr2cmp) ) return;
		}
	}

	// No enemy players left, goal fulfilled.
	RoundManager()->RemoveRoundEndBlocker(this);
	TurnManager()->RegisterTurnStartBlocker(this);
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

func EnableSavedCrews()
{
	for (var i = 0; i < GetPlayerCount(); i++)
	{
		var player = GetPlayerByIndex(i);
		var crew = GetProtectedCrew(player);
		crew->SetCrewEnabled(true);
		FocusCrew(player, crew);
	}
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
		var player = GetPlayerByIndex(i);
		CreatePlayerCrew(player);
		GetProtectedCrew(player)->SetCrewStatus(player, false);
	}
}

func CreatePlayerCrew(int player)
{
	for (var i = 0; i < crew_count; i++)
	{
		var pos = FindStartingPosition(player);

		var crew = CreateObject(Clonk, 0, 0, player);
		crew->MakeCrewMember(player);
		crew->DoEnergy(100000);
		crew->SetPosition(pos[0], pos[1]);
		crew.CorrosionResist = true; // do not lose energy in acid

		// this is the first available crew, since the other one is a saved crew
		if (i == 1) FocusCrew(player, crew);
	}
}

func RemovePlayerCrews()
{
	for (var i = 0; i < GetPlayerCount(); i++)
	{
		var player = GetPlayerByIndex(i);
		RemovePlayerCrew(player);
		GetProtectedCrew(player)->SetCrewStatus(player, true);
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
	
			if (health_phys && !IsProtectedCrew(crew))
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

func IsPlayerEliminated(int player)
{
	if (!RoundManager()->IsRoundActive()) return false;
	return GetPlayerHealth(player) <= 0;
}

func FocusCrew(int player, object crew)
{
	SetCursor(player, crew, false);
	SetPlrView(player, crew);
}

public func ReleaseCrew(object crew, bool instant)
{
	if (IsProtectedCrew(crew)) return;
	_inherited(crew, instant);
}

private func ScanStartingPositions()
{
	var desired_positions = crew_count * GetPlayerCount();
	while (GetLength(saved_starting_positions) < desired_positions)
	{
		var pos = GuessStartingPosition();
		if (pos) PushBack(saved_starting_positions, pos);
	}
}

private func GuessStartingPosition()
{
	var tx, ty; // Test position.
	for (var i = 0; i < 500; i++)
	{
		tx = Random(LandscapeWidth());
		ty = Random(LandscapeHeight());
		if (GBackSemiSolid(AbsX(tx), AbsY(ty))
		 || GBackSemiSolid(AbsX(tx+5), AbsY(ty+10))
		 || GBackSemiSolid(AbsX(tx+5), AbsY(ty-10))
		 || GBackSemiSolid(AbsX(tx-5), AbsY(ty+10))
		 || GBackSemiSolid(AbsX(tx-5), AbsY(ty-10))
		// different from other goals: must be on the ground
		 || !GBackSolid(AbsX(tx), AbsY(ty+12))
		 || IsStartingPositionTooClose(tx, ty))
			continue;

		// Success.
		return [tx, ty];
	}
	return nil;
}

private func FindStartingPosition(int plr)
{
	return PopFront(saved_starting_positions);
}

private func IsStartingPositionTooClose(int x, int y)
{
	for (var saved_pos in saved_starting_positions)
	{
		if (Distance(x, y, saved_pos[0], saved_pos[1]) < 30)
			return true;
	}

	return false;
}

func GetCrewCountCurrent()
{
	return crew_count;
}

func GetCrewCountMax()
{
	return DetermineCrewCount(GOAL_Elimination_CrewFactor_Max);
}

func SetCrewCount(int number)
{
	crew_count = BoundBy(number, 1, GetCrewCountMax());
}

func DetermineCrewCount(int factor)
{
	var area = LandscapeWidth() * LandscapeHeight();
	area /= GetPlayerCount();
	return area / factor;
}

func InitMaterial(int player)
{
	player_material[player] = CreateObject(Dummy);
	player_material[player].IsContainer = GetID().ReturnTrue;
	//player_material[player].RejectInteractionMenu = GetID().ReturnTrue;
	player_material[player]->CreateContents(Dynamite)->SetStackCount(20);
	player_material[player]->CreateContents(Firestone)->SetInfiniteStackCount();
}

func GetPlayerMaterial(int player)
{
	return player_material[player];
}

// dummy function for overriding certain stuff
func ReturnTrue(){ return true;}