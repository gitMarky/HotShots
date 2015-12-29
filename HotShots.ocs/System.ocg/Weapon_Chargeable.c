/**
 Effects for weapon charging.
 @author Marky
 */


static const FX_ChargeGUI = "IntChargeGui";
 

global func AddChargeGui()
{
	if (!this)
	{
		FatalError("This function has to be called from object context!");
	}

	var effect = GetChargeGui();
	
	if (!effect)
	{
		effect = AddEffect(FX_ChargeGUI, this, 1);
	}
	
	effect.charge = 0;
	effect.max_charge = 0;
	
	return effect;
}


global func GetChargeGui()
{
	if (!this)
	{
		FatalError("This function has to be called from object context!");
	}

	return GetEffect(FX_ChargeGUI, this);
}


global func RemoveChargeGui()
{
	if (!this)
	{
		FatalError("This function has to be called from object context!");
	}

	var effect = GetChargeGui();
	
	if (effect)
	{
		RemoveEffect(nil, this, effect);
	}
}


global func ChargeGuiSetMaxCharge(int amount)
{
	var effect = GetChargeGui();
	if (!effect)
	{
		FatalError("This function needs a charge GUI first. Call AddChargeGui on this object");
	}
	
	effect.max_charge = amount;
}


global func ChargeGuiDoCharge(int amount)
{
	var effect = GetChargeGui();
	if (!effect)
	{
		FatalError("This function needs a charge GUI first. Call AddChargeGui on this object");
	}

	if (effect.max_charge)
	{
		effect.charge = BoundBy(effect.charge + amount, 0, effect.max_charge);
	}
	else
	{
		effect.charge = Max(0, effect.charge + amount);
	}
}


global func ChargeGuiGetCharge()
{
	var effect = GetChargeGui();
	if (!effect)
	{
		FatalError("This function needs a charge GUI first. Call AddChargeGui on this object");
	}

	return effect.charge;
}


global func ChargeGuiGetChargePercent()
{
	var effect = GetChargeGui();
	if (!effect)
	{
		FatalError("This function needs a charge GUI first. Call AddChargeGui on this object");
	}
	if (!effect.max_charge)
	{
		FatalError("This function needs a maximum charge first. Call ChargeGuiSetMaxCharge on this object");
	}
	
	return 100 * effect.charge / effect.max_charge;
}
