/**
 Player health display.
 @author Marky
*/

local menu_id;  // the main window GUI id
local menu_def; // the main window properties

local player_bars; // counter for player bars

static const GUI_PlrHlDis_WindowPosX_Percent = 0;
static const GUI_PlrHlDis_WindowPosX_10thEm = 5;
static const GUI_PlrHlDis_WindowWidth_10thEm = 50;


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
	var window_y = 100;
	var window_height = 10;

	menu_def = 
	{
		ID = 1,
		Target = this,
		Left = PercentAndEm(0, window_x),
		Top = PercentAndEm(0, window_y),
		Right = PercentAndEm(0, window_x + window_width),		
		Bottom = PercentAndEm(0, window_y + window_height),
		BackgroundColor = RGBa(40, 40, 40, 120),
		Style = GUI_Multiple | GUI_VerticalLayout | GUI_IgnoreMouse
	};
	
	menu_id = GuiOpen(menu_def);
}

func Display()
{
	GuiUpdate({Player = nil}, menu_id, nil, this);
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
		Bottom = PercentAndEm(0, 10),
		BackgroundColor = RGB(255, 255, 255),
		Style = GUI_NoCrop,
		Priority = 3,

		Component_Fill = {
			Target = this,
			Left = "0%",
			Right = "100%",
			Margin = ["0.2em"],
			BackgroundColor = RGB(100, 100, 100),
			Priority = 4,
		}
	};

	menu_def[GetPlayerBarName(player_bars)] = bar;

	// add to existing gui
	GuiUpdate(menu_def, menu_id);
	
	// increase number of bars
	player_bars++;
}

private func UpdatePlayerDisplay(int player)
{
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
}

private func GetPlayerBarBottom(int bar)
{
}

private func GetPlayerBarFillWidth(int health_current, int health_max)
{
}