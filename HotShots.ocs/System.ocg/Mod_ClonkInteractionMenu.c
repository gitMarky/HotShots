#appendto Clonk

public func RejectInteractionMenu(object to)
{
	if (Contents()) return true;

	return _inherited(to);
}

protected func Collection2(object obj)
{
	// close contents menu on collection, because this usually
	// means that we selected an item from the player inventory
	if (GetMenu() && GetMenu()->~IsContentMenu())
	{
		TryCancelMenu();
	}
	
	return _inherited(obj);
}