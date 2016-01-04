#appendto Clonk

local turn_contents; // collected contents this turn?

public func RejectInteractionMenu(object to)
{
	if (Contents() || turn_contents) return true;

	return _inherited(to);
}

protected func Collection2(object obj)
{
	// close contents menu on collection, because this usually
	// means that we selected an item from the player inventory
	if (GetMenu() && GetMenu()->~IsContentMenu())
	{
		ScheduleCall(this, this.TryCancelMenu, 1);
	}
	
	turn_contents = true;
	
	return _inherited(obj);
}