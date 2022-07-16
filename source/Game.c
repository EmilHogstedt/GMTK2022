#include <stdio.h>

#include "Globals.h"
#include "Game.h"
#include "DiceSystem.h"
#include "Player.h"
#include "Menu.h"

#include "raylib/raymath.h"

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#define MAX_MODELS 20

#define RLIGHTS_IMPLEMENTATION
#include "raylib/rlights.h"

Shader shader = { 0 };
Player player = { 0 };
DiceSystem sixDice = { 0 };

Light* lights = NULL;

Mesh meshes[MAX_MODELS] = { 0 };
Model models[MAX_MODELS] = { 0 };

//Forward declarations
void UpdateGame(void);
void DrawGame(void);
void GenerateLevel(void);

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

    shader = LoadShader(TextFormat("shaders/base_lighting.vs", GLSL_VERSION), TextFormat("shaders/lighting.fs", GLSL_VERSION));
    shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");

    int ambientLoc = GetShaderLocation(shader, "ambient");
    SetShaderValue(shader, ambientLoc, (float[4]) { 0.3f, 0.3f, 0.3f, 1.0f }, SHADER_UNIFORM_VEC4);
    Light temp = CreateLight(LIGHT_POINT, (Vector3) { -2.0f, 10.0f, -2.0f }, Vector3Zero(), WHITE, shader);
    arrpush(lights, temp);

    //Set the shader for all our objects.
    for (unsigned i = 0; i < pistolModel.materialCount; i++)
    {
        pistolModel.materials[i].shader = shader;
    }
    for (unsigned i = 0; i < smgModel.materialCount; i++)
    {
        smgModel.materials[i].shader = shader;
    }
    for (unsigned i = 0; i < shotgunModel.materialCount; i++)
    {
        shotgunModel.materials[i].shader = shader;
    }
    for (unsigned i = 0; i < shotModel.materialCount; i++)
    {
        shotModel.materials[i].shader = shader;
    }
    GenerateLevel();

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


void DrawGame(void)
{
    BeginDrawing();

    ClearBackground(RAYWHITE);

    BeginMode3D(player.camera);

    DrawPlayer(&player);

    DrawModel(models[0], (Vector3){0.0f, 0.0f, 0.0f}, 1.f, DARKGRAY);
    //DrawPlane((Vector3) { 0.0f, 0.0f, 0.0f }, (Vector2) { 32.0f, 32.0f }, LIGHTGRAY); // Draw ground
    // DrawCube((Vector3) { -16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, BLUE);     // Draw a blue wall
    // DrawCube((Vector3) { 16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, LIME);      // Draw a green wall
    // DrawCube((Vector3) { 0.0f, 2.5f, 16.0f }, 32.0f, 5.0f, 1.0f, GOLD);      // Draw a yellow wall

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

void GenerateLevel(void)
{
    meshes[0] = GenMeshCylinder(20.f, 1.f, 10);
    models[0] = LoadModelFromMesh(meshes[0]);
    for (unsigned i = 0; i < models[0].materialCount; i++)
    {
        models[0].materials[i].shader = shader;
    }
}