#include "Menu.h"
#include "raylib/raylib.h"
#include "raylib/raygui.h"
#include "Globals.h"

Rectangle bplay = {
        .height = 30.f,
        .width = 90.f,
        .x = 740.f,
        .y = 900.f / 2.f
    };

Rectangle bexit = {
        .height = 30.f,
        .width = 90.f,
        .x = 740.f,
        .y = 900.f / 1.8f
    };

Music menuMusic = { 0 };

void UpdateMenu(void)
{
    if (!IsMusicStreamPlaying(menuMusic))
    {
        menuMusic = LoadMusicStream("resources/SOunds/Music/Menu.wav");
        PlayMusicStream(menuMusic);
    }
    UpdateMusicStream(menuMusic);
    EnableCursor();
    
    if (GuiButton(bplay, GuiIconText(RICON_DEMON, "Play")))
    {
        gamestate = game;
        DisableCursor();
        StopMusicStream(menuMusic);
    }
    if (GuiButton(bexit, GuiIconText(RICON_UNDO, "Exit")))
        run = false;
}

void DrawMenu(void)
{
    BeginDrawing();

    ClearBackground(RAYWHITE);

    
    DrawText("Diece", 650, 200, 100, RED);
    EndDrawing();
}