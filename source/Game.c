#include <stdio.h>

#include "Globals.h"
#include "Game.h"
#include "DiceSystem.h"
#include "Player.h"
#include "Enemies.h"
#include "time.h"

#define RAYGUI_IMPLEMENTATION
#include "raylib/raygui.h"

#include "raylib/raymath.h"
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#define RLIGHTS_IMPLEMENTATION
#include "raylib/rlights.h"

#define PHYSAC_IMPLEMENTATION
#define PHYSAC_NO_THREADS
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

//Textures
Texture2D AmmoUITex = { 0 };
Texture2D CurrentEffectUITex = { 0 };
Texture2D CurrentEnemyUITex = { 0 };
Texture2D ScoreUITex = { 0 };
Texture2D WeaponUITex = { 0 };

//Forward declarations
void UpdateGame(void);
void DrawGame(void);
void GenerateLevel(void);
void SetupPlayerAndGuns(void);
void SetupEnemyModels(void);
void SetupDice(void);

//Private functions

void ResetGame(void)
{
    SetupPlayer();

    SetupDice();

    //TEMP
    if (enemies != NULL)
    {
        arrfree(enemies);
    }
    Enemy temp2 = CreateEnemy(Skull, (Vector3) { 40.0f, 20.0f, 1.0f }, player.camera.position, (Vector3) { 1.0f, 1.0f, 1.0f }, 10);
    arrpush(enemies, temp2);
}

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
    SetMusicVolume(ingameMusic, MasterVolume * MusicVolume);
}

//Definitions
void Setup(void)
{
    InitWindow(screenWidth, screenHeight, "GMTK2022");

    InitPhysics();
    InitAudioDevice();
    SetWindowState(FLAG_VSYNC_HINT);

    LoadGunModels();

    ResetGame();

    shader = LoadShader(TextFormat("shaders/base_lighting.vs", GLSL_VERSION), TextFormat("shaders/lighting.fs", GLSL_VERSION));
    shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
    int ambientLoc = GetShaderLocation(shader, "ambient");
    SetShaderValue(shader, ambientLoc, (float[4]) { 0.3f, 0.3f, 0.3f, 1.0f }, SHADER_UNIFORM_VEC4);

    Light temp = CreateLight(LIGHT_POINT, (Vector3) { -2.0f, 40.0f, -2.0f }, Vector3Zero(), WHITE, shader);
    arrpush(lights, temp);

    SetupMenu();

    SetCameraMode(player.camera, CAMERA_FIRST_PERSON);

    //Set the shader for all our objects.
    //Guns
    SetupPlayerAndGuns();
    
    //Enemies
    SetupEnemyModels();

    //Level
    GenerateLevel();

    //See if a highscore exists. If it does we load it.
    FILE* highscoreFile;
    if (highscoreFile = fopen("bin/highscore.txt", "r"))
    {
        char buf[100];
        fgets(buf, 100, highscoreFile);
        HighScore = strtol(buf, NULL, 10);
        fclose(highscoreFile);
    }

    //See if a volume file exists. If it does we load it.
    FILE* volumeFile;
    if (volumeFile = fopen("bin/volume.txt", "r"))
    {
        char masterBuf[10];
        fgets(masterBuf, 100, volumeFile);
        MasterVolume = (float)(strtol(masterBuf, NULL, 10)) / 100.0f;

        char musicBuf[10];
        fgets(musicBuf, 100, volumeFile);
        MusicVolume = (float)(strtol(musicBuf, NULL, 10)) / 100.0f;

        char soundeffectBuf[10];
        fgets(soundeffectBuf, 100, volumeFile);
        SoundEffectVolume = (float)(strtol(soundeffectBuf, NULL, 10)) / 100.0f;

        fclose(volumeFile);
    }

    //Load textures.
    
    AmmoUITex = LoadTexture("resources/Textures/Ammo.png");
    CurrentEffectUITex = LoadTexture("resources/Textures/CurrentEffect.png");
    CurrentEnemyUITex = LoadTexture("resources/Textures/CurrentEnemy.png");
    ScoreUITex = LoadTexture("resources/Textures/ScoreAndTimers.png");
    WeaponUITex = LoadTexture("resources/Textures/WeaponTimer.png");
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
    default:
        break;
    }
}

