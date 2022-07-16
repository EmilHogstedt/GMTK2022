#include <stdio.h>

#include "Globals.h"
#include "Game.h"
#include "DiceSystem.h"
#include "Player.h"
#include "Menu.h"
#include "Enemies.h"

#include "raylib/raymath.h"

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"



#define RLIGHTS_IMPLEMENTATION
#include "raylib/rlights.h"

Player player = { 0 };
Shader shader = { 0 };
DiceSystem sixDice = { 0 };

Light* lights = NULL;

Enemy* enemies = NULL;

Mesh meshes[MAX_MODELS] = { 0 };
Model models[MAX_MODELS] = { 0 };
BoundingBox mapElementsHitBox[MAX_MODELS] = { 0 };

Music ingameMusic = { 0 };

//Forward declarations
void UpdateGame(void);
void DrawGame(void);
void GenerateLevel(void);
void SetupPlayerAndGuns(void);
void SetupEnemyModels(void);

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

    SetupPlayer();

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
    //Guns
    SetupPlayerAndGuns();
    
    //Enemies
    SetupEnemyModels();

    //Level
    GenerateLevel();


    //TEMP
    Enemy temp2 = CreateEnemy(Skull, (Vector3) { 40.0f, 20.0f, 1.0f }, player.camera.position, (Vector3){ 1.0f, 1.0f, 1.0f}, 10);
    arrpush(enemies, temp2);

    ingameMusic = LoadMusicStream("resources/Sounds/Music/Wrath.mp3");
    PlayMusicStream(ingameMusic);
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
    if (!IsMusicStreamPlaying(ingameMusic))
    {
        PlayMusicStream(ingameMusic);
    }
    UpdatePlayer(enemies);
    if (IsKeyPressed(KEY_ESCAPE))
    {
        gamestate = menu;
        StopMusicStream(ingameMusic);
    }
    if (UpdateDiceSystem(&sixDice))
    {
        unsigned roll = RollDice(sixDice.sides);
        sixDice.lastRoll = roll;
    }

    for (unsigned i = 0; i < arrlen(enemies); i++)
    {
        UpdateEnemy(&enemies[i]);
    }
}


void DrawGame(void)
{
    BeginDrawing();

    ClearBackground(RAYWHITE);

    BeginMode3D(player.camera);

    DrawPlayer();

    DrawModel(models[0], (Vector3){0.0f, 0.0f, 0.0f}, 1.f, DARKGRAY);

    for (unsigned i = 0; i < arrlen(enemies); i++)
    {
        DrawEnemy(&enemies[i]);
    }

    DrawBoundingBox(mapElementsHitBox[0], GREEN);  
    EndMode3D();

    char str[2];
    sprintf(str, "%d", sixDice.lastRoll);
    DrawText(str, 300, 50, 30, BLACK);

    char str2[10];
    sprintf(str2, "%d / %d", player.gun.currentAmmo, player.gun.maxAmmo);
    DrawText(str2, 200, 200, 50, BLACK);

    float lineLength = 20.0f;
    DrawLineEx((Vector2) { screenWidth / 2.0f, screenHeight / 2.0f - lineLength / 2.0f }, (Vector2) { screenWidth / 2.0f, screenHeight / 2.0f + lineLength / 2.0f }, 3.0f, WHITE);
    DrawLineEx((Vector2) { screenWidth / 2.0f - lineLength / 2.0f, screenHeight / 2.0f }, (Vector2) { screenWidth / 2.0f + lineLength / 2.0f, screenHeight / 2.0f }, 3.0f, WHITE);
    EndDrawing();
}

void SetupPlayerAndGuns(void)
{
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
}

void SetupEnemyModels(void)
{
    SetupEnemies();
    for (unsigned i = 0; i < skullModel.materialCount; i++)
    {
        skullModel.materials[i].shader = shader;
    }
}

void GenerateLevel(void)
{
    meshes[0] = GenMeshCylinder(20.f, 1.f, 10);
    models[0] = LoadModelFromMesh(meshes[0]);
    mapElementsHitBox[0] = GetModelBoundingBox(models[0]);
    for (unsigned i = 0; i < models[0].materialCount; i++)
    {
        models[0].materials[i].shader = shader;
    }
}