#pragma once
#include <stdbool.h>

#include "raylib/raylib.h"
#define GLSL_VERSION 330

#define MAX_COLUMNS 20
#define MAX_MODELS 20

typedef enum gamestates
{
   none = 0,
   menu,
   game,
   pause,
   settings,
   credits
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

extern int screenWidth;
extern int screenHeight;

extern float dt;
extern double totalTime;
extern unsigned fps;

extern unsigned gamestate;
extern unsigned innerGamestate;

extern bool run;

extern Player player;

extern unsigned shots;

extern BoundingBox mapElementsHitBox[MAX_MODELS];

extern unsigned CurrentScore;
extern unsigned HighScore;

extern float MasterVolume;
extern float MusicVolume;
extern float SoundEffectVolume;