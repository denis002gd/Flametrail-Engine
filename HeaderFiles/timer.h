#ifndef TIMER_H
#define TIMER_H
#include <stdbool.h>
#include "render.h"

typedef struct{
  double deltaTime;
  Uint64 earlyFrame;
  Uint64 lateFrame;
  Uint64 elapsedTime;

  //time since beginning of game
  float time;
}TimeInfo;

typedef struct{
  float currentTime;
  float goalTime;
  bool isActive;
}Timer;

void Time_CalculateDeltaTime(TimeInfo *time);
void Time_CalculateGlobalTime(TimeInfo *time);
//returns true after timeGoal is reached (must use delta time)
void Timer_Set(Timer *timer, float goalTime);
bool Timer_Update(Timer *timer, double deltaTime);
bool Timer_IsFinished(Timer *timer);
float Timer_GetElapsedTime(Timer *timer);
float Timer_GetRemainingTime(Timer *timer);
void Timer_ResetTimer(Timer *timer);
float Timer_GetProgress(Timer *timer);
#endif // !TIMER_H
