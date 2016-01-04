/**
 Adds the option of a fake throw that behaves like a real throw to
 the shooter.
 */

#appendto Firestone
#appendto Lantern

func ForceThrow(object shooter, object target, int angle, throw_speed, int throw_time)
{
	if (angle < 180) shooter->SetDir(DIR_Right);
	else shooter->SetDir(DIR_Left);

	var orig_speed = shooter.ThrowSpeed;
	shooter.ThrowSpeed = throw_speed;
	shooter->DoThrow(this, angle);
	shooter.ThrowSpeed = orig_speed;
	var animation = "ThrowArms";
	shooter->PlayAnimation(animation, CLONK_ANIM_SLOT_Arms, Anim_Linear(0, 0, shooter->GetAnimationLength(animation), throw_time), Anim_Const(1000));

}
