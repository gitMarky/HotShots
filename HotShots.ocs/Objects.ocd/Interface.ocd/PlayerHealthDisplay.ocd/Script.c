/**
 Player health display.
 @author Marky
*/

local menu_id;  // the main window GUI id
local menu_def; // the main window properties

local player_bars; // counter for player bars

static const GUI_PlrHlDis_WindowPosX_Percent = 0;
static const GUI_PlrHlDis_WindowPosX_10thEm = 1;
static const GUI_PlrHlDis_WindowWidth_10thEm = 50;
static const GUI_PlrHlDis_WindowPosY_10thEm = 70;

static const GUI_PlrHlDis_BarHeight_10thEm = 12;

global func GuiPlayerHealthDisplay()
{
	return FindObject(Find_ID(GUI_PlayerHealthDisplay));
}

func Initialize()
{
	Create();
	Hide();
}

func Create()
{
	var window_x = GUI_PlrHlDis_WindowPosX_10thEm;
	var window_width = GUI_PlrHlDis_WindowWidth_10thEm;
	var window_y = GUI_PlrHlDis_WindowPosY_10thEm;
	var window_height = 10;

	menu_def = 
	{
		ID = 1,
		Target = this,
		Left = PercentAndEm(0, window_x),
		Top = PercentAndEm(0, window_y),
		Right = PercentAndEm(0, window_x + window_width),		
		Bottom = PercentAndEm(0, window_y + window_height),
		BackgroundColor = RGBa(0, 0, 0, 0),
		Style = GUI_Multiple | GUI_VerticalLayout | GUI_IgnoreMouse | GUI_NoCrop
	};
	
	menu_id = GuiOpen(menu_def);
}

func Display()
{
	GuiUpdate({Player = nil}, menu_id, nil, this);
	UpdatePlayerDisplays();
}

func Hide()
{
	GuiUpdate({Player = NO_OWNER}, menu_id, nil, this);
}

// Update everything
private func UpdatePlayerDisplays()
{
	// add as many player bars as there are players
	while (player_bars < GetPlayerCount())
	{
		AddPlayerBar();
	}

	// update display for existing bars
	for (var i = 0; i < player_bars; i++)
	{
		UpdatePlayerDisplay(i);
	}
}

private func AddPlayerBar()
{
	// build new submenu
	var bar = {
		ID = GetPlayerBarID(player_bars),
		Target = this,
		Left = "0%",
		Right = "100%",
		Top = "0%",
		Bottom = PercentAndEm(0, 12),
		Margin = ["0em", "0.1em"],
		BackgroundColor = RGB(10, 5, 5),
		Style = GUI_NoCrop,
		Priority = 3,

		Component_Fill = {
			Target = this,
			Left = "0%",
			Right = "100%",
			Margin = ["0.2em"],
			BackgroundColor = RGB(255, 255, 255),
			Priority = 4,
		}
	};

	menu_def[GetPlayerBarName(player_bars)] = bar;

	// add to existing gui
	GuiUpdate(menu_def, menu_id);
	
	// increase number of bars
	player_bars++;
}

private func UpdatePlayerDisplay(int index)
{
	var player = GetPlayerByIndex(index);
	
	if (player >= 0)
	{
		var health_max = 0;
		var health_cur = 0;
		for (var c = 0; c < GetCrewCount(player); c++)
		{
			var crew = GetCrew(player, c);
			// Health
			var health_phys = crew->~GetMaxEnergy();
	
			if (health_phys)
			{
				health_max += health_phys;
				health_cur += crew->GetEnergy();
			}
		}
		
		if (health_max)
		{
			menu_def[GetPlayerBarName(index)].Player = nil; // displays for all players
			menu_def[GetPlayerBarName(index)].Component_Fill.Right = GetPlayerBarFillWidth(health_cur, health_max);
			menu_def[GetPlayerBarName(index)].Component_Fill.BackgroundColor = GetPlayerColor(player);
		}
		//Log("The actual update, player had %d of %d health", health_cur, health_max);
	}
	else
	{
		menu_def[GetPlayerBarName(index)].Player = NO_OWNER; // hides for all players
	}
	
	GuiUpdate(menu_def, menu_id);
}

private func PercentAndEm(int percent, int em, int factor)
{
	return Format("%d%%%s", percent, ToEmString(em, factor));
}

private func GetPlayerBarID(int bar)
{
	return 100 + bar;
}

private func GetPlayerBarName(int bar)
{
	return Format("bar%d", bar);
}

private func GetPlayerBarTop(int bar)
{
	return bar * GUI_PlrHlDis_BarHeight_10thEm;
}

private func GetPlayerBarBottom(int bar)
{
	return (bar + 1) * GUI_PlrHlDis_BarHeight_10thEm;
}

private func GetPlayerBarFillWidth(int health_current, int health_max)
{
	return ToPercentString(1000 * health_current / health_max);
}
