/**
 Player health display.
 @author Marky
*/

local menu_id;
local menu_def;

local player_bars;

static const GUI_PlayerHealthDisplay_BarWidth_Em = 40;


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
	menu_def = 
	{
		ID = 1,
		Target = this,
		Left = "0.5em", //"0%",
		Top = "10em",
		Right = "5.5em", //Format("0%%%s", ToEmString(GUI_PlayerHealthDisplay_BarWidth_Em)),		
		Bottom = "11em", //"5%",
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
