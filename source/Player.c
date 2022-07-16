#include "Player.h"

void SetupPlayer()
{
	player.camera.position = (Vector3){ 0.0f, 3.0f, 0.0f };
	player.camera.target = (Vector3){ 0.0f, 0.0f, 1.0f };
	player.camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
	player.camera.fovy = 90.0f;
	player.camera.projection = CAMERA_PERSPECTIVE;

	SetupGun(&player.gun);
}

void UpdatePlayer(Enemy* enemies)
{
	UpdateCamera(&player.camera);
	Matrix temp = GetCameraMatrix(player.camera);
	//player->camera.target = (Vector3){ temp.m8, temp.m9, temp.m10 };

	UpdateGun(&player.gun, player.camera, enemies);
	//Input handling
	//If left mouse button is pressed, or if left mouse button is down and player is holding an SMG
	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
	{
		Shoot(&player.gun, player.camera);
	}
	if (IsKeyPressed(KEY_ONE) && player.gun.currentGun != Pistol)
	{
		ChangeGun(&player.gun, Pistol);
	}
	else if (IsKeyPressed(KEY_TWO) && player.gun.currentGun != SMG)
	{
		ChangeGun(&player.gun, SMG);
	}
	else if (IsKeyPressed(KEY_THREE) && player.gun.currentGun != Shotgun)
	{
		ChangeGun(&player.gun, Shotgun);
	}
}

void DrawPlayer()
{
	DrawGun(&player.gun);
}