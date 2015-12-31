#include Library_Map


// Called be the engine: draw the complete map here.
protected func InitializeMap(proplist map)
{
	// Map size: all other settings depend on this value.
	// The map size depends on the number of players.
	var players = GetStartupPlayerCount();
	var map_width = 60 + players * 20;
	var map_height = BoundBy(3 * map_width / 4, 75, 150);

	map->Resize(map_width, map_height);

	// Draw water at the bottom 20% of the map
	var water = {Algo = MAPALGO_Rect, X = 0, Y = map_height * 8 / 10, Wdt = map_width, Hgt = map_height * 2 / 10};
	Draw("Water", water);

	// Draw a random surface
	var surface = {Algo = MAPALGO_Rect, X = 0, Y = map.Hgt * 7 / 10, Wdt = map.Wdt, Hgt = (map.Hgt + 1) * 3 / 10};
	surface = {Algo = MAPALGO_Turbulence, Seed = Random(65536), Op = surface, Amplitude = [25, 15], Scale = 10, Iterations = 3};
	Draw("Earth", surface);	

	// Return true to tell the engine a map has been successfully created.
	return true;
}

