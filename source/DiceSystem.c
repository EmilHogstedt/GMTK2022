#include "DiceSystem.h"

//Definitions

bool UpdateDiceSystem(DiceSystem* diceSystem)
{
	if (diceSystem->sides == 0)
	{
		return false;
	}

	diceSystem->timer += dt;

	if (diceSystem->timer > diceSystem->rollTime)
	{
		return true;
	}
	else
	{
		return false;
	}
}

unsigned RollDice(unsigned sides)
{
	return sides;
}