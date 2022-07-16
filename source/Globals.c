#include "Globals.h"

const int screenWidth = 1600;
const int screenHeight = 900;

float dt = 0.0f;
double totalTime = 0.0;
unsigned fps = 0;

unsigned gamestate = menu;
bool run = true;

unsigned shots = 0;