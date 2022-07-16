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

extern const int screenWidth;
extern const int screenHeight;

extern float dt;
extern double totalTime;
extern unsigned fps;

extern unsigned gamestate;
extern bool run;

extern unsigned shots;