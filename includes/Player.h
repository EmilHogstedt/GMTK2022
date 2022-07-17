#pragma once
#include "Gun.h"
#include "Enemies.h"

void LoadGunModels(void);
void SetupPlayer(void);
void UpdatePlayer(Enemy* enemies);
void DrawPlayer(void);