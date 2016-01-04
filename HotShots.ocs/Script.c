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
	DigFreeCaves();
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

public func DigFreeCaves()
{
	// identify caves
	
	var raster_size = 10;
	var map_pixels = [];

	for (var x = 0; x < LandscapeWidth() / raster_size; x++)
	{
		for (var y = 0; y < LandscapeHeight() / raster_size; y++)
		{
			if (!GBackSemiSolid(x * raster_size, y * raster_size))
			{
				var has_sky = GetMaterial(x * raster_size, y * raster_size) == Material("Sky");

				var pixel = {X = x, Y = y, reachable = has_sky};

				// ignore everything that is surrounded by sky
				if (has_sky
				&& !GBackSemiSolid((x - 1) * raster_size, (y - 1) * raster_size)
				&& !GBackSemiSolid((x - 1) * raster_size, (y - 1) * raster_size)
				&& !GBackSemiSolid((x - 1) * raster_size, (y - 1) * raster_size)
				&& !GBackSemiSolid((x + 0) * raster_size, (y + 0) * raster_size)
				&& !GBackSemiSolid((x + 0) * raster_size, (y + 0) * raster_size)
				&& !GBackSemiSolid((x + 0) * raster_size, (y + 0) * raster_size)
				&& !GBackSemiSolid((x + 1) * raster_size, (y + 1) * raster_size)
				&& !GBackSemiSolid((x + 1) * raster_size, (y + 1) * raster_size)
				&& !GBackSemiSolid((x + 1) * raster_size, (y + 1) * raster_size))
					continue;
					
				PushBack(map_pixels, pixel);
			}
		}
	}

	// minimize the array
	Log("Minimizing sky pixels array");
	var old_length = GetLength(map_pixels);
	var new_length = 0;
	var cave_pixels = GetPixels(map_pixels, "reachable", false);
	while (new_length != old_length)
	{
		Log("... old %d vs new %d", old_length, new_length);
		Log("    ... minimize sky pixels");
		map_pixels = MaximizeReachablePixels(map_pixels);
		Log("    ... minimize cave pixels");
		cave_pixels = GetPixels(map_pixels, "reachable", false);
		old_length = new_length;
		new_length = GetLength(map_pixels);
	}
	
	// minimize the array
	Log("Minimizing cave pixels array");
	old_length = GetLength(cave_pixels);
	new_length = 0;
	while (new_length != old_length)
	{
		Log("... old %d vs new %d", old_length, new_length);
		cave_pixels = MinimizeCavePixels(cave_pixels);
		old_length = new_length;
		new_length = GetLength(cave_pixels);
	}
	
	var sky_pixels = GetPixels(map_pixels, "reachable", true);
	
	Log("Sky pixels: %v", sky_pixels);
	Log("Cave pixels: %v", cave_pixels);
	
	for (var pixel in sky_pixels)
	{
		var dummy = CreateObject(Rock, pixel.X * raster_size, pixel.Y * raster_size);
		dummy->SetCategory(C4D_StaticBack);
	}

	for (var pixel in cave_pixels)
	{
		var dummy = CreateObject(Rock, pixel.X * raster_size, pixel.Y * raster_size);
		dummy->SetCategory(C4D_StaticBack);
		dummy->SetClrModulation(RGB(50, 50, 50));
	}


	// now only cave_pixels are left
	for (var pixel in cave_pixels)
	{
		var sky_pixel = GetNearestPixel(pixel, sky_pixels);
		var cave_pixel = GetNearestPixel(pixel, cave_pixels);
		
		var distance_cave = Distance(0, 0, cave_pixel.Y, cave_pixel.Y);
		var distance_sky = Distance(0, 0, cave_pixel.Y, cave_pixel.Y);
		
		var dig_pixel = sky_pixel;
		if (distance_cave < distance_sky && !cave_pixel.cave)
		{
			pixel.cave = true;
			dig_pixel = cave_pixel;
			
			Log("... Digging to cave pixel");
		}
		else
		{
			Log("... Digging to sky pixel");
		}
		
		DigFreeLine(pixel.X * raster_size, pixel.Y * raster_size,
		            dig_pixel.X * raster_size, dig_pixel.Y * raster_size);
	}
}

func GetPixels(array pixels, string property, value)
{
	var filtered = [];
	for (var pixel in pixels)
		if (pixel[property] == value)
			PushBack(filtered, pixel);
	return filtered;
}

func GetNeighbourPixels(proplist target, array pixels)
{
	var neighbours = [];
	for (var pixel in pixels)
	{
		var diff_x = Abs(pixel.X - target.X);
		var diff_y = Abs(pixel.Y - target.Y);
		
		if (diff_x <= 1 && diff_y <= 1 && !(diff_x == 0 && diff_y == 0))
			PushBack(neighbours, pixel);
	}

	return neighbours;
}

func MaximizeReachablePixels(array pixels)
{
	var filtered = [];
	for (var pixel in pixels)
	{
		var neighbours = GetNeighbourPixels(pixel, pixels);

		// not reachable pixels are automatically forwarded
		if (!pixel.reachable)
		{
			var reachable = false;
			// update the status for the next iteration
			for (var neighbour in neighbours)
			{
				if (neighbour.reachable)
				{
					reachable = true;
				}
			}

			PushBack(filtered, pixel); //{X = pixel.X, Y = pixel.Y, reachable = reachable});
		}
		else
		{
			PushBack(filtered, pixel);
		}
	}
	
	return filtered;
}

func MinimizeCavePixels(array pixels)
{
	var filtered = [];
	for (var pixel in pixels)
	{
		pixel.is_neighbour = 0;
	}
	for (var pixel in pixels)
	{
		var neighbours = GetNeighbourPixels(pixel, pixels);

		if (!pixel.is_neighbour) pixel.is_neighbour = 1; 

		for (var neighbour in neighbours)
		{
			if (!neighbour.is_neighbour) neighbour.is_neighbour = 2;
		}


		if (pixel.is_neighbour == 1)
		{
			PushBack(filtered, pixel);
		}
	}
	
	return filtered;
}

func GetNearestPixel(proplist pixel, array pixels)
{
	var nearest = nil;
	var min_distance = 0;
	for (var other in pixels)
	{
		var distance = Distance(pixel.X * 10, pixel.Y * 10, other.X * 10, other.Y * 10);
		if (!nearest || distance < min_distance)
		{
			nearest = other;
			min_distance = distance;
		}
	}

	return nearest;
}

func DigFreeLine(int x1, int y1, int x2, int y2)
{
	Log("    ... Digging from %d %d to %d %d", x1, y1, x2, y2);

	var distance = Distance(x1, y1, x2, y2);
	var dx = x2-x1;
	var dy = y2-y1;
	var radius = 10;
	for (var i = 0; i < distance; i++)
	{
		if (i % 8 == 0) radius = RandomX(10, 15);
	
		var dummy = CreateObject(Rock, x1 + i * dx / distance,
		        y1 + i * dy / distance);
		dummy->SetCategory(C4D_StaticBack);
		dummy->SetClrModulation(RGB(255,0,0));


		DigFree(x1 + i * dx / distance,
		        y1 + i * dy / distance,
		        radius);
	}
}
