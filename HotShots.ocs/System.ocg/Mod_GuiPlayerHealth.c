#appendto GUI_Controller_Goal

// HUD margin and size in tenths of em.
static const GUI_Controller_PlayerBar_IconSize = 100; // Used by the next clonk display + 1em for the bars (health, breath)
static const GUI_Controller_PlayerBar_IconMargin = 6;
static const GUI_Controller_PlayerBar_CursorSize = 40; // Cursor size is bigger
static const GUI_Controller_PlayerBar_CursorMargin = 3;
static const GUI_Controller_PlayerBar_BarSize = 10;
static const GUI_Controller_PlayerBar_BarMargin = 5;


local player_bars;

//local goal_player_gui_id;
//local goal_player_gui_menu;

// Callback from the goal library: display this goal.
public func OnGoalUpdate(object goal)
{
	if (goal == Goal())
	{
		UpdatePlayerDisplays();
	}
	
	_inherited(goal);
}


public func Initialize()
{
	player_bars = [];
	_inherited(...);
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


private func UpdatePlayerDisplay(int i)
{
	Log("Update for player %d", i);
	var player = GetPlayerByIndex(i);
	
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
			ShowPlayerBar(i, player);
			SetPlayerBarValue(i, 1000 * health_cur / health_max, health_cur);
		}
		Log("The actual update, player had %d of %d health", health_cur, health_max);
	}
	else
		HidePlayerBar(i);
}


private func AddPlayerBar()
{
	Log("Add Player bar");

	var foreground = RGB(255, 255, 255);
	var background = RGB(40, 40, 40);

	var y_begin = GUI_Controller_PlayerBar_CursorMargin + GUI_Controller_PlayerBar_CursorSize;
	var y_end = y_begin + GUI_Controller_PlayerBar_BarSize;

	var bar_number = GetLength(player_bars);
	var new_bar = 
	{
		ID = GetPlayerBarID(bar_number),
		shown = false,
		associated_player = -1
	};
	PushBack(player_bars, new_bar);

	goal_gui_menu[GetPlayerBarName(bar_number)] =
	{
		Target = this,
		Player = NO_OWNER,
		Style = GUI_NoCrop,
		ID = new_bar.ID,
		Left = "0%",
		Right = Format("0%%%s", ToEmString(GUI_Controller_PlayerBar_CursorSize)),
		Top = ToEmString(y_begin),
		Bottom = ToEmString(y_end),
		BackgroundColor = background,
		Priority = 3,
		fill =
		{
			Target = this,
			Left = "0%",
			Margin = ["0em", "0.1em"],
			BackgroundColor = foreground,
			Priority = 4
		},
		text =
		{
			Target = this,
			Top = "-0.45em",
			Style = GUI_TextHCenter,
			Text = "",
			Priority = 5
		}
	};

	GuiUpdate(goal_gui_menu, goal_gui_id);
	UpdatePlayerDisplay(bar_number);

	return bar_number;
}


// Shows the bar that was saved in player_bars[bar]
private func ShowPlayerBar(int bar, int player)
{
	if (!player_bars[bar]) return;
	if (GetOwner() == NO_OWNER) return;
	if (!player_bars[bar].shown)
	{
	
		var top = GUI_Controller_PlayerBar_CursorMargin + GUI_Controller_PlayerBar_CursorSize;
		var i = 0;
		while (i < bar)
		{
			if (player_bars[i] && player_bars[i].shown)
				top += GUI_Controller_PlayerBar_BarSize + GUI_Controller_PlayerBar_BarMargin;
			i++;
		}
		var bottom = ToEmString(top + GUI_Controller_PlayerBar_BarSize);
		top = ToEmString(top);
		
		player_bars[bar].shown = GuiUpdate({ Player = GetOwner(), Top = top, Bottom = bottom }, goal_gui_id, player_bars[bar].ID, this);
	}

	// update player and color
	player_bars[bar].associated_player = player;
	GuiUpdate({fill = {BackgroundColor = GetPlayerColor(player)}}, goal_gui_id, player_bars[bar].ID, this);
}


// Sets the fill status of the bar. value is between 0 and 1000
// Shows text_val before the bar if given
private func SetPlayerBarValue(int bar, int value, int text_val)
{
	if (!player_bars[bar]) return;
	value = BoundBy(value, 0, 1000);
	var plr = GetOwner();
	var bar_text = "";
	if (text_val) bar_text = Format("<c dddd00>%d</c>", text_val);
	// Displaying the fill with Top = 100% creates an unwanted scrollbar
	//if (value == 0) plr = NO_OWNER;

	GuiUpdate({ fill = { Player = plr, Right = ToPercentString(value) }, text = { Text = bar_text } }, goal_gui_id, player_bars[bar].ID, this);
}


// Hides the bar that was saved in player_bars[bar]
private func HidePlayerBar(int bar)
{
	if (!player_bars[bar]) return;
	if (!player_bars[bar].shown) return;

	GuiUpdate({ Player = NO_OWNER }, goal_gui_id, player_bars[bar].ID, this);
	player_bars[bar].shown = false;

	// Update position of all following bars
	for (var i = bar; i < GetLength(player_bars); i++)
		if (player_bars[i].shown)
		{
			player_bars[i].shown = false;
			ShowPlayerBar(i);
		}
}


private func GetPlayerBarID(int bar)
{
	return 100 + bar;
}


private func GetPlayerBarName(int bar)
{
	return Format("bar%d", GetPlayerBarID(bar));
}
