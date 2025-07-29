#include "../HeaderFiles/timer.h"
#include <SDL2/SDL_timer.h>
void Time_CalculateDeltaTime(TimeInfo *time){
 time->deltaTime = (double)((time->earlyFrame - time->lateFrame) * 1000)/SDL_GetPerformanceFrequency(); 
}
void Time_CalculateGlobalTime(TimeInfo *time){
 time->time += time->deltaTime * 1000;
}

void Timer_Set(Timer *timer, float goalTime){
 timer->goalTime = goalTime;
 timer->currentTime = 0;
 timer->isActive = false;
}
bool Timer_Update(Timer *timer, double deltaTime){
 if(timer->isActive) return true;

 timer->currentTime += deltaTime * 1000;
 if(timer->currentTime >= timer->goalTime){
  timer->currentTime = 0.0f;
  timer->isActive = true;
  return true;
 }
 return false;
}
bool Timer_IsFinished(Timer *timer){
 return timer->isActive || (timer->currentTime >= timer->goalTime);
}
float Timer_GetElapsedTime(Timer *timer){
 return timer->currentTime;
}
float Timer_GetRemainingTime(Timer *timer){
 return timer->goalTime - timer->currentTime;
}
void Timer_ResetTimer(Timer *timer){
 timer->currentTime = 0;
 timer->isActive = false;
}
//returns the progress of the timer 0.0 == no progress / 1.0 = finished
float Timer_GetProgress(Timer *timer){
 return 1.0f - (timer->goalTime - timer->currentTime);
}
