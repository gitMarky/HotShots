global func FxFireTimer(object target, proplist effect, int time)
{
	// safety
	if (!target) return FX_Execute_Kill;
	
	// get cause
	//if(!GetPlayerName(effect.caused_by)) effect.caused_by=NO_OWNER;;
		
	// strength changes over time
	if ((effect.strength < 100) && (time % 8 == 0))
	{
		if (effect.strength < 50)
		{
			if (time % 8 == 0)
			{
				effect.strength = Max(effect.strength - 1, 0);
				if (effect.strength <= 0)
			 		return FX_Execute_Kill;
			}
		}
		else
		{
			effect.strength = Min(100, effect.strength + 2);
			
			if (effect.strength > 49)
			{
				effect.smoke.R = 127;
				effect.smoke.G = 127;
				effect.smoke.B = 127;
			}
		}
			
		// update particle properties that depend on the effect strength
		EffectCall(target, effect, "UpdateEffectProperties");
	}
		
	// target is in liquid?
	if (time % (20 + effect.FreqReduction) == 0)
	{	
		var mat;
		if (mat = GetMaterial())
			if (GetMaterialVal("Extinguisher", "Material", mat))
			{
				var steam =
				{
					Size = PV_Linear(effect.width*2, effect.width*4),
					Alpha = PV_Linear(87,0),
					R = 255,
					G = 255,
					B = 255,
					Rotation = PV_Random(0, 359)
				};
				CreateParticle("Dust", 0, -5, 0, 0, 180, steam, 2);
				return FX_Execute_Kill;
			}
	
		// check spreading of fire
		if (effect.strength > 10)
		{
			var container = target->Contained(), inc_objs;
			// If contained do not incinerate anything. This would mostly affect inventory items and burns the Clonk too easily while lacking feedback.
			if (container)
			{
				inc_objs = [];
			}
			// Or if not contained incinerate all objects outside the burning object.
			// Do not incinerate inventory objects, since this, too, would mostly affect Clonks and losing one's bow in a fight without noticing it is nothing but annoying to the player.
			else
			{
				inc_objs = FindObjects(Find_AtRect(-effect.width/2, -effect.height/2, effect.width, effect.height), Find_Exclude(target), Find_Layer(target->GetObjectLayer()), Find_NoContainer());
			}
			// Loop through the selected set of objects and check contact incinerate.
			for (var obj in inc_objs)
			{
				// Check if the obj still exists, an object could be have been removed in this loop.
				if (!obj)
					continue;
				
				if (obj->GetCategory() & C4D_Living)
					if (!obj->GetAlive()) 
						continue;
				
				var inf = obj.ContactIncinerate;
				if (!inf)
					continue;
				var amount = (effect.strength / 3) / Max(1, inf);
	
				if (!amount)
					continue;
					
				var old_fire_value = obj->OnFire();
				if(old_fire_value < 100) // only if the object was not already fully ablaze
				{
					// incinerate the other object a bit
					obj->Incinerate(Max(10, amount), effect.caused_by, false, effect.incinerating_obj);
				
					// Incinerating other objects weakens the own fire.
					// This is done in order to make fires spread slower especially as a chain reaction.
					var min = Min(10, effect.strength);
					if(effect.strength > 50) min = 50;
					effect.strength = BoundBy(effect.strength - amount/2, min, 100);
				}
			}
		}
	}
	
	//############## changes to original script start here ###################
	
	// causes on object
	//target->ExecFire(effect_number, caused_by);
	if (target->GetAlive())
	{
		target->DoEnergy(-effect.strength*4, true, FX_Call_EngFire, effect.caused_by); 
	}
	else 
	{
		if ((time*10) % 120 <= effect.strength)
		{
			target->DoDamage(2, FX_Call_DmgFire, effect.caused_by);
			if (target && !Random(2) && !effect.no_burn_decay)
			{
				target->DoCon(-1);
				if (target)
					EffectCall(target, effect, "UpdateEffectProperties");
			}
		} 
	}

	//############## changes to original script end here ###################


	if (!target) 
		return FX_Execute_Kill;
	if (target->Contained()) 
		return FX_OK;

	// Fire particles
	// Fire denses, smoke, chaotic sparks
	if (time % effect.FreqReduction == 0)
	{
		// A few red fire particles popping up in the higher area. Does not affect very small flames.
		if (effect.fire_width + effect.fire_height > 40)
		{
			if (!Random(2)) CreateParticle("FireDense", PV_Random(-effect.fire_width, effect.pspeed), PV_Random(effect.pspeed, effect.fire_height), PV_Random(-3,3), effect.pspeed, effect.fire_height/2+6, effect.redfiredense);
			if (!Random(2)) CreateParticle("FireDense", PV_Random(effect.fire_width/2, effect.fire_width), PV_Random(effect.pspeed, effect.fire_height), PV_Random(-3,3), effect.pspeed, effect.fire_height/2+6, effect.redfiredense);
		}

		// Smoke
		CreateParticle("SmokeThick", PV_Random(-effect.fire_width,effect.fire_width), -effect.fire_height, 0, -6, 300, effect.smoke);

		// Chaotic particles
		if (!Random(3) && effect.FreqReduction < 14)
			CreateParticle("Magic", PV_Random(-effect.fire_width, effect.fire_width), PV_Random(-effect.fire_height, effect.fire_height), PV_Random(25, -25), PV_Random(-25, 12), 50, effect.chaoticspark);
		
		// Flameborders and sharp flames

		// Flame borders
		if (effect.border_active)
		{
			CreateParticle("FireBorder", 0, effect.maxhgt, 0, effect.pspeed, 30, effect.flameborder);
		}
		
		// Sharp flames
		CreateParticle("FireSharp", 0, effect.maxhgt, 0, effect.pspeed, 30, effect.sharpflame);
	}

	return FX_OK;
}
