#include <iso646.h>

#include "Globals.h"
#include "Game.h"
#include "DiceSystem.h"
#include "Player.h"
#include "raylib/raygui.h"

#include <stdio.h>

Player player = { 0 };
DiceSystem sixDice = { 0 };
unsigned gamestate = menu;
bool run = true;


//Forward declarations
void UpdateGame(void);
void DrawGame(void);
void DrawMenu(void);
void UpdateMenu(void);

//Private functions
void UpdateDT(void)
{
    dt = GetFrameTime();
    totalTime = GetTime();
    fps = GetFPS();
}

//Definitions
void Setup(void)
{
    InitWindow(screenWidth, screenHeight, "GMTK2022");
    SetWindowState(FLAG_VSYNC_HINT);

    SetupPlayer(&player);
    

    SetCameraMode(player.camera, CAMERA_FIRST_PERSON);

    sixDice.timer = 0.0f;
    sixDice.rollTime = 10.0f;
    sixDice.lastRoll = 0;
    sixDice.sides = 6;
}

void Run(void)
{
    while (run)
    {
        Update();
        Draw();
    }
}

void Update(void)
{
    UpdateDT();

    switch (gamestate)
    {
    case menu:
        UpdateMenu();
        break;
    case game:
        UpdateGame();
        break;
    case highscore:
        return;
        break;
    default:
        break;
    }
}

void Draw(void)
{
    switch (gamestate)
    {
    case menu:
        DrawMenu();
        break;
    case game:
        DrawGame();
        break;
    case highscore:
        return;
        break;
    default:
        break;
    }
}

void UpdateGame(void)
{
    UpdatePlayer(&player);
    if(IsKeyPressed(KEY_ESCAPE))
        gamestate = menu;
    if (UpdateDiceSystem(&sixDice))
    {
        unsigned roll = RollDice(sixDice.sides);
        sixDice.lastRoll = roll;
    }
}

void UpdateMenu(void)
{
    EnableCursor();
    Rectangle play = {
        .height = 30.f,
        .width = 90.f,
        .x = 740.f,
        .y = 900.f / 2.f
    };

    Rectangle exit = {
        .height = 30.f,
        .width = 90.f,
        .x = 740.f,
        .y = 900.f / 1.8f
    };

    
    if (GuiButton(play, GuiIconText(RICON_DEMON, "Play")))
    {
        gamestate = game;
        DisableCursor();
    }
    if (GuiButton(exit, GuiIconText(RICON_UNDO, "Exit")))
        run = false;
}


void DrawGame(void)
{
    BeginDrawing();

    ClearBackground(RAYWHITE);

    BeginMode3D(player.camera);

    DrawPlayer(&player);

    DrawPlane((Vector3) { 0.0f, 0.0f, 0.0f }, (Vector2) { 32.0f, 32.0f }, LIGHTGRAY); // Draw ground
    DrawCube((Vector3) { -16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, BLUE);     // Draw a blue wall
    DrawCube((Vector3) { 16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, LIME);      // Draw a green wall
    DrawCube((Vector3) { 0.0f, 2.5f, 16.0f }, 32.0f, 5.0f, 1.0f, GOLD);      // Draw a yellow wall

    EndMode3D();

    DrawRectangle(10, 10, 220, 70, Fade(SKYBLUE, 0.5f));
    DrawRectangleLines(10, 10, 220, 70, BLUE);


    char str[2];
    sprintf(str, "%d", sixDice.lastRoll);
    DrawText(str, 10, 50, 30, BLACK);

    char str2[2];
    sprintf(str2, "%d", shots);
    DrawText(str2, 200, 200, 50, BLACK);
    DrawText("First person camera default controls:", 20, 20, 10, BLACK);
    DrawText("- Move with keys: W, A, S, D", 40, 40, 10, DARKGRAY);
    DrawText("- Mouse move to look around", 40, 60, 10, DARKGRAY);

    EndDrawing();
}

void DrawMenu(void)
{
    BeginDrawing();

    ClearBackground(RAYWHITE);


    DrawText("Diece", 650, 200, 100, RED);
    EndDrawing();
}