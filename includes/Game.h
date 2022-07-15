#pragma once

typedef enum gamestates
{
   menu,
   game,
   highscore
}gamestates;

extern unsigned gamestate;


void Setup(void);
void Run(void);
void Update(void);
void Draw(void);