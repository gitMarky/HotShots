/**
 The object interaction menu should display a selection of
 weapons instead.
 */

#appendto GUI_ObjectInteractionMenu


// updates the objects shown in the side bar
// if an object which is in the menu on the left or on the right is not in the side bar anymore, another object is selected
func UpdateObjects(array new_objects)
{
	// need to close a menu?
	for (var i = 0; i < GetLength(current_menus); ++i)
	{
		if (!current_menus[i]) continue; // todo: I don't actually know why this can happen.
		if (current_menus[i].forced) continue;
		
		var target = current_menus[i].target;
		// Still existant? Nothing to do!
		if (GetIndexOf(new_objects, target) != -1) continue;
		// not found? close!
		// sub menus close automatically (and remove their dummy) due to a clever usage of OnClose
		current_menus[i].menu_object->RemoveObject();
		current_menus[i] = nil;
		// Notify the target of the now closed menu.
		DoInteractionMenuClosedCallback(target);
	}
	
	current_objects = new_objects;
	
	// need to fill an empty menu slot?
	for (var i = 0; i < 2; ++i)
	{
		// If the menu already exists, just update the sidebar.
		if (current_menus[i] != nil)
		{
			RefreshSidebar(i);
			continue;
		}
		// look for next object to fill slot
		for (var obj in current_objects)
		{
			// but only if the object's menu is not already open
			var is_already_open = false;
			for (var menu in current_menus)
			{
				if (!menu) continue; // todo: I don't actually know why that can happen.
				if (menu.target != obj) continue;
				is_already_open = true;
				break;
			}
			if (is_already_open) continue;
			// use object to create a new menu at that slot
			OpenMenuForObject(obj, i);
			break;
		}
	}
}

func FxIntCheckObjectsTimer(target, effect fx)
{
	var new_objects = [target, Goal()->GetPlayerMaterial(target->GetOwner())];
	var equal = GetLength(new_objects) == GetLength(current_objects);
	
	if (equal)
	{
		for (var i = GetLength(new_objects) - 1; i >= 0; --i)
		{
			if (new_objects[i] == current_objects[i]) continue;
			equal = false;
			break;
		}
	}
	if (!equal)
		UpdateObjects(new_objects);
}

private func OnContentsSelection(symbol, extra_data)
{
	var target = current_menus[extra_data.slot].target;
	if (!target) return;
	// no target to swap to?
	if (!current_menus[1 - extra_data.slot]) return;
	var other_target = current_menus[1 - extra_data.slot].target;
	if (!other_target) return;
	
	// Only if the object wants to be interacted with (hostility etc.)
	if (other_target->~RejectInteractionMenu(cursor)) return;
	
	// Allow transfer only into containers.
	if (!other_target->~IsContainer())
	{
		// Todo: other sound for "nope".
		Sound("LightMetalHit*", nil, 10, GetController(), nil, nil, -50);
		return;
	}
	
	var transfer_only_one = GetPlayerControlState(GetOwner(), CON_ModifierMenu1) == 0; // Transfer ONE object of the stack?
	var to_transfer = nil;
	
	if (transfer_only_one)
	{
		for (var possible in extra_data.objects)
		{
			if (possible == nil) continue;
			to_transfer = [possible];
			break;
		}
	}
	else
	{
		to_transfer = extra_data.objects;
	}
	
	var successful_transfers = 0;
	
	// Try to transfer all the previously selected items.
	for (var obj in to_transfer)
	{
		if (!obj) continue;
		
		var handled = false;
		// Does the object not want to leave the other container anyway?
		if (!obj->Contained() || !obj->~QueryRejectDeparture(target))
		{
			// If stackable, always try to grab a full stack.
			// Imagine armory with 200 arrows, but not 10 stacks with 20 each but 200 stacks with 1 each.
			if (obj->~IsStackable())
			{
				var others = FindObjects(Find_Container(target), Find_ID(symbol), Find_Exclude(obj));
				for (var other in others) 
				{
					if (obj->IsFullStack()) break;
					other->TryAddToStack(obj);
				}
			}
			
			// More special handling for Stackable items..
			handled = obj->~TryPutInto(other_target);
			// Try to normally collect the object otherwise.
			if (!handled)
				handled = other_target->Collect(obj, true);
		}
		if (handled)
			successful_transfers += 1;
	}
	
	// Did we at least transfer one item?
	if (successful_transfers > 0)
	{
		Sound("SoftTouch*", true, nil, GetOwner());
		return true;
	}
	else
	{
		Sound("BalloonPop", true, nil, GetOwner());
		return false;
	}
}

// May not transfer all!
public func OnMoveAllToClicked(int menu_id)
{
	Sound("BalloonPop", true, nil, GetOwner());
	return;
}

