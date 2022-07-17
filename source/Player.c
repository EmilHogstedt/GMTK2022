#include "Globals.h"
#include "Player.h"
#include "raylib/raymath.h"

#include "raylib/physac.h"

BoundingBox hitbox = { 0 };
Vector3 gravity = {0.0f, -9.82f, 0.0f};
PhysicsBody pphys = { 0 };

void LoadGunModels(void)
{
	LoadModelsAndSounds();
}

void SetupPlayer()
{
	player.camera.position = (Vector3){ 0.0f, 40.0f, 0.0f };
	player.camera.target = (Vector3){ 0.0f, 0.0f, 1.0f };
	player.camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
	player.camera.fovy = 90.0f;
	player.camera.projection = CAMERA_PERSPECTIVE;

	SetupGun(&player.gun);

	//Setup hitbox
	hitbox.min = (Vector3){-1.0f, -2.0f, -1.0f};
	hitbox.max = (Vector3){1.0f, 2.0f, 1.0f};
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

	player.camera.position = Vector3Add(player.camera.position, Vector3Scale(gravity, dt));
	//Matrix temp = GetCameraMatrix(player->camera);
	//player->camera.target = (Vector3){ temp.m8, temp.m9, temp.m10 };

	//Physics


	//Collissions
	if(CheckCollisionBoxes(hitbox, mapElementsHitBox[0]))
		player.camera.position.y = 4.0f;

	//Update hitbox position.
	hitbox.min = (Vector3){-0.5f, -1.0f, -0.5f};
	hitbox.max = (Vector3){0.5f, 0.5f, 0.5f};
	Matrix ppos = MatrixTranslate(player.camera.position.x, player.camera.position.y, player.camera.position.z);
	hitbox.min = Vector3Transform(hitbox.min, ppos);
	hitbox.max = Vector3Transform(hitbox.max, ppos);


	// hitbox.min = Vector3Add(hitbox.min, player->camera.position);
	// hitbox.max = Vector3Add(hitbox.max, player->camera.position);

	

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