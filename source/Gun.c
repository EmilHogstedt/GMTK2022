#include "Gun.h"
#include "stdio.h"
#include "raylib/raymath.h"

#include <stdlib.h>

#include "stb_ds.h"

#include "time.h"

#define PistolAmmo 10
#define SMGAmmo 30
#define ShotgunAmmo 5

#define PistolCooldown 1.0f
#define SMGCooldown 0.3f
#define ShotgunCooldown 3.0f

#define PistolPower 20.0f
#define SMGPower 8.0f
#define ShotgunPower 15.0f

#define PistolReload 1.0f
#define SMGReload 2.0f
#define ShotgunReload 2.0f

Model pistolModel = { 0 };
Model smgModel = { 0 };
Model shotgunModel = { 0 };
Model shotModel = { 0 };

Sound pistolShotSounds[6];
Sound smgShotSounds[6];
Sound shotgunShotSounds[6];

Sound pistolCasingSounds[6];
Sound smgCasingSounds[6];
Sound shotgunCasingSounds[6];

Sound reloadSounds[3];
//Private functions

void UpdateShots(Gun* gun, Vector3 playerPos, Enemy* enemies)
{
	for (unsigned i = 0; i < arrlen(gun->shotsInfo); i++)
	{
		gun->shotsInfo[i].shotPos = Vector3Add(gun->shotsInfo[i].shotPos, Vector3Scale(gun->shotsInfo[i].shotVel, dt));
		if (Vector3Length(Vector3Subtract(gun->shotsInfo[i].shotPos, playerPos)) > 100)
		{
			arrdel(gun->shotsInfo, i);
		}
		/* Check if the bullet hits any enemies, then the enemy takes damage and the bullet disappears.
		for (unsigned j = 0; j < arrlen(enemies); j++)
		{

		}
		*/
	}
}

void RandomizeShotSound(void)
{
	srand((unsigned)time(NULL));
	unsigned shotIndex = rand() % 6;
	switch (player.gun.currentGun)
	{
	case Pistol:
	{
		PlaySound(pistolShotSounds[shotIndex]);
		SetSoundVolume(pistolShotSounds[shotIndex], MasterVolume * SoundEffectVolume);
		break;
	}
	case SMG:
	{
		PlaySound(smgShotSounds[shotIndex]);
		SetSoundVolume(smgShotSounds[shotIndex], MasterVolume * SoundEffectVolume);
		break;
	}
	case Shotgun:
	{
		PlaySound(shotgunShotSounds[shotIndex]);
		SetSoundVolume(shotgunShotSounds[shotIndex], MasterVolume * SoundEffectVolume);
		break;
	}
	default:
	{
		break;
	}
	}
}

void RandomizeCasingSound(void)
{
	srand((unsigned)time(NULL));
	unsigned casingIndex = rand() % 6;
	switch (player.gun.currentGun)
	{
	case Pistol:
	{
		PlaySound(pistolCasingSounds[casingIndex]);
		SetSoundVolume(pistolCasingSounds[casingIndex], MasterVolume * SoundEffectVolume);
		break;
	}
	case SMG:
	{
		PlaySound(smgCasingSounds[casingIndex]);
		SetSoundVolume(smgCasingSounds[casingIndex], MasterVolume * SoundEffectVolume);
		break;
	}
	case Shotgun:
	{
		PlaySound(shotgunCasingSounds[casingIndex]);
		SetSoundVolume(shotgunCasingSounds[casingIndex], MasterVolume * SoundEffectVolume);
		break;
	}
	default:
	{
		break;
	}
	}
}

