/**
 The object interaction menu should display a selection of
 weapons instead.
 */

#appendto GUI_ObjectInteractionMenu

/*
	This is the entry point.
	Create a menu for an object (usually from current_objects) and also create everything around it if it's the first time a menu is opened.
*/
func OpenMenuForObject(object obj, int slot, bool forced)
{
	forced = forced ?? false;
	// clean up old menu
	var old_menu = current_menus[slot];
	var other_menu = current_menus[1 - slot];
	if (old_menu)
	{
		// Notify other object of the closed menu.
		DoInteractionMenuClosedCallback(old_menu.target);
		
		// Re-enable entry in (other!) sidebar.
		if (other_menu)
		{
			GuiUpdate({Symbol = nil}, current_main_menu_id, 1 + 1 - slot, old_menu.target);
		}
		// ..and close old menu.
		old_menu.menu_object->RemoveObject();
	}
	current_menus[slot] = nil;
	// before creating the sidebar, we have to create a new entry in current_menus, even if it contains not all information
	current_menus[slot] = {target = obj, forced = forced};
	// clean up old inventory-check effects that are not needed anymore
	var effect_index = 0, inv_effect = nil;
	while (inv_effect = GetEffect("IntRefreshContentsMenu", this, effect_index))
	{
		if (inv_effect.obj != current_menus[0].target && inv_effect.obj != current_menus[1].target)
			RemoveEffect(nil, nil, inv_effect);
		else
			++effect_index;
	}
	// Create a menu with all interaction possibilities for an object.
	// Always create the side bar AFTER the main menu, so that the target can be copied.
	var main = CreateMainMenu(obj, slot);
	// To close the part_menu automatically when the main menu is closed. The sidebar will use this target, too.
	current_menus[slot].menu_object = main.Target;
	// Now, the sidebar.
	var sidebar = CreateSideBar(slot);
	
	var sidebar_size_em = ToEmString(InteractionMenu_SideBarSize);
	var part_menu =
	{
		Left = "0%", Right = "50%-3em",
		Bottom = "100%-7em",
		sidebar = sidebar, main = main,
		Target = current_menus[slot].menu_object,
		ID = 1
	};
	
	if (slot == 1)
	{
		part_menu.Left = "50%-1em";
		part_menu.Right = "100%-2em";
	}
	
	if (this.minimized)
	{
		part_menu.Bottom = nil; // maximum height
	}
	

	// need to open a completely new menu?
	if (!current_main_menu_id)
	{
		if (!current_description_box.target)
		{
			current_description_box.target = CreateDummy();
			current_description_box.symbol_target = CreateDummy();
			current_description_box.desc_target = CreateDummy();
		}
		if (!current_center_column_target)
			current_center_column_target = CreateDummy();
		
		var root_menu =
		{
			_one_part = part_menu,
			Target = this,
			Decoration = GUI_MenuDeco,
			BackgroundColor = RGB(0, 0, 0),
			minimize_button = 
			{
				Bottom = "100%",
				Top = "100% - 2em",
				Left = "100% - 2em",
				Tooltip = "$Minimize$",
				Symbol = Icon_Arrow,
				GraphicsName = "Down",
				BackgroundColor = {Std = nil, OnHover = 0x50ffff00},
				OnMouseIn = GuiAction_SetTag("OnHover"),
				OnMouseOut = GuiAction_SetTag("Std"),
				OnClick = GuiAction_Call(this, "OnToggleMinimizeClicked")
			},
			center_column =
			{
				Left = "50%-3em",
				Right = "50%-1em",
				Top = "1.75em",
				Bottom = "100%-7em",
				Style = GUI_VerticalLayout,
				move_all_left =
				{
					Target = current_center_column_target,
					ID = 10 + 0,
					Right = "2em", Bottom = "3em",
					Style = GUI_TextHCenter | GUI_TextVCenter,
					Symbol = Icon_MoveItems, GraphicsName = "Left",
					Tooltip = "",
					BackgroundColor ={Std = 0, Hover = 0x50ffff00},
					OnMouseIn = GuiAction_SetTag("Hover"),
					OnMouseOut = GuiAction_SetTag("Std"),
					OnClick = GuiAction_Call(this, "OnMoveAllToClicked", 0)
				},
				move_all_right =
				{
					Target = current_center_column_target,
					ID = 10 + 1,
					Right = "2em", Bottom = "3em",
					Style = GUI_TextHCenter | GUI_TextVCenter,
					Symbol = Icon_MoveItems,
					Tooltip = "",
					BackgroundColor ={Std = 0, Hover = 0x50ffff00},
					OnMouseIn = GuiAction_SetTag("Hover"),
					OnMouseOut = GuiAction_SetTag("Std"),
					OnClick = GuiAction_Call(this, "OnMoveAllToClicked", 1)
				}
			},
			description_box =
			{
				Top = "100%-5em",
				Right = "100% - 2em",
				Margin = [sidebar_size_em, "0em"],
				BackgroundColor = RGB(25, 25, 25),
				symbol_part =
				{
					Right = "5em",
					Symbol = nil,
					Margin = "0.5em",
					ID = 1,
					Target = current_description_box.symbol_target
				},
				desc_part =
				{
					Left = "5em",
					Margin = "0.5em",
					ID = 1,
					Target = current_description_box.target,
					real_contents = // nested one more time so it can dynamically be replaced without messing up the layout
					{
						ID = 1,
						Target = current_description_box.desc_target
					}
				}
			}
		};
		
		// Allow the menu to be closed with a clickable button.
		var close_button = GuiAddCloseButton(root_menu, this, "Close");
		
		// Special setup for a minimized menu.
		if (this.minimized)
		{
			root_menu.Top = "75%";
			root_menu.minimize_button.Tooltip = "$Maximize$";
			root_menu.minimize_button.GraphicsName = "Up";
			root_menu.center_column.Bottom = nil; // full size
			root_menu.description_box = nil;
		}
		
		current_main_menu_id = GuiOpen(root_menu);
	}
	else // menu already exists and only one part has to be added
	{
		GuiUpdate({_update: part_menu}, current_main_menu_id, nil, nil);
	}
	
	// Show "put/take all items" buttons if applicable. Also update tooltip.
	var show_grab_all = current_menus[0] && current_menus[1];
	// Both objects have to be containers.
	show_grab_all = show_grab_all 
					&& (current_menus[0].target->~IsContainer())
					&& (current_menus[1].target->~IsContainer());
	// And neither must disallow interaction.
	show_grab_all = show_grab_all
					&& !current_menus[0].target->~RejectInteractionMenu(cursor)
					&& !current_menus[1].target->~RejectInteractionMenu(cursor);
	if (show_grab_all)
	{
		current_center_column_target.Visibility = VIS_Owner;
		for (var i = 0; i < 2; ++i)
			GuiUpdate({Tooltip: Format("$MoveAllTo$", current_menus[i].target->GetName())}, current_main_menu_id, 10 + i, current_center_column_target);
	}
	else
	{
		current_center_column_target.Visibility = VIS_None;
	}
	
	// Now tell all user-provided effects for the new menu that the menu is ready.
	// Those effects can be used to update only very specific menu entries without triggering a full refresh.
	for (var menu in current_menus[slot].menus)
	{
		if (!menu.entries) continue;
		
		for (var entry in menu.entries)
		{
			if (!entry.fx) continue;
			EffectCall(nil, entry.fx, "OnMenuOpened", current_main_menu_id, entry.ID, menu.menu_object);
		}
	}
	
	// Finally disable object for selection in other sidebar, if available.
	if (other_menu)
	{
		GuiUpdate({Symbol = Icon_Cancel}, current_main_menu_id, 1 + 1 - slot, obj);
	}
	
	// And notify the object of the fresh menu.
	DoInteractionMenuOpenedCallback(obj);
}

