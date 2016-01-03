/**
 Makes a weapon stackable, because the I do not know how to import the stackable feature.
 */

#appendto Firestone


func Initialize()
{
	// copy functions from the stackable library
	for (var property in Library_Stackable->GetProperties())
	{
		if (!this[property])
		{
			this[property] = Library_Stackable[property];
		}
	}

	// do the other stuff
	_inherited(...);
}