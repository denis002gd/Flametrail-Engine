#include "timer.h"
#include <SDL2/SDL_timer.h>
void CalculateDeltaTime(GameContext *Game_Context){
 Game_Context->deltaTime = (double)((Game_Context->earlyFrame - Game_Context->lateFrame) * 1000)/SDL_GetPerformanceFrequency(); 
}
