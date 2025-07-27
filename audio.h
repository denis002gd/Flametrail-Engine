#ifndef AUDIO_H
#define AUDIO_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdbool.h>

#define MAX_AUDIO_CLIPS 32
#define MAX_FILENAM_LENGHT 128

typedef struct{
  char name[MAX_FILENAM_LENGHT];
  Mix_Chunk* chunk;
  Mix_Music* music;
  bool isMusic;
  bool isLoaded;
  float volume;
}AudioClip;

typedef struct {
  int channel;
  AudioClip* clip;
  bool isPlaying;
  bool loop;
  float volume;
  float pitch;
}AudioSource;

typedef struct{
  AudioClip clips[MAX_AUDIO_CLIPS];
  int clipCount;
  bool initialized;
  float masterVolume;
}AudioManager;

bool Audio_Init(AudioManager *audioManager);
void Audio_Quit(AudioManager *audioManager);
AudioClip *Audio_LoadClip(AudioManager* audioManager,const char* filename, const char* name, bool isMusic);
AudioClip *Audio_FindClip(AudioManager* audioManager, const char* name);
bool Audio_PlayMusic(AudioManager *audioManager, const char* name, float volume, bool loop);
void Audio_PlayOneShot(AudioManager *audioManager, const char* name, float volume);
void Audio_StopMusic();
void Audio_PauseMusic();
void Audio_ResumeMusic();
bool Audio_IsPlayingMusic();
void Audio_SetMasterVolume(AudioManager *audioManager, float newVolume);

#endif // !DEBUG
