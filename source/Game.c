#include "Globals.h"
#include "Game.h"
#include "DiceSystem.h"

Camera camera = { 0 };
DiceSystem sixDice = { 0 };

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

    camera.position = (Vector3){ 4.0f, 2.0f, 4.0f };
    camera.target = (Vector3){ 0.0f, 1.8f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 90.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    SetCameraMode(camera, CAMERA_FIRST_PERSON);

    sixDice.timer = 0.0f;
    sixDice.rollTime = 0.0f;
    sixDice.lastRoll = 0;
    sixDice.sides = 6;
}

void Run(void)
{
    while (!WindowShouldClose())
    {
        Update();
        Draw();
    }
}

void Update(void)
{
    UpdateDT();

    UpdateCamera(&camera);
    if (UpdateDiceSystem(&sixDice))
    {
        unsigned roll = RollDice(sixDice.sides);
        sixDice.lastRoll = roll;
    }
}

void Draw(void)
{
    BeginDrawing();

    ClearBackground(RAYWHITE);

    BeginMode3D(camera);

    DrawPlane((Vector3) { 0.0f, 0.0f, 0.0f }, (Vector2) { 32.0f, 32.0f }, LIGHTGRAY); // Draw ground
    DrawCube((Vector3) { -16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, BLUE);     // Draw a blue wall
    DrawCube((Vector3) { 16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, LIME);      // Draw a green wall
    DrawCube((Vector3) { 0.0f, 2.5f, 16.0f }, 32.0f, 5.0f, 1.0f, GOLD);      // Draw a yellow wall

    EndMode3D();

    DrawRectangle(10, 10, 220, 70, Fade(SKYBLUE, 0.5f));
    DrawRectangleLines(10, 10, 220, 70, BLUE);

    DrawText("First person camera default controls:", 20, 20, 10, BLACK);
    DrawText("- Move with keys: W, A, S, D", 40, 40, 10, DARKGRAY);
    DrawText("- Mouse move to look around", 40, 60, 10, DARKGRAY);

    EndDrawing();
}