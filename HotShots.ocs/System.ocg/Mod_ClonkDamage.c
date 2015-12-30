#appendto Clonk


func Damage(int change, int cause, int by_player)
{
	TurnFinishCountdown()->ProlongCountdown();
	GuiPlayerHealthDisplay()->UpdatePlayerDisplays();
	_inherited(change, cause, by_player);
}