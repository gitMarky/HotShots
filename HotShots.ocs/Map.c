/**
	Dark Mine
	A single large cave surrounded by lots of small caves, connected through
	narrow mine shafts.
	
	@authors Maikel
*/

#include Library_Map


// List for storing the different large caves.
static cave_list;

// Called be the engine: draw the complete map here.
protected func InitializeMap(proplist map)
{
	// Map size: all other settings depend on this value.
	// The map size depends on the number of players.
	var players = GetStartupPlayerCount();
	var map_width = 60 + players * 20;
	var map_height = BoundBy(3 * map_width / 4, 50, 100);

	map->Resize(map_width, map_height);
	
	// Draw the background materials for the whole map.
	DrawBackground();

	// Return true to tell the engine a map has been successfully created.
	return true;
}


/*-- Map Drawing --*/

public func DrawBackground()
{
//	Draw("Rock");
//	DrawVariations("Rock", 50, 5, 15);
//	DrawVariations("Ore", 10, 8, 8);
//	DrawVariations("Firestone", 8, 12, 3);
//	DrawVariations("Earth", 3, 8, 3);
//	DrawVariations("Earth-earth", 3, 8, 3);
//	DrawVariations("Earth-earth", 3, 8, 3);
//	DrawVariations("Earth-earth_root", 3, 8, 3);
//	DrawVariations("Earth-earth_spongy", 3, 8, 3);
//	DrawVariations("Firestone", 6, 12, 3);
//	DrawVariations("Coal", 8, 8, 3);
//	DrawVariations("Gold", 5, 4, 4);
//	DrawVariations("Granite", 14, 15, 5);
//	DrawVariations("Granite", 14, 5, 15);
	return true;
}


/*-- Helper Functions --*/

public func DrawVariations(string mat, int ratio, int sx, int sy)
{
	var rand_algo = {Algo=MAPALGO_RndChecker, Ratio = ratio, Wdt = sx, Hgt = sy};
	var turb_algo = {Algo=MAPALGO_Turbulence, Amplitude = 12, Scale = 8, Op = rand_algo};
	return Draw(mat, turb_algo);
}
