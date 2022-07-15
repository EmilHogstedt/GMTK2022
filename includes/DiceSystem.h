#include "Globals.h"

typedef struct DiceSystem
{
	float timer;
	float rollTime;
	unsigned sides;
	unsigned lastRoll;

} DiceSystem;

bool UpdateDiceSystem(DiceSystem* diceSystem);
unsigned RollDice(unsigned sides);