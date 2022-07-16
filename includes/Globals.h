#pragma once
#include <stdbool.h>
#include "raylib/raylib.h"

#define GLSL_VERSION 330

#define MAX_COLUMNS 20

typedef enum gamestates
{
   menu,
   game,
   highscore
}gamestates;

typedef struct ShotInfo
{
	Vector3 shotPos;
	Vector3 shotVel;
} ShotInfo;

typedef enum GunType {
	Pistol = 0,
	SMG,
	Shotgun
} GunType;

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
	bool reloading;
	float reloadCD;
	float reloadTimer;
} Gun;

typedef struct Player
{
	Camera camera;
	Gun gun;
} Player;

extern const int screenWidth;
extern const int screenHeight;

extern float dt;
extern double totalTime;
extern unsigned fps;

extern unsigned gamestate;
extern bool run;

extern Player player;