//Definitions
void LoadModelsAndSounds(void)
{
	pistolModel = LoadModel("resources/Models/Pistol/gun.obj");
	smgModel = LoadModel("resources/Models/SMG/Uzi.obj");
	shotgunModel = LoadModel("resources/Models/Shotgun/Rifle.obj");
	shotModel = LoadModelFromMesh(GenMeshSphere(0.035f, 10, 10));

	//Setup sounds
	//Shots
	{
		pistolShotSounds[0] = LoadSound("resources/Sounds/Pistol/shot1.wav");
		pistolShotSounds[1] = LoadSound("resources/Sounds/Pistol/shot2.wav");
		pistolShotSounds[2] = LoadSound("resources/Sounds/Pistol/shot3.wav");
		pistolShotSounds[3] = LoadSound("resources/Sounds/Pistol/shot4.wav");
		pistolShotSounds[4] = LoadSound("resources/Sounds/Pistol/shot5.wav");
		pistolShotSounds[5] = LoadSound("resources/Sounds/Pistol/shot6.wav");

		smgShotSounds[0] = LoadSound("resources/Sounds/SMG/shot1.wav");
		smgShotSounds[1] = LoadSound("resources/Sounds/SMG/shot2.wav");
		smgShotSounds[2] = LoadSound("resources/Sounds/SMG/shot3.wav");
		smgShotSounds[3] = LoadSound("resources/Sounds/SMG/shot4.wav");
		smgShotSounds[4] = LoadSound("resources/Sounds/SMG/shot5.wav");
		smgShotSounds[5] = LoadSound("resources/Sounds/SMG/shot6.wav");

		shotgunShotSounds[0] = LoadSound("resources/Sounds/Shotgun/shot1.wav");
		shotgunShotSounds[1] = LoadSound("resources/Sounds/Shotgun/shot2.wav");
		shotgunShotSounds[2] = LoadSound("resources/Sounds/Shotgun/shot3.wav");
		shotgunShotSounds[3] = LoadSound("resources/Sounds/Shotgun/shot4.wav");
		shotgunShotSounds[4] = LoadSound("resources/Sounds/Shotgun/shot5.wav");
		shotgunShotSounds[5] = LoadSound("resources/Sounds/Shotgun/shot6.wav");
	}

	//Casings
	{
		pistolCasingSounds[0] = LoadSound("resources/Sounds/Pistol/casing1.wav");
		pistolCasingSounds[1] = LoadSound("resources/Sounds/Pistol/casing2.wav");
		pistolCasingSounds[2] = LoadSound("resources/Sounds/Pistol/casing3.wav");
		pistolCasingSounds[3] = LoadSound("resources/Sounds/Pistol/casing4.wav");
		pistolCasingSounds[4] = LoadSound("resources/Sounds/Pistol/casing5.wav");
		pistolCasingSounds[5] = LoadSound("resources/Sounds/Pistol/casing6.wav");

		smgCasingSounds[0] = LoadSound("resources/Sounds/SMG/casing1.wav");
		smgCasingSounds[1] = LoadSound("resources/Sounds/SMG/casing2.wav");
		smgCasingSounds[2] = LoadSound("resources/Sounds/SMG/casing3.wav");
		smgCasingSounds[3] = LoadSound("resources/Sounds/SMG/casing4.wav");
		smgCasingSounds[4] = LoadSound("resources/Sounds/SMG/casing5.wav");
		smgCasingSounds[5] = LoadSound("resources/Sounds/SMG/casing6.wav");

		shotgunCasingSounds[0] = LoadSound("resources/Sounds/Shotgun/casing1.wav");
		shotgunCasingSounds[1] = LoadSound("resources/Sounds/Shotgun/casing2.wav");
		shotgunCasingSounds[2] = LoadSound("resources/Sounds/Shotgun/casing3.wav");
		shotgunCasingSounds[3] = LoadSound("resources/Sounds/Shotgun/casing4.wav");
		shotgunCasingSounds[4] = LoadSound("resources/Sounds/Shotgun/casing5.wav");
		shotgunCasingSounds[5] = LoadSound("resources/Sounds/Shotgun/casing6.wav");
	}

	//Reloads
	reloadSounds[0] = LoadSound("resources/Sounds/Pistol/reload.wav");
	reloadSounds[1] = LoadSound("resources/Sounds/SMG/reload.wav");
	reloadSounds[2] = LoadSound("resources/Sounds/Shotgun/reload.wav");
}

void SetupGun(Gun* gun)
{
	gun->currentGun = Pistol;
	gun->maxAmmo = PistolAmmo;
	gun->shootCD = PistolCooldown;
	gun->power = PistolPower;
	gun->reloadCD = PistolReload;

	srand((unsigned)time(NULL));
	gun->currentAmmo = rand() % gun->maxAmmo + 1;
	gun->shootTimer = 0.0f;
	gun->pos = (Vector3){ 0.0f, 5.0f, 0.0f };
	gun->shotsInfo = NULL;
}