/*
	Generates and creates one side of the menu.
	Returns the proplist that will be put into the main menu on the left or right side.
*/
func CreateMainMenu(object obj, int slot)
{
	var big_menu =
	{
		Target = CreateDummy(),
		Priority = 5,
		Right = Format("100%% %s", ToEmString(-InteractionMenu_SideBarSize)),
		container =
		{
			Top = "1em",
			Style = GUI_VerticalLayout,
			BackgroundColor = RGB(25, 25, 25),
		},
		headline = 
		{
			Bottom = "1em",
			Text = obj->GetName(),
			Style = GUI_TextHCenter | GUI_TextVCenter,
			BackgroundColor = 0xff000000
		}
		
	};
	var container = big_menu.container;
	
	if (slot == 0)
	{
		big_menu.Left = ToEmString(InteractionMenu_SideBarSize);
		big_menu.Right = "100%";
	}
	
	// Do virtually nothing if the building/object is not ready to be interacted with. This can be caused by several things.
	var error_message = obj->~RejectInteractionMenu(cursor);
	
	if (error_message)
	{
		if (GetType(error_message) != C4V_String)
			error_message = "$NoInteractionsPossible$";
		container.Style = GUI_TextVCenter | GUI_TextHCenter;
		container.Text = error_message;
		current_menus[slot].menus = [];
		return big_menu;
	}
	
	var menus = obj->~GetInteractionMenus(cursor) ?? [];
	// get all interaction info from the object and put it into a menu
	// contents first
	if (obj->~IsContainer())
	{
		var info =
		{
			flag = InteractionMenu_Contents,
			title = "$Contents$",
			entries = [],
			entries_callback = nil,
			callback = "OnContentsSelection",
			callback_target = this,
			decoration = GUI_MenuDecoInventoryHeader,
			Priority = 10
		};
		PushBack(menus, info);
	}
	
	current_menus[slot].menus = menus;
	
	// now generate the actual menus from the information-list
	for (var i = 0; i < GetLength(menus); ++i)
	{
		var menu = menus[i];
		if (!menu.flag)
			menu.flag = InteractionMenu_Custom;
		if (menu.entries_callback)
			menu.entries = obj->Call(menu.entries_callback, cursor);
		if (menu.entries == nil)
		{
			FatalError(Format("An interaction menu did not return valid entries. %s -> %v() (object %v)", obj->GetName(), menu.entries_callback, obj));
			continue;
		}
		menu.menu_object = CreateObject(MenuStyle_Grid);
		
		menu.menu_object.Top = "+1em";
		menu.menu_object.Priority = 2;
		menu.menu_object->SetPermanent();
		menu.menu_object->SetFitChildren();
		menu.menu_object->SetMouseOverCallback(this, "OnMenuEntryHover");
		for (var e = 0; e < GetLength(menu.entries); ++e)
		{
			var entry = menu.entries[e];
			entry.unique_index = ++menu.entry_index_count;
			// This also allows the interaction-menu user to supply a custom entry with custom layout f.e.
			var added_entry = menu.menu_object->AddItem(entry.symbol, entry.text, entry.unique_index, this, "OnMenuEntrySelected", { slot = slot, index = i }, entry["custom"]);
			// Remember the menu entry's ID (e.g. for passing it to an update effect after the menu has been opened).
			entry.ID = added_entry.ID;
		}
		
		var all =
		{
			Priority = menu.Priority ?? i,
			Style = GUI_FitChildren,
			title_bar = 
			{
				Priority = 1,
				Style = GUI_TextVCenter | GUI_TextHCenter,
				Bottom = "+0.75em",
				Text = menu.title,
				BackgroundColor = 0xa0000000,
				//Decoration = menu.decoration
				hline = {Bottom = "0.05em", BackgroundColor = RGB(100, 100, 100)}
			},
			Margin = [nil, nil, nil, "0.25em"],
			real_menu = menu.menu_object,
			spacer = {Left = "0em", Right = "0em", Bottom = "3em"} // guarantees a minimum height
		};
		if (menu.flag == InteractionMenu_Contents)
			all.BackgroundColor = RGB(0, 50, 0);
		else if (menu.BackgroundColor)
				all.BackgroundColor = menu.BackgroundColor;
		else if (menu.decoration)
			menu.menu_object.BackgroundColor = menu.decoration->FrameDecorationBackClr();
		GuiAddSubwindow(all, container);
	}
	
	// add refreshing effects for all of the contents menus
	for (var i = 0; i < GetLength(menus); ++i)
	{
		if (!(menus[i].flag & InteractionMenu_Contents)) continue;
		AddEffect("IntRefreshContentsMenu", this, 1, 1, this, nil, obj, slot, i);
	}
	
	return big_menu;
}

