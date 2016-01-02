#appendto Environment_Configuration

public func ReleaseCrew(object crew, bool instant)
{
	if (Goal()->IsProtectedCrew(crew)) return;
	
	_inherited(crew, instant, ...);
}

// remove item configuration
public func CanConfigureSpawnPoints()
{
	return false;
}

// remove bots configuration
public func CanConfigureBots()
{
	return false;
}

// custom menu entries
public func MainMenuAddItemCustom(object player)
{
	MainMenuAddItemCrew(player);
}

private func MainMenuAddItemCrew(object player)
{
	player->AddMenuItem("$TxtConfigureCrew$", "MenuConfigureCrew", GAMECONFIG_Icon_Bots, nil, player);
}

protected func MenuConfigureCrew(id menu_symbol, object player, int selection)
{
	CreateConfigurationMenu(player, menu_symbol, "$TxtConfigureCrew$");

	var number_crew = Goal()->GetCrewCountCurrent();
	var number_max = Goal()->GetCrewCountMax();
	
	var command0, command1, command2, command3;
	var caption0 = Format("$TxtNumberCrew$", number_crew, number_max);
	var caption1 = "$MoreCrew$";
	var caption2 = "$LessCrew$";
	var caption3 = "$Finished$";
	
	command0 = Format("MenuConfigureCrew(%i, Object(%d), %d)", menu_symbol, player->ObjectNumber(), 0);
	command1 = Format("ChangeCrewAmount(%i, Object(%d), %d, %d)", menu_symbol, player->ObjectNumber(), 1, number_crew +1);
	command2 = Format("ChangeCrewAmount(%i, Object(%d), %d, %d)", menu_symbol, player->ObjectNumber(), 2, number_crew -1);
	command3 = "OpenMainMenu()";

	player->AddMenuItem(caption0, command0, GetIcon(menu_symbol));
	player->AddMenuItem(caption1, command1, Icon_Plus, nil, nil, "$MoreCrew$");
	player->AddMenuItem(caption2, command2, Icon_Minus, nil, nil, "$LessCrew$");
	player->AddMenuItem(caption3, command3, Icon_Ok, nil, nil, "$Finished$");
	
	player->SelectMenuItem(selection);
}

private func ChangeCrewAmount(id menu_symbol, object player, int selection, int amount)
{
	Goal()->SetCrewCount(amount);
	MenuConfigureCrew(GAMECONFIG_Icon_Bots, player, selection);
}

