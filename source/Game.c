#include <stdio.h>

#include "Globals.h"
#include "Game.h"
#include "DiceSystem.h"
#include "Player.h"
#include "Menu.h"
#include "Enemies.h"
#include "time.h"

#include "raylib/raymath.h"

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#define RLIGHTS_IMPLEMENTATION
#include "raylib/rlights.h"

#define PHYSAC_IMPLEMENTATION
#include "raylib/physac.h"

Player player = { 0 };
Shader shader = { 0 };
DiceSystem sixDice = { 0 };
DiceSystem weaponCoin = { 0 };

Light* lights = NULL;

Enemy* enemies = NULL;

Mesh meshes[MAX_MODELS] = { 0 };
Model models[MAX_MODELS] = { 0 };
BoundingBox mapElementsHitBox[MAX_MODELS] = { 0 };

Music ingameMusic = { 0 };

float currentScoreTimer = 0.0f;

//Forward declarations
void UpdateGame(void);
void DrawGame(void);
void GenerateLevel(void);
void SetupPlayerAndGuns(void);
void SetupEnemyModels(void);
void SetupDice(void);

//Private functions
void UpdateDT(void)
{
    dt = GetFrameTime();
    totalTime = GetTime();
    fps = GetFPS();
}

void RandomizeGameSong(void)
{
    srand(time(NULL));
    unsigned songIndex = rand() % 3;
    if (songIndex == 1)
    {
        ingameMusic = LoadMusicStream("resources/Sounds/Music/Wrath.mp3");
    }
    else if (songIndex == 2)
    {
        ingameMusic = LoadMusicStream("resources/Sounds/Music/Valhalla.mp3");
    }
    else
    {
        ingameMusic = LoadMusicStream("resources/Sounds/Music/Armageddon.mp3");
    }
    PlayMusicStream(ingameMusic);
    SetMusicVolume(ingameMusic, 0.7f);
}
//Definitions
void Setup(void)
{
    InitWindow(screenWidth, screenHeight, "GMTK2022");

    InitAudioDevice();
    SetWindowState(FLAG_VSYNC_HINT);

    SetupPlayer();

    InitPhysics();

    SetCameraMode(player.camera, CAMERA_FIRST_PERSON);

    SetupDice();

    shader = LoadShader(TextFormat("shaders/base_lighting.vs", GLSL_VERSION), TextFormat("shaders/lighting.fs", GLSL_VERSION));
    shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");

    int ambientLoc = GetShaderLocation(shader, "ambient");
    SetShaderValue(shader, ambientLoc, (float[4]) { 0.3f, 0.3f, 0.3f, 1.0f }, SHADER_UNIFORM_VEC4);
    Light temp = CreateLight(LIGHT_POINT, (Vector3) { -2.0f, 40.0f, -2.0f }, Vector3Zero(), WHITE, shader);
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

    //See if a highscore exists. If it does we load it.
    FILE* highscoreFile;
    if (highscoreFile = fopen("bin/highscore.txt", "r"))
    {
        char buf[100];
        fgets(buf, 100, highscoreFile);
        HighScore = strtol(buf, NULL, 10);
        fclose(highscoreFile);
    }
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
        RandomizeGameSong();
    }
    UpdateMusicStream(ingameMusic);

    currentScoreTimer += dt;
    if (currentScoreTimer > 1.0f)
    {
        currentScoreTimer = 0.0f;
        CurrentScore += 1;
    }

    UpdatePlayer(enemies);
    if(IsKeyPressed(KEY_ESCAPE))
    {
        EnableCursor();
        gamestate = menu;
        StopMusicStream(ingameMusic);

        if (CurrentScore > HighScore)
        {
            HighScore = CurrentScore;

            FILE* highscoreFile;
            highscoreFile = fopen("bin/highscore.txt", "w");
            char buf[100];
            sprintf(buf, "%d", HighScore);
            fputs(buf, highscoreFile);
            fclose(highscoreFile);
        }
    }
    if (UpdateDiceSystem(&sixDice))
    {
        unsigned roll = RollDice(sixDice.sides);
        sixDice.lastRoll = roll;
    }
    if (UpdateDiceSystem(&weaponCoin))
    {
        unsigned roll = RollDice(weaponCoin.sides);
        weaponCoin.lastRoll = roll;
        ChangeGun(&player.gun, (player.gun.currentGun + roll) % 3);
    }

    unsigned len = arrlen(enemies);
    for (unsigned i = 0; i < len; i++)
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

    char ammoStr[10];
    sprintf(ammoStr, "%d / %d", player.gun.currentAmmo, player.gun.maxAmmo);
    DrawText(ammoStr, 200, 200, 50, BLACK);

    switch (player.gun.currentGun)
    {
    case Pistol:
    {
        DrawText("Current Weapon: Pistol", screenWidth / 2 - screenWidth / 8, screenHeight - 30, 30, BLACK);
        break;
    }
    case SMG:
    {
        DrawText("Current Weapon: SMG", screenWidth / 2 - screenWidth / 10, screenHeight - 30, 30, BLACK);
        break;
    }
    case Shotgun:
    {
        DrawText("Current Weapon: Shotgun", screenWidth / 2 - screenWidth / 10, screenHeight - 30, 30, BLACK);
        break;
    }
    default:
    {
        break;
    }
    }

    char weaponSwitchStr[10];
    sprintf(weaponSwitchStr, "%.2f", weaponCoin.rollTime - weaponCoin.timer);
    DrawText(weaponSwitchStr, screenWidth / 2 - screenWidth / 20, screenHeight - 90, 50, BLACK);

    //Crosshair
    float lineLength = 20.0f;
    DrawLineEx((Vector2) { screenWidth / 2.0f, screenHeight / 2.0f - lineLength / 2.0f }, (Vector2) { screenWidth / 2.0f, screenHeight / 2.0f + lineLength / 2.0f }, 3.0f, WHITE);
    DrawLineEx((Vector2) { screenWidth / 2.0f - lineLength / 2.0f, screenHeight / 2.0f }, (Vector2) { screenWidth / 2.0f + lineLength / 2.0f, screenHeight / 2.0f }, 3.0f, WHITE);
    EndDrawing();

    //Current points.
    DrawText("CURRENT POINTS", screenWidth / 2, screenHeight / 6, 60, ORANGE);
    char scoreBuf[100];
    sprintf(scoreBuf, "%d", CurrentScore);
    DrawText(scoreBuf, screenWidth / 2, screenHeight / 4, 50, BLUE);
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

void SetupDice(void)
{
    sixDice.timer = 0.0f;
    sixDice.rollTime = 10.0f;
    sixDice.lastRoll = 0;
    sixDice.sides = 6;

    weaponCoin.timer = 0.0f;
    weaponCoin.rollTime = 15.0f;
    weaponCoin.lastRoll = 0;
    weaponCoin.sides = 2;
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