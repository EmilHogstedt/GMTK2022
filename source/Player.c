#include "Player.h"

void SetupPlayer(Player* player)
{
	player->camera.position = (Vector3){ 0.0f, 0.0f, 0.0f };
	player->camera.target = (Vector3){ 0.0f, 0.0f, 1.0f };
	player->camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
	player->camera.fovy = 90.0f;
	player->camera.projection = CAMERA_PERSPECTIVE;

	SetupGun(&player->gun);
}

void UpdatePlayer(Player* player)
{
	UpdateCamera(&player->camera);
	Matrix temp = GetCameraMatrix(player->camera);
	//player->camera.target = (Vector3){ temp.m8, temp.m9, temp.m10 };

	UpdateGun(&player->gun, player->camera);
	//Input handling
	//If left mouse button is pressed, or if left mouse button is down and player is holding an SMG
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && player->gun.currentGun == SMG))
	{
		Shoot(&player->gun);
	}
}

void DrawPlayer(Player* player)
{
	DrawGun(&player->gun);
}