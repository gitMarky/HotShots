/** 
	Hot Shots

	@author Marky
*/



protected func Initialize()
{
	// configuration
	CreateObject(Environment_RoundManager);
	CreateObject(Environment_Configuration);
	CreateObject(Turn_SelectionCountdown);
	CreateObject(Turn_ActionCountdown);
	CreateObject(Turn_FinishCountdown);
	CreateObject(Turn_EndHandler);
	CreateObject(Environment_TurnManager);
	CreateObject(GUI_PlayerHealthDisplay);

	// goal
	CreateObject(Goal_Elimination);
	CreateObject(Rule_KillLogs);
	CreateObject(Rule_Gravestones);
	
	// environment
	SetSkyParallax(1, 20, 20, 0, 0, nil, nil);
	//SetSkyAdjust(RGB(255, 128, 0));
	//SetMatAdjust(RGB(255, 150, 128));
	
	PlaceGrass();
	PlaceMushrooms();
	return;
}

private func PlaceGrass()
{
	for (var x = 0; x < LandscapeWidth(); x += 10)
	{
		var y = 0;
		while(GetMaterial(x, y) != Material("Tunnel") && y < LandscapeHeight()) y += 3;

		var grass=CreateObjectAbove(Grass, x, y, NO_OWNER);
		grass->SetCategory(C4D_StaticBack);
		//grass->SetR(r[i]); 
		grass->Initialize();
		//grass->SetClrModulation(RGB(225+Random(30), Random(30), Random(30)));		
	}
	return;
}

private func PlaceMushrooms()
{
	for (var x = 0; x < LandscapeWidth(); x += RandomX(40, 70))
	{
		var y = LandscapeHeight();
		var suitable = false;
		while(!suitable && y > 0)
		{
			y -= 3;
			suitable = GBackSolid(x, y) && GetMaterial(x, y - 5) == Material("Tunnel");
		}

		if (suitable) CreateObject(Mushroom, x, y, NO_OWNER);
	}
}

protected func OnPlayerRelaunch(int plr)
{
	var clonk = GetCrew(plr);
	clonk->DoEnergy(100000);
	var x = RandomX(75,500);
	var y=100;
	while(!GBackSolid(x,y)) y+=1;
	y-=30;
	var relaunch = CreateObjectAbove(RelaunchContainer, x, y, clonk->GetOwner());
	relaunch->StartRelaunch(clonk);
	relaunch->SetRelaunchTime(3);
	clonk->CreateContents(TeleGlove);
	return;
}

public func OnClonkLeftRelaunch(object clonk)
{
	clonk->CreateParticle("Fire", 0, 0, PV_Random(-20, 20), PV_Random(-40, 5), PV_Random(20, 90), Particles_Glimmer(), 30);
	clonk->SetYDir(-5);
	return;
}

// Remove contents of clonks on their death
public func OnClonkDeath(object clonk)
{
	while (clonk->Contents())
		clonk->Contents()->RemoveObject();
	return;
}

// Settings for DM
public func WinKillCount() { return 5; }
