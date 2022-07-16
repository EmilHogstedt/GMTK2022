#pragma once
#include "Globals.h"
#include "Enemies.h"

void SetupGun(Gun* gun);
void ChangeGun(Gun* gun, GunType type);
void UpdateGun(Gun* gun, Camera playerCamera, Enemy* enemies);
void Shoot(Gun* gun, Camera playerCamera);
void DrawGun(Gun* gun);

Model pistolModel;
Model smgModel;
Model shotgunModel;
Model shotModel;