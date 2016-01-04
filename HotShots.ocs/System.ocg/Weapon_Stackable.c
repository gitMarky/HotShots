/**
 Makes a weapon stackable, because the I do not know how to import the stackable feature.
 */

#appendto Firestone
#appendto Dynamite
#appendto Shovel

func Initialize()
{
	// copy functions from the stackable library
	for (var property in Library_Stackable->GetProperties())
	{
		if (!this[property])
		{
				this[property] = Library_Stackable[property];
		}

		if (property == "TryPutInto")
		{
			this.TryPutIntoInherited = Library_Stackable[property];
		}
	}

	// do the other stuff
	_inherited(...);
}

public func TryPutInto(object into, bool only_add_to_existing_stacks)
{
	Log("Try put into %v", into);
	
	if (!into) FatalError("Cannot put object into nil");

	// takes only one item out of the player inventory stack at a time
	if (Contained() && Contained()->GetID() == Dummy)
	{
		Log("Is in player inventory");

		var item = this->~TakeObject();
		if (!item) FatalError("An item should be takeable from this stack");
		
		if (item->TryPutInto(into, only_add_to_existing_stacks))
		{
			return true;
		}
		else // add the item back to the stack
		{
			this->~Stack(item);
			return false;
		}
	}
	else
	{
		// default handling?
		var handled = Call(this.TryPutIntoInherited, into, only_add_to_existing_stacks);

		//Log("Handled adding to existing stack? %v", handled);
		
		// if not: add this object to the inventory
		if (!handled)
		{
			if (!into->FindContents(GetID()))
			{
				handled = into->Collect(this, true);
			}
		}
		
		return handled;
	}	
}

public func RemoveOnActionPhaseEnd()
{
	var handled = _inherited();
	if (handled) return true;

	var player = TurnManager()->GetActivePlayer();
	var inventory = Goal()->GetPlayerMaterial(player);
	handled = TryPutInto(inventory);
	//Log("Transferred to player inventory? %v", handled);
	return handled;
}
