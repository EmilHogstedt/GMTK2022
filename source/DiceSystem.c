#include "DiceSystem.h"
#include <stdlib.h>
#include "time.h"
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
		diceSystem->timer = 0.0f;
		return true;
	}
	else
	{
		return false;
	}
}

unsigned RollDice(unsigned sides)
{
	srand(time(NULL));
	return (rand() % sides) + 1;
}