#include "Globals.h"
#include "Player.h"
#include "raylib/raymath.h"

#define CAMERA_IMPLEMENTATION
#include "raylib/rcamera.h"

BoundingBox hitbox = { 0 };
Vector3 gravity = {0.0f, -9.82f, 0.0f};

void LoadGunModels(void)
{
	LoadModelsAndSounds();
}

void SetupPlayer()
{
	player.camera.position = (Vector3){ 0.0f, 10.0f, 0.0f };
	player.camera.target = (Vector3){ 0.0f, 0.0f, 1.0f };
	player.camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
	player.camera.fovy = 90.0f;
	player.camera.projection = CAMERA_PERSPECTIVE;

	hitbox.min = (Vector3){ -0.5f, -4.0f, -0.5f };
	hitbox.max = (Vector3){ 0.5f, 1.0f, 0.5f };
	Matrix ppos = MatrixTranslate(player.camera.position.x, player.camera.position.y, player.camera.position.z);
	hitbox.min = Vector3Transform(hitbox.min, ppos);
	hitbox.max = Vector3Transform(hitbox.max, ppos);

	SetupGun(&player.gun);
}

void UpdatePlayer(Enemy* enemies)
{
	UpdateCamera(&player.camera);
	//Matrix temp = GetCameraMatrix(player.camera);
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

	Vector3 oldPos = player.camera.position;
	player.camera.position = Vector3Add(player.camera.position, Vector3Scale(gravity, dt));
	//Matrix temp = GetCameraMatrix(player->camera);
	//player->camera.target = (Vector3){ temp.m8, temp.m9, temp.m10 };

	//Physics
	//Update hitbox position.
	hitbox.min = (Vector3){ -0.5f, -4.0f, -0.5f };
	hitbox.max = (Vector3){ 0.5f, 1.0f, 0.5f };
	Matrix ppos = MatrixTranslate(player.camera.position.x, player.camera.position.y, player.camera.position.z);
	hitbox.min = Vector3Transform(hitbox.min, ppos);
	hitbox.max = Vector3Transform(hitbox.max, ppos);

	//Collissions
	if (CheckCollisionBoxes(hitbox, mapElementsHitBox[0]))
		player.camera.position.y = mapElementsHitBox[0].max.y + 4.0f + 0.01f;

	

	UpdateGun(&player.gun, player.camera, enemies);
	//Input handling
	//If left mouse button is pressed, or if left mouse button is down and player is holding an SMG
	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
		Shoot(&player.gun, player.camera);
}

void DrawPlayer()
{
	DrawGun(&player.gun);
	DrawBoundingBox(hitbox, GREEN);
}