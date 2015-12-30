/**
 Player health display.
 @author Marky
*/

local menu_id;
local menu_def;

local player_bars;

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
	while (GetLength(player_bars) < GetPlayerCount())
	{
		AddPlayerBar();
	}

	for (var i = 0; i < GetLength(player_bars); i++)
	{
		UpdatePlayerDisplay(i);
	}
}

private func AddPlayerBar()
{
}

private func UpdatePlayerDisplay(int player)
{
}

private func PercentAndEm(int percent, int em, int factor)
{
	return Format("%d%%%s", percent, ToEmString(em, factor));
}