void ChangeGun(Gun* gun, GunType type)
{
	switch (type)
	{
	case Pistol:
	{
		gun->maxAmmo = PistolAmmo;
		gun->shootCD = PistolCooldown;
		gun->power = PistolPower;
		gun->reloadCD = PistolReload;
		break;
	}
	case SMG:
	{
		gun->maxAmmo = SMGAmmo;
		gun->shootCD = SMGCooldown;
		gun->power = SMGPower;
		gun->reloadCD = SMGReload;
		break;
	}
	case Shotgun:
	{
		gun->maxAmmo = ShotgunAmmo;
		gun->shootCD = ShotgunCooldown;
		gun->power = ShotgunPower;
		gun->reloadCD = ShotgunReload;
		break;
	}
	default:
	{
		break;
	}
	}

	gun->currentGun = type;
	srand((unsigned)time(NULL));
	gun->currentAmmo = rand() % gun->maxAmmo + 1;
	gun->shootTimer = 0.0f;
	gun->reloading = true;
	gun->reloadTimer = gun->reloadCD;

	PlaySound(reloadSounds[gun->currentGun]);
	SetSoundVolume(reloadSounds[gun->currentGun], MasterVolume * SoundEffectVolume);
}

void UpdateGun(Gun* gun, Camera playerCamera, Enemy* enemies)
{
	if (gun->reloading)
	{
		gun->reloadTimer -= dt;
		if (gun->reloadTimer < 0.0f)
		{
			gun->reloading = false;
			RandomizeCasingSound();
		}
	}
	Vector3 playerPos = playerCamera.position;
	Vector3 playerTarget = playerCamera.target;
	Vector3 playerUp = playerCamera.up;
	
	Vector3 playerForward = Vector3Normalize(Vector3Subtract(playerTarget, playerPos));
	//printf("Player Target:%f, %f, %f\n", playerTarget.x, playerTarget.y, playerTarget.z);
	Vector3 normalizedUp = Vector3Normalize(playerUp);

	if (gun->shootTimer > 0.0f)
	{
		gun->shootTimer -= dt;
	}

	Matrix s = { 0 };
	Vector2 pitchYaw = { 0 };
	Matrix r = { 0 };
	Matrix t = { 0 };
	Vector3 right = Vector3Normalize(Vector3CrossProduct(normalizedUp, playerForward));
	Vector3 up = Vector3Normalize(Vector3CrossProduct(right, playerForward));
	switch (gun->currentGun)
	{
	case Pistol:
	{
		s = MatrixScale(0.01f, 0.01f, 0.01f);
		pitchYaw = Vector3Angle(playerForward, (Vector3) { pistolModel.transform.m8, pistolModel.transform.m9, pistolModel.transform.m10 });
		
		if (gun->reloading)
		{
			pitchYaw.y -= Lerp(0.0f, PI * 2.0f, 1.0f - (gun->reloadTimer / gun->reloadCD));
		}
		else
		{
			float first = 1.0f / 6.0f;
			if (gun->shootTimer > gun->shootCD * (1.0f - first))
			{
				pitchYaw.y -= Lerp(0, PI / 2.0f, 1.0f - ((gun->shootTimer - (gun->shootCD * (1.0f - first))) / (gun->shootCD * first)));
			}
			else if (gun->shootTimer > 0.0f)
			{
				pitchYaw.y -= Lerp(PI / 2.0f, 0.0f, 1.0f - (gun->shootTimer / (gun->shootCD * (1.0f - first))));
			}
		}
		Matrix rY = MatrixRotateY(-pitchYaw.x + PI / 2.0f);
		Matrix rX = MatrixRotate(right, pitchYaw.y);

		r = MatrixMultiply(rY, rX);

		Vector3 tempPos = Vector3Add(playerPos, Vector3Scale(playerForward, 1.5f));
		tempPos = Vector3Subtract(tempPos, Vector3Scale(right, 1.5f));
		tempPos = Vector3Add(tempPos, Vector3Scale(up, 1.0f));
		t = MatrixTranslate(tempPos.x, tempPos.y, tempPos.z);

		pistolModel.transform = MatrixMultiply(MatrixMultiply(s, r), t);
		gun->pos = (Vector3){ pistolModel.transform.m12, pistolModel.transform.m13, pistolModel.transform.m14 };
		gun->pos = Vector3Subtract(gun->pos, Vector3Scale(up, 0.2f));
		break;
	}
	case SMG:
	{
		s = MatrixScale(1.0f, 1.0f, 1.0f);
		pitchYaw = Vector3Angle(playerForward, (Vector3) { smgModel.transform.m8, smgModel.transform.m9, smgModel.transform.m10 });

		if (gun->reloading)
		{
			float first = 1.0f / 4.0f;
			if (gun->reloadTimer > gun->reloadCD * (1.0f - first))
			{
				pitchYaw.x += Lerp(0.0f, PI / 4.0f, 1.0f - ((gun->reloadTimer - (gun->reloadCD * (1.0f - first))) / (gun->reloadCD * first)));
			}
			else if (gun->reloadTimer > first)
			{
				pitchYaw.x += PI / 4.0f;
			}
			else if (gun->reloadTimer > 0.0f)
			{
				pitchYaw.x += Lerp(PI / 4.0f, 0.0f, 1.0f - ((gun->reloadTimer) / (gun->reloadCD * first)));
			}
		}
		

		Matrix rY = MatrixRotateY(-pitchYaw.x + PI / 16.0f);
		Matrix rX = MatrixRotate(right, pitchYaw.y);

		r = MatrixMultiply(rY, rX);

		Vector3 tempPos = Vector3Add(playerPos, Vector3Scale(playerForward, 1.5f));
		tempPos = Vector3Subtract(tempPos, Vector3Scale(right, 1.5f));
		tempPos = Vector3Add(tempPos, Vector3Scale(up, 1.5f));
		t = MatrixTranslate(tempPos.x, tempPos.y, tempPos.z);
		
		smgModel.transform = MatrixMultiply(MatrixMultiply(s, r), t);
		gun->pos = (Vector3){ smgModel.transform.m12, smgModel.transform.m13, smgModel.transform.m14 };
		gun->pos = Vector3Add(gun->pos, Vector3Scale(playerForward, 1.2f));
		gun->pos = Vector3Subtract(gun->pos, Vector3Scale(up, 0.9f));
		gun->pos = Vector3Subtract(gun->pos, Vector3Scale(right, 0.4f));
		break;
	}
	case Shotgun:
	{
		s = MatrixScale(1.0f, 1.0f, 0.2f);
		pitchYaw = Vector3Angle(playerForward, (Vector3) { shotgunModel.transform.m8, shotgunModel.transform.m9, shotgunModel.transform.m10 });

		if (gun->reloading)
		{
			pitchYaw.x += Lerp(0.0f, PI * 2.0f, 1.0f - (gun->reloadTimer / gun->reloadCD));
		}
		else
		{
			float first = 1.0f / 6.0f;
			if (gun->shootTimer > gun->shootCD * (1.0f - first))
			{
				pitchYaw.y -= Lerp(0, PI / 2.0f, 1.0f - ((gun->shootTimer - (gun->shootCD * (1.0f - first))) / (gun->shootCD * first)));
			}
			else if (gun->shootTimer > 0.0f)
			{
				pitchYaw.y -= Lerp(PI / 2.0f, 0.0f, 1.0f - (gun->shootTimer / (gun->shootCD * (1.0f - first))));
			}
		}
		Matrix rY = MatrixRotateY(-pitchYaw.x + PI / 16.0f);

		Matrix rX = MatrixRotate(right, pitchYaw.y);

		r = MatrixMultiply(rY, rX);

		Vector3 tempPos = Vector3Add(playerPos, Vector3Scale(playerForward, 1.0f));
		tempPos = Vector3Subtract(tempPos, Vector3Scale(right, 1.5f));
		tempPos = Vector3Add(tempPos, Vector3Scale(up, 1.5f));
		t = MatrixTranslate(tempPos.x, tempPos.y, tempPos.z);

		shotgunModel.transform = MatrixMultiply(MatrixMultiply(s, r), t);
		gun->pos = (Vector3){ shotgunModel.transform.m12, shotgunModel.transform.m13, shotgunModel.transform.m14 };
		gun->pos = Vector3Add(gun->pos, Vector3Scale(playerForward, 1.6f));
		gun->pos = Vector3Subtract(gun->pos, Vector3Scale(up, 0.6f));
		gun->pos = Vector3Subtract(gun->pos, Vector3Scale(right, 0.4f));
		break;
	}
	default:
	{
		break;
	}
	}

	UpdateShots(gun, playerPos, enemies);
}

