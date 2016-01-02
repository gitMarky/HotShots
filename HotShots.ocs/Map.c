#include Library_Map


// Called be the engine: draw the complete map here.
protected func InitializeMap(proplist map)
{
	// Map size: all other settings depend on this value.
	// The map size depends on the number of players.
	var players = GetStartupPlayerCount();
	var map_width = 100 + players * 50;
	var map_height = BoundBy(map_width / 3, 50, 150);

	var water_level = 7 * map_height / 10;

	map->Resize(map_width, map_height);

	// Draw water at the bottom 20% of the map
	var water = {Algo = MAPALGO_Rect, X = 0, Y = water_level, Wdt = map_width, Hgt = map_height - water_level};
	Draw("Water", water);

	DrawIsland(map_width, map_height, water_level);
	DrawRuins(map_width, map_height, water_level);

	// Return true to tell the engine a map has been successfully created.
	return true;
}

func DrawIsland(int map_width, int map_height, int water_level)
{
	// Draw the main surface: an island with the polygon algorithm.
	var x_points = [              0, 15 * map_width / 100, 50 * map_width / 100, 85 * map_width / 100, map_width];
	var y_points = [map_height + 10,          water_level, 60 * map_height /100,          water_level, map_height + 10];
	var island = {Algo = MAPALGO_Polygon, X = x_points, Y = y_points};
	island = {Algo = MAPALGO_Turbulence, Seed = Random(65536), Op = island, Amplitude = [25, 15], Scale = 10, Iterations = 1};	
	Draw("Earth", island);
	DrawMaterial("Earth-earth_root", island);
	DrawMaterial("Earth-earth_spongy", island);
	DrawMaterial("Rock-rock", island, 3, 10);
	DrawMaterial("Tunnel", island, 5, 20);
}

func DrawRuins(int map_width, int map_height, int water_level)
{
	var ruin_amount = 5;
	var ruin_x_start = 22 * map_width / 100;
	var ruin_x_end = 78 * map_width / 100;
	var ruin_width = (ruin_x_end - ruin_x_start) / (ruin_amount + 1);
	var ruin_y = 2 * map_height / 10;
	var ruin_min_height = (water_level - ruin_y) / 2;

	for (; ruin_x_start < ruin_x_end; ruin_x_start += ruin_width)
	{
		var y = RandomX(ruin_y, ruin_y + ruin_min_height);
		DrawRuin(ruin_x_start, y, ruin_width, water_level - y);
	}
}

func DrawRuin(int x, int y, int width, int height)
{
	var ruins = {Algo = MAPALGO_Rect, X = x, Y = y, Wdt = width, Hgt = height};
	ruins = {Algo = MAPALGO_Turbulence, Op = ruins, Iterations = 4};
	Draw("Earth", ruins);
	DrawMaterial("Rock-rock", ruins, 2, 10);
	DrawMaterial("Brick", ruins, 3, 40);
	DrawMaterial("Tunnel-brickback", ruins, 5, 20);
	DrawMaterial("Tunnel", ruins);
}
