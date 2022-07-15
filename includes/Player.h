#pragma once
#include "Globals.h"
#include "Gun.h"

typedef struct Player
{
	Camera camera;
	Gun gun;
} Player;

void SetupPlayer(Player* player);
void UpdatePlayer(Player* player);
void DrawPlayer(Player* player);