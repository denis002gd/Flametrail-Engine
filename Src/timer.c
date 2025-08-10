#include "../HeaderFiles/timer.h"
#include <SDL2/SDL_timer.h>

void Time_CalculateDeltaTime(TimeInfo *time){
    if(!time) return;
    time->deltaTime = (double)(time->earlyFrame - time->lateFrame) / 1000.0;
    
    if(time->deltaTime > 0.1) {
        time->deltaTime = 0.1;
    }
}

void Time_CalculateGlobalTime(TimeInfo *time){
    if(!time) return;
    time->time += (float)time->deltaTime;
}

void Time_Initialize(TimeInfo *time){
    if(!time) return;
    
    time->earlyFrame = SDL_GetTicks();
    time->lateFrame = time->earlyFrame;
    time->deltaTime = 0.0;
    time->time = 0.0f;
    time->frameCount = 0;
    time->fps = 0.0f;
    time->fpsUpdateTimer = SDL_GetTicks();
    time->fpsFrameCount = 0;
    time->performanceCounter = SDL_GetPerformanceCounter();
    time->elapsedTime = 0;
}

void Time_Update(TimeInfo *time){
    if(!time) return;
    
    time->lateFrame = time->earlyFrame;
    time->earlyFrame = SDL_GetTicks();
    
    Time_CalculateDeltaTime(time);
    Time_CalculateGlobalTime(time);
    
    time->frameCount++;
    time->fpsFrameCount++;
    
    // Update FPS every second
    if(time->earlyFrame - time->fpsUpdateTimer >= 1000){
        time->fps = (float)time->fpsFrameCount / ((time->earlyFrame - time->fpsUpdateTimer) / 1000.0f);
        time->fpsFrameCount = 0;
        time->fpsUpdateTimer = time->earlyFrame;
    }
    
    // Update elapsed time since start
    time->elapsedTime = time->earlyFrame;
}

void Time_CalculateDeltaTime_HighPrecision(TimeInfo *time){
    if(!time) return;
    
    Uint64 currentCounter = SDL_GetPerformanceCounter();
    Uint64 lastCounter = time->performanceCounter;
    
    if(lastCounter == 0) {
        lastCounter = currentCounter; 
    }
    
    time->deltaTime = (double)(currentCounter - lastCounter) / SDL_GetPerformanceFrequency();
    time->performanceCounter = currentCounter;
    
    // Clamp delta time
    if(time->deltaTime > 0.1) {
        time->deltaTime = 0.1;
    }
}

// Timer Functions
void Timer_Set(Timer *timer, float goalTime){
    if(!timer) return;
    
    timer->goalTime = goalTime;
    timer->currentTime = 0.0f;
    timer->isActive = true;  
}

bool Timer_Update(Timer *timer, double deltaTime){
    if(!timer || !timer->isActive) return false;

    timer->currentTime += (float)deltaTime;
    
    if(timer->currentTime >= timer->goalTime){
        timer->isActive = false;
        return true;
    }
    
    return false;
}

bool Timer_IsFinished(Timer *timer){
    if(!timer) return true;
    return !timer->isActive && (timer->currentTime >= timer->goalTime);
}

float Timer_GetElapsedTime(Timer *timer){
    if(!timer) return 0.0f;
    return timer->currentTime;
}

float Timer_GetRemainingTime(Timer *timer){
    if(!timer) return 0.0f;
    
    float remaining = timer->goalTime - timer->currentTime;
    return remaining > 0.0f ? remaining : 0.0f;
}

void Timer_ResetTimer(Timer *timer){
    if(!timer) return;
    
    timer->currentTime = 0.0f;
    timer->isActive = true;
}

void Timer_Stop(Timer *timer){
    if(!timer) return;
    timer->isActive = false;
}

void Timer_Resume(Timer *timer){
    if(!timer) return;
    if(timer->currentTime < timer->goalTime) {
        timer->isActive = true;
    }
}

// Returns the progress of the timer 0.0 == no progress / 1.0 = finished
float Timer_GetProgress(Timer *timer){
    if(!timer || timer->goalTime <= 0.0f) return 0.0f;
    
    float progress = timer->currentTime / timer->goalTime;
    return progress > 1.0f ? 1.0f : progress;
}

// Additional utility functions
bool Timer_IsRunning(Timer *timer) {
    if(!timer) return false;
    return timer->isActive && timer->currentTime < timer->goalTime;
}

void Timer_SetProgress(Timer *timer, float progress) {
    if(!timer || progress < 0.0f || progress > 1.0f) return;
    
    timer->currentTime = timer->goalTime * progress;
    if(progress >= 1.0f) {
        timer->isActive = false;
    }
}
