#appendto Clonk


func Damage(int change, int cause, int by_player)
{
	TurnFinishCountdown()->ProlongCountdown();
	_inherited(change, cause, by_player);
}