void Shoot(Gun* gun, Camera playerCamera)
{
	if (gun->shootTimer <= 0.0f && !gun->reloading)
	{
		if (gun->currentAmmo == 0)
		{
			gun->reloading = true;
			gun->reloadTimer = gun->reloadCD;
			srand((unsigned)time(NULL));
			gun->currentAmmo = rand() % gun->maxAmmo + 1;
			PlaySound(reloadSounds[gun->currentGun]);
			SetSoundVolume(reloadSounds[gun->currentGun], MasterVolume * SoundEffectVolume);
			return;
		}
		Vector3 playerForward = Vector3Normalize(Vector3Subtract(playerCamera.target, playerCamera.position));

		Vector3 normalizedUp = Vector3Normalize(playerCamera.up);
		Vector3 right = Vector3Normalize(Vector3CrossProduct(normalizedUp, playerForward));
		Vector3 up = Vector3Normalize(Vector3CrossProduct(right, playerForward));

		gun->shootTimer = gun->shootCD;
		if (gun->currentGun == Shotgun)
		{
			srand((unsigned)time(NULL));
			unsigned newShots = rand() % 6 + 4;
			for (unsigned i = 0; i < newShots; i++)
			{
				ShotInfo info = { 0 };
				info.shotPos = gun->pos;

				Vector3 newTarget = (Vector3){ playerCamera.target.x + ((rand() % 1000) / 2000.0f) - 0.25f, playerCamera.target.y + ((rand() % 1000) / 2000.0f) - 0.25f, playerCamera.target.z + ((rand() % 1000) / 2000.0f) - 0.25f};
				newTarget = Vector3Subtract(newTarget, Vector3Scale(up, 1.3f));
				newTarget = Vector3Add(newTarget, Vector3Scale(right, 0.40f));
				Vector3 newDir = Vector3Normalize(Vector3Subtract(newTarget, playerCamera.position));
				info.shotVel = Vector3Scale(newDir, gun->power);
				float gravity = -2.0f;
				info.shotVel.y += gravity;
				arrpush(gun->shotsInfo, info);
			}
		}
		else
		{
			ShotInfo info = { 0 };
			info.shotPos = gun->pos;
			if (gun->currentGun == Pistol)
			{
				playerForward = Vector3Subtract(playerForward, Vector3Scale(up, 0.175f));
			}
			else if (gun->currentGun == SMG)
			{
				playerForward = Vector3Subtract(playerForward, Vector3Scale(up, 0.275f));
			}
			
			playerForward = Vector3Add(playerForward, Vector3Scale(right, 0.15f));
			info.shotVel = Vector3Scale(playerForward, gun->power);
			float gravity = -2.0f;
			info.shotVel.y += gravity;
			arrpush(gun->shotsInfo, info);
		}
		gun->currentAmmo -= 1;
		RandomizeShotSound();
	}
}

void DrawGun(Gun* gun)
{
	switch (gun->currentGun)
	{
	case Pistol:
	{
		DrawModel(pistolModel, (Vector3) { 0.0f, 0.0f, 0.0f }, 1.0f, GRAY);
		break;
	}
	case SMG:
	{
		DrawModel(smgModel, (Vector3) { 0.0f, 0.0f, 0.0f }, 1.0f, GRAY);
		break;
	}
	case Shotgun:
	{
		DrawModel(shotgunModel, (Vector3) { 0.0f, 0.0f, 0.0f }, 1.0f, GRAY);
		break;
	}
	default:
	{
		break;
	}
	}

	//Draw shots
	for (unsigned i = 0; i < arrlen(gun->shotsInfo); i++)
	{
		DrawModel(shotModel, gun->shotsInfo[i].shotPos, 1.0f, RED);
	}
}