void UpdateGame(void)
{
    if (innerGamestate == none)
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
        if (IsKeyPressed(KEY_ESCAPE))
        {
            EnableCursor();
            innerGamestate = pause;
            PauseMusicStream(ingameMusic);
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
    else if (innerGamestate == pause)
    {
        if (IsKeyPressed(KEY_ESCAPE))
        {
            DisableCursor();
            ResumeMusicStream(ingameMusic);
            innerGamestate = none;
        }
    }
    else if (innerGamestate == settings)
    {
        return;
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

    //DRAW THE UI TEXTURES!.
    DrawTexture(AmmoUITex, 0, screenHeight - AmmoUITex.height, WHITE);
    DrawTexture(CurrentEffectUITex, 0, 0, WHITE);
    DrawTexture(CurrentEnemyUITex, screenWidth - CurrentEnemyUITex.width, 0, WHITE);
    DrawTexture(ScoreUITex, screenWidth / 2 - ScoreUITex.width / 2, 0, WHITE);
    DrawTexture(WeaponUITex, screenWidth / 2 - WeaponUITex.width / 2, screenHeight - WeaponUITex.height, WHITE);

    //Effect timer
    char str[2];
    sprintf(str, "%.0f", sixDice.rollTime - sixDice.timer);
    DrawText(str, screenWidth / 2 - 310, 10, 50, WHITE);

    //Effect Icon
    //Needs to be added

    //Enemy timer
    //Needs to be added

    //Current enemy icon
    //Needs to be added.

    //Ammo
    char ammoStr[10];
    sprintf(ammoStr, "%d / %d", player.gun.currentAmmo, player.gun.maxAmmo);
    DrawText(ammoStr, 30, screenHeight - 70, 50, WHITE);

    //Current gun
    switch (player.gun.currentGun)
    {
    case Pistol:
    {
        DrawText("Current Weapon: Pistol", screenWidth / 2 - 185, screenHeight - 40, 30, WHITE);
        break;
    }
    case SMG:
    {
        DrawText("Current Weapon: SMG", screenWidth / 2 - 185, screenHeight - 40, 30, WHITE);
        break;
    }
    case Shotgun:
    {
        DrawText("Current Weapon: Shotgun", screenWidth / 2 - 185, screenHeight - 40, 30, WHITE);
        break;
    }
    default:
    {
        break;
    }
    }

    //Time until weapon switch
    char weaponSwitchStr[10];
    sprintf(weaponSwitchStr, "%.1f", weaponCoin.rollTime - weaponCoin.timer);
    DrawText(weaponSwitchStr, screenWidth / 2 - 40, screenHeight - 90, 50, WHITE);

    //Crosshair
    float lineLength = 20.0f;
    DrawLineEx((Vector2) { screenWidth / 2.0f, screenHeight / 2.0f - lineLength / 2.0f }, (Vector2) { screenWidth / 2.0f, screenHeight / 2.0f + lineLength / 2.0f }, 3.0f, BLACK);
    DrawLineEx((Vector2) { screenWidth / 2.0f - lineLength / 2.0f, screenHeight / 2.0f }, (Vector2) { screenWidth / 2.0f + lineLength / 2.0f, screenHeight / 2.0f }, 3.0f, BLACK);
    

    //Current points.
    DrawText("SCORE", screenWidth / 2 - 80, 5, 50, WHITE);
    char scoreBuf[100];
    sprintf(scoreBuf, "%d", CurrentScore);
    DrawText(scoreBuf, screenWidth / 2 - 10, 60, 40, BLUE);


    if (innerGamestate == pause)
    {
        DrawSettings();
        DrawRectangle(screenWidth / 2 - 60, screenHeight / 2 - 90, 120, 190, RAYWHITE);

        if (GuiButton(bpauseContinue, GuiIconText(RICON_DEMON, "Continue")))
        {
            DisableCursor();
            ResumeMusicStream(ingameMusic);
            innerGamestate = none;
        }
        if (GuiButton(bpauseSettings, GuiIconText(RICON_DEMON, "Settings")))
        {
            innerGamestate = settings;
        }
        if (GuiButton(bpauseQuit, GuiIconText(RICON_DEMON, "Quit")))
        {
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
            innerGamestate = none;
            gamestate = menu;
        }
    }
    else if (innerGamestate == settings)
    {
        DrawSettings();
        DrawRectangle(screenWidth / 2.0f - 210, screenHeight / 2 - 10, 420, 310, RAYWHITE);

        char masterStr[5];
        sprintf(masterStr, "%d", (int)(MasterVolume * 100.0f));
        MasterVolume = GuiSlider(bmaster, "Master Volume", masterStr, MasterVolume, 0.0f, 1.0f);

        char musicStr[5];
        sprintf(musicStr, "%d", (int)(MusicVolume * 100.0f));
        MusicVolume = GuiSlider(bmusic, "Music Volume", musicStr, MusicVolume, 0.0f, 1.0f);

        char soundeffectStr[5];
        sprintf(soundeffectStr, "%d", (int)(SoundEffectVolume * 100.0f));
        SoundEffectVolume = GuiSlider(bsoundeffect, "Sound Effect Volume", soundeffectStr, SoundEffectVolume, 0.0f, 1.0f);

        if (GuiButton(bpauseReturn, GuiIconText(RICON_DOOR, "Return")))
        {
            //Write the current volume to the volume file
            FILE* volumeFile;
            volumeFile = fopen("bin/volume.txt", "w");

            char masterBuf[100];
            sprintf(masterBuf, "%d\n", (int)(MasterVolume * 100.0f));
            fputs(masterBuf, volumeFile);

            char musicBuf[100];
            sprintf(musicBuf, "%d\n", (int)(MusicVolume * 100.0f));
            fputs(musicBuf, volumeFile);

            char soundeffectBuf[100];
            sprintf(soundeffectBuf, "%d\n", (int)(SoundEffectVolume * 100.0f));
            fputs(soundeffectBuf, volumeFile);

            fclose(volumeFile);

            innerGamestate = pause;

            SetMusicVolume(ingameMusic, MasterVolume * MusicVolume);
        }
    }
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



Rectangle bplay = { 0 };

Rectangle bsettings = { 0 };

Rectangle bcredits = { 0 };

Rectangle bexit = { 0 };

Rectangle breturn = { 0 };

Rectangle bmaster = { 0 };
Rectangle bmusic = { 0 };
Rectangle bsoundeffect = { 0 };

Rectangle bpauseContinue = { 0 };
Rectangle bpauseSettings = { 0 };
Rectangle bpauseQuit = { 0 };

Rectangle bpauseReturn = { 0 };

Music menuMusic = { 0 };

void SetupMenu(void)
{
    bplay = (Rectangle){
    .height = 30.f,
    .width = 90.f,
    .x = screenWidth / 2.0f - 45.0f,
    .y = screenHeight / 2.0f
    };

    bsettings = (Rectangle){
        .height = 30.f,
        .width = 90.f,
        .x = screenWidth / 2.0f - 45.0f,
        .y = screenHeight / 2.0f + 80
    };

    bcredits = (Rectangle){
        .height = 30.f,
        .width = 90.f,
        .x = screenWidth / 2.0f - 45.0f,
        .y = screenHeight / 2.0f + 160
    };

    bexit = (Rectangle){
        .height = 30.f,
        .width = 90.f,
        .x = screenWidth / 2.0f - 45.0f,
        .y = screenHeight / 2.0f + 240
    };

    breturn = (Rectangle){
        .height = 50.f,
        .width = 90.f,
        .x = screenWidth / 2.0f - 45.0f,
        .y = screenHeight - 80
    };

    bmaster = (Rectangle){
        .height = 50.f,
        .width = 180.f,
        .x = screenWidth / 2.0f - 90,
        .y = screenHeight / 2.0f
    };

    bmusic = (Rectangle){
        .height = 50.f,
        .width = 180.f,
        .x = screenWidth / 2.0f - 90.0f,
        .y = screenHeight / 2.0f + 80
    };

    bsoundeffect = (Rectangle){
        .height = 50.f,
        .width = 180.f,
        .x = screenWidth / 2.0f - 90.0f,
        .y = screenHeight / 2.0f + 160
    };

    //For pause screen.
    bpauseContinue = (Rectangle){
        .height = 50.f,
        .width = 90.f,
        .x = screenWidth / 2.0f - 45.0f,
        .y = screenHeight / 2.0f - 80
    };

    bpauseSettings = (Rectangle){
        .height = 50.f,
        .width = 90.f,
        .x = screenWidth / 2.0f - 45.0f,
        .y = screenHeight / 2.0f - 20
    };

    bpauseQuit = (Rectangle){
        .height = 50.f,
        .width = 90.f,
        .x = screenWidth / 2.0f - 45.0f,
        .y = screenHeight / 2.0f + 40
    };

    bpauseReturn = (Rectangle){
        .height = 50.f,
        .width = 90.f,
        .x = screenWidth / 2.0f - 45.0f,
        .y = screenHeight / 2.0f + 240
    };
}

void UpdateMenu(void)
{
    if (!IsMusicStreamPlaying(menuMusic))
    {
        menuMusic = LoadMusicStream("resources/Sounds/Music/Menu.wav");
        PlayMusicStream(menuMusic);
        SetMusicVolume(menuMusic, MasterVolume * MusicVolume);
    }
    UpdateMusicStream(menuMusic);
    EnableCursor();

    if (innerGamestate == none)
    {
        if (GuiButton(bplay, GuiIconText(RICON_DEMON, "Play")))
        {
            ResetGame();
            gamestate = game;
            DisableCursor();
            StopMusicStream(menuMusic);
        }
        if (GuiButton(bsettings, GuiIconText(RICON_FILETYPE_AUDIO, "Settings")))
        {
            innerGamestate = settings;
        }
        if (GuiButton(bcredits, GuiIconText(RICON_INFO, "Credits")))
        {
            innerGamestate = credits;
        }
        if (GuiButton(bexit, GuiIconText(RICON_UNDO, "Exit")))
        {
            //Write the current volume to the volume file
            FILE* volumeFile;
            volumeFile = fopen("bin/volume.txt", "w");

            char masterBuf[100];
            sprintf(masterBuf, "%d\n", (int)(MasterVolume * 100.0f));
            fputs(masterBuf, volumeFile);

            char musicBuf[100];
            sprintf(musicBuf, "%d\n", (int)(MusicVolume * 100.0f));
            fputs(musicBuf, volumeFile);

            char soundeffectBuf[100];
            sprintf(soundeffectBuf, "%d\n", (int)(SoundEffectVolume * 100.0f));
            fputs(soundeffectBuf, volumeFile);

            fclose(volumeFile);

            run = false;
        }
    }
    else if (innerGamestate == settings)
    {
        char masterStr[5];
        sprintf(masterStr, "%d", (int)(MasterVolume * 100.0f));
        MasterVolume = GuiSlider(bmaster, "Master Volume", masterStr, MasterVolume, 0.0f, 1.0f);

        char musicStr[5];
        sprintf(musicStr, "%d", (int)(MusicVolume * 100.0f));
        MusicVolume = GuiSlider(bmusic, "Music Volume", musicStr, MusicVolume, 0.0f, 1.0f);

        char soundeffectStr[5];
        sprintf(soundeffectStr, "%d", (int)(SoundEffectVolume * 100.0f));
        SoundEffectVolume = GuiSlider(bsoundeffect, "Sound Effect Volume", soundeffectStr, SoundEffectVolume, 0.0f, 1.0f);

        if (GuiButton(breturn, GuiIconText(RICON_DOOR, "Return")))
        {
            //Write the current volume to the volume file
            FILE* volumeFile;
            volumeFile = fopen("bin/volume.txt", "w");

            char masterBuf[100];
            sprintf(masterBuf, "%d\n", (int)(MasterVolume * 100.0f));
            fputs(masterBuf, volumeFile);

            char musicBuf[100];
            sprintf(musicBuf, "%d\n", (int)(MusicVolume * 100.0f));
            fputs(musicBuf, volumeFile);

            char soundeffectBuf[100];
            sprintf(soundeffectBuf, "%d\n", (int)(SoundEffectVolume * 100.0f));
            fputs(soundeffectBuf, volumeFile);

            fclose(volumeFile);

            innerGamestate = none;
        }

        //Update music volume while changing settings.
        SetMusicVolume(menuMusic, MasterVolume * MusicVolume);
    }
    else if (innerGamestate == credits)
    {
        if (GuiButton(breturn, GuiIconText(RICON_DOOR, "Return")))
        {
            innerGamestate = none;
        }
    }
}


void DrawMenu(void)
{
    BeginDrawing();
    ClearBackground(RAYWHITE);
    if (innerGamestate == none || innerGamestate == settings)
    {
        if (HighScore > 0)
        {
            char scoreBuf[100];
            sprintf(scoreBuf, "HighScore: %d", HighScore);
            DrawText(scoreBuf, screenWidth / 2 - 180, screenHeight / 4, 50, BLUE);
        }
        DrawText("Diece", screenWidth / 2 - 130, screenHeight / 8, 100, RED);

        if (innerGamestate == settings)
        {
            DrawSettings();
        }
    }
    else if (innerGamestate == credits)
    {
        DrawCredits();
    }

    EndDrawing();
}

void DrawSettings(void)
{
    DrawRectangle(0, 0, screenWidth, screenHeight, (Color) { .r = 150, .g = 150, .b = 150, .a = 100 });
}

void DrawCredits(void)
{
    DrawText("A game made by:", (int)(screenWidth / 2.0f - 250), (int)(screenHeight / 8.0f), 60, BLACK);
    DrawText("Emil Hogstedt & Oscar Milstein", (int)(screenWidth / 2.0f - 450), (int)(screenHeight / 8.0f + 70), 60, BLACK);
    DrawText("Music: Armageddon, Valhalla & Wrath by Alexander Nakarada (www.serpentsoundstudios.com)", (int)(screenWidth / 2.0f - 700), (int)(screenHeight / 2.5f), 30, BLACK);
    DrawText("Licensed under Creative Commons BY Attribution 4.0 License", (int)(screenWidth / 2.0f - 450), (int)(screenHeight / 2.5f + 40), 30, BLACK);
    DrawText("https://creativecommons.org/licenses/by/4.0/", (int)(screenWidth / 2.0f - 350), (int)(screenHeight / 2.5f + 80), 30, BLACK);

    DrawText("The rest of the music & all the sound effects have royalty free/creative commons licenses", (int)(screenWidth / 2.0f - 730), (int)(screenHeight / 1.5f), 30, BLACK);
    DrawText("or have been bought by the developers.", (int)(screenWidth / 2.0f - 320), (int)(screenHeight / 1.5f + 35), 30, BLACK);

    DrawText("Special shoutout to the demon under my bed", (int)(screenWidth / 2.0f - 170), (int)(screenHeight / 1.2f), 15, BLACK);
}