#include "Globals.h"

int screenWidth = 1600;
int screenHeight = 900;

float dt = 0.0f;
double totalTime = 0.0;
unsigned fps = 0;

unsigned gamestate = menu;
unsigned innerGamestate = none;
bool run = true;

unsigned shots = 0;

unsigned CurrentScore = 0;
unsigned HighScore = 0;

float MasterVolume = 1.0f;
float MusicVolume = 1.0f;
float SoundEffectVolume = 1.0f;