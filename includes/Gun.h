#pragma once

typedef enum GunType{
	Pistol = 0,
	SMG,
	Shotgun
} GunType;

typedef struct ShotInfo
{
	Vector3 shotPos;
	Vector3 shotVel;
} ShotInfo;

typedef struct Gun
{
	GunType currentGun;
	unsigned currentAmmo;
	unsigned maxAmmo;
	float shootCD;
	float shootTimer;
	Vector3 pos;
	float power;
	ShotInfo* shotsInfo;
} Gun;

void SetupGun(Gun* gun);
void ChangeGun(Gun* gun, GunType type);
void UpdateGun(Gun* gun, Camera playerCamera);
void Shoot(Gun* gun, Camera playerCamera);
void DrawGun(Gun* gun);

Model pistolModel;
Model smgModel;
Model shotgunModel;
Model shotModel;