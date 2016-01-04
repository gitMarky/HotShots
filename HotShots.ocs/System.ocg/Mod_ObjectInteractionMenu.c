/**
 The object interaction menu should display a selection of
 weapons instead.
 */

#appendto GUI_ObjectInteractionMenu


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

// May not transfer all!
public func OnMoveAllToClicked(int menu_id)
{
	Sound("BalloonPop", true, nil, GetOwner());
	return;
}

