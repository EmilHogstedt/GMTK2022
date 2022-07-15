#include "Globals.h"
#include "Gun.h"
#include "stdio.h"
#define PistolAmmo 10
#define SMGAmmo 30
#define ShotgunAmmo 5

#define PistolCooldown 1.0f
#define SMGCooldown 0.1f
#define ShotgunCooldown 3.0f

typedef struct PistolModel
{
	Mesh Holder;
	Mesh Barrel;
} PistolModel;

typedef struct SMGModel
{
	Mesh Holder;
	Mesh Barrel;
	Mesh Magazine;
} SMGModel;

typedef struct ShotgunModel
{
	Mesh Holder;
	Mesh Barrel;
} ShotgunModel;

PistolModel pistolModel = { 0 };
SMGModel smgModel = { 0 };
ShotgunModel shotgunModel = { 0 };

void SetupGun(Gun* gun)
{
	gun->currentGun = Pistol;
	gun->maxAmmo = PistolAmmo;
	gun->currentAmmo = gun->maxAmmo;
	gun->shootCD = PistolCooldown;
	gun->shootTimer = 0.0f;
	gun->matrix = (Matrix){ 0 };
}

void ChangeGun(Gun* gun, GunType type)
{
	switch (type)
	{
	case Pistol:
	{
		gun->maxAmmo = PistolAmmo;
		gun->shootCD = PistolCooldown;
		break;
	}
	case SMG:
	{
		gun->maxAmmo = SMGAmmo;
		gun->shootCD = SMGCooldown;
		break;
	}
	case Shotgun:
	{
		gun->maxAmmo = ShotgunAmmo;
		gun->shootCD = ShotgunCooldown;
		break;
	}
	default:
	{
		break;
	}
	}

	gun->currentGun = type;
	gun->currentAmmo = gun->maxAmmo;
	gun->shootTimer = 0.0f;
}

void UpdateGun(Gun* gun, Vector3 playerPos, Vector3 playerForward)
{
	if (gun->shootTimer > 0.0f)
	{
		gun->shootTimer -= dt;
	}

}

void Shoot(Gun* gun)
{
	if (gun->shootTimer <= 0.0f)
	{
		gun->shootTimer = gun->shootCD;
		if (gun->currentGun == Shotgun)
		{
			return;
			//Shot a rain of shots
		}
		else
		{
			shots += 1;
			return;
			//Shoot 1 shot.
		}
	}
}

void DrawGun(Gun* gun)
{
	switch (gun->currentGun)
	{
	case Pistol:
	{
		
		break;
	}
	case SMG:
	{
		break;
	}
	case Shotgun:
	{
		break;
	}
	default:
	{
		break;
	}
	}
}