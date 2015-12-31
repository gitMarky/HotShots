#include Library_Map


// Called be the engine: draw the complete map here.
protected func InitializeMap(proplist map)
{
	// Map size: all other settings depend on this value.
	// The map size depends on the number of players.
	var players = GetStartupPlayerCount();
	var map_width = 100 + players * 40;
	var map_height = BoundBy(map_width / 3, 50, 150);

	map->Resize(map_width, map_height);

	// Draw water at the bottom 20% of the map
	var water = {Algo = MAPALGO_Rect, X = 0, Y = map_height * 8 / 10, Wdt = map_width, Hgt = map_height * 2 / 10};
	Draw("Water", water);

	// Draw the main surface: an island with the polygon algorithm.
	var x_points = [              0, 15 * map_width / 100, 48 * map_width / 100, 52 * map_width / 100, 85 * map_width / 100, map_width];
	var y_points = [map_height + 10, 70 * map_height /100, 60 * map_height /100, 60 * map_height /100, 70 * map_height /100, map_height + 10];
	var island = {Algo = MAPALGO_Polygon, X = x_points, Y = y_points};
	island = {Algo = MAPALGO_Turbulence, Seed = Random(65536), Op = island, Amplitude = [25, 15], Scale = 10, Iterations = 1};	
	Draw("Earth", island);	


	// Return true to tell the engine a map has been successfully created.
	return true;
}

