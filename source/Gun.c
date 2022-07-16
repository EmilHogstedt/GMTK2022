#include "Globals.h"
#include "Gun.h"
#include "stdio.h"
#include "raylib/raymath.h"

#define PistolAmmo 10
#define SMGAmmo 30
#define ShotgunAmmo 5

#define PistolCooldown 1.0f
#define SMGCooldown 0.1f
#define ShotgunCooldown 3.0f

Model pistolModel = { 0 };
Model smgModel = { 0 };
Model shotgunModel = { 0 };

void SetupGun(Gun* gun)
{
	gun->currentGun = Shotgun;
	gun->maxAmmo = PistolAmmo;
	gun->currentAmmo = gun->maxAmmo;
	gun->shootCD = PistolCooldown;
	gun->shootTimer = 0.0f;
	gun->pos = (Vector3){ 0.0f, 5.0f, 0.0f };
	gun->rot = 0.0f;

	pistolModel = LoadModel("resources/Models/Pistol/gun.obj");
	smgModel = LoadModel("resources/Models/SMG/Uzi.obj");
	shotgunModel = LoadModel("resources/Models/Shotgun/Rifle.obj");
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

void UpdateGun(Gun* gun, Camera playerCamera)
{
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
	switch (gun->currentGun)
	{
	case Pistol:
	{
		s = MatrixScale(0.01f, 0.01f, 0.01f);
		pitchYaw = Vector3Angle(playerForward, (Vector3) { pistolModel.transform.m8, pistolModel.transform.m9, pistolModel.transform.m10 });
		r = MatrixRotateXYZ(
			(Vector3) {
			pitchYaw.y * abs(playerForward.z), -pitchYaw.x + PI / 2.0f, pitchYaw.y * abs(playerForward.x)
		}
		);
		Vector3 tempPos = Vector3Add(playerPos, (Vector3) { playerForward.x - (1.5 * playerForward.z), -0.5f, playerForward.z + (1.5 * playerForward.x) });
		t = MatrixTranslate(tempPos.x, tempPos.y, tempPos.z);


		pistolModel.transform = MatrixMultiply(MatrixMultiply(s, r), t);
		break;
	}
	case SMG:
	{
		s = MatrixScale(1.0f, 1.0f, 1.0f);
		pitchYaw = Vector3Angle(playerForward, (Vector3) { smgModel.transform.m8, smgModel.transform.m9, smgModel.transform.m10 });
		r = MatrixRotateXYZ(
			(Vector3) {
			pitchYaw.y * abs(playerForward.z), -pitchYaw.x, pitchYaw.y * abs(playerForward.x)
		}
		);
		Vector3 tempPos = Vector3Add(playerPos, (Vector3) { playerForward.x - (1.5 * playerForward.z), -2.0f, playerForward.z + (1.5 * playerForward.x) });
		t = MatrixTranslate(tempPos.x, tempPos.y, tempPos.z);

		smgModel.transform = MatrixMultiply(MatrixMultiply(s, r), t);
		break;
	}
	case Shotgun:
	{
		s = MatrixScale(0.5f, 0.5f, 0.2f);
		pitchYaw = Vector3Angle(playerForward, (Vector3) { shotgunModel.transform.m8, shotgunModel.transform.m9, shotgunModel.transform.m10 });
		r = MatrixRotateXYZ(
			(Vector3) {
			pitchYaw.y* abs(playerForward.z), -pitchYaw.x, pitchYaw.y * abs(playerForward.x)
		}
		);
		Vector3 tempPos = Vector3Add(playerPos, (Vector3) {playerForward.x - (0.5f * playerForward.z), -0.5f, playerForward.z + (0.5f * playerForward.x) });
		tempPos.x -= 1.2f * playerForward.x;
		tempPos.z -= 1.2f * playerForward.z;
		t = MatrixTranslate(tempPos.x, tempPos.y, tempPos.z);
		shotgunModel.transform = MatrixMultiply(MatrixMultiply(s, r), t);
		break;
	}
	default:
	{
		break;
	}
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
		//DrawModelEx(pistolModel, gun->pos, (Vector3) { 0.0f, 1.0f, 0.0f }, gun->rot, (Vector3) { 0.01f, 0.01f, 0.01f }, RED);
		DrawModel(pistolModel, (Vector3) { 0.0f, 0.0f, 0.0f }, 1.0f, WHITE);
		break;
	}
	case SMG:
	{
		DrawModel(smgModel, (Vector3) { 0.0f, 0.0f, 0.0f }, 1.0f, WHITE);
		break;
	}
	case Shotgun:
	{
		DrawModel(shotgunModel, (Vector3) { 0.0f, 0.0f, 0.0f }, 1.0f, WHITE);
		break;
	}
	default:
	{
		break;
	}
	}
}