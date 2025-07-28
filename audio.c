#include "audio.h"
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string.h>

//Initializes an AudioManager Struct, call once
bool Audio_Init(AudioManager *audioManager){
  if(SDL_Init(SDL_INIT_AUDIO) < 0){
    printf("SDL audio initialization failed: %s\n", Mix_GetError());
    return false;
  }
  int flags = MIX_INIT_MP3 | MIX_INIT_OGG;
  if((Mix_Init(flags) & flags) != flags){
    printf("Mix_init failed: %s\n", Mix_GetError());
    return false;
  }

  if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0){
    printf("Mix_openAudio falied: %s\n", Mix_GetError());
    return false;
  }
  Mix_AllocateChannels(16);
  audioManager->initialized = true;
  audioManager->masterVolume = 1.0f;
  audioManager->clipCount = 0;

  return true;
}
//Free all components of the AudioManager struct, call at quit
void Audio_Quit(AudioManager *audioManager){
  if(!audioManager->initialized) return;

  for(int i = 0; i < audioManager->clipCount; i++){
    if(audioManager->clips[i].isLoaded){
      if(audioManager->clips->isMusic && audioManager->clips->music){
        Mix_FreeMusic(audioManager->clips[i].music);
      }else if(audioManager->clips[i].chunk){
        Mix_FreeChunk(audioManager->clips[i].chunk);
      }
    }
  }
  Mix_CloseAudio();
  Mix_Quit();
  audioManager->initialized = false;
}

AudioSource *Audio_CreateAudioSource(float volume, bool loop, bool playOnStart){
  AudioSource *audioSrc = malloc(sizeof(AudioSource));
  audioSrc->playOnStart = playOnStart;
  audioSrc->loop = loop;
  if(volume > 1.0f) volume = 1.0f;
  if(volume < 0.0f) volume = 0.0f;
  audioSrc->volume = volume;
  return audioSrc;
}
//Load a Music or Audio file 
AudioClip *Audio_LoadClip(AudioManager* audioManager, const char* filename, const char* name, bool isMusic){
  if(!audioManager->initialized || audioManager->clipCount >= MAX_AUDIO_CLIPS){
    return NULL;
  }
  AudioClip* clip = &audioManager->clips[audioManager->clipCount];

  strncpy(clip->name, name, MAX_FILENAM_LENGHT - 1);
  clip->name[MAX_FILENAM_LENGHT - 1] = '\0';
  clip->isMusic = isMusic;
  clip->volume = 1.0f;
  clip->isLoaded = false;

  if(isMusic){
    clip->music = Mix_LoadMUS(filename);
    if(!clip->music){
      printf("Failed to load music %s: %s\n", filename, Mix_GetError());
      return NULL;
    }
    clip->chunk = NULL;
  }else{
    clip->chunk = Mix_LoadWAV(filename);
    if(!clip->chunk){
      printf("Failed to load chunk %s: %s\n", filename, Mix_GetError());
      return NULL;
    }
    clip->music = NULL;
  }
  clip->isLoaded = true;
  audioManager->clipCount++;
  return clip;
}
//Plays a loaded audio file (Use Audio_PlayOneShot for SFX)
int Audio_PlaySound(AudioManager *audioManager, const char* clipName, bool loop, float volume) {
    AudioClip* clip = Audio_FindClip(audioManager, clipName);
    if (!clip || clip->isMusic || !clip->chunk) {
        printf("Sound clip not found or invalid: %s\n", clipName);
        return -1;
    }
    
    int sdlVolume = (int)(volume * clip->volume * audioManager->masterVolume * 128.0f);
    Mix_VolumeChunk(clip->chunk, sdlVolume);
    
    int channel = Mix_PlayChannel(-1, clip->chunk, loop ? -1 : 0);
    if (channel == -1) {
        printf("Failed to play sound: %s\n", Mix_GetError());
    }
    
    return channel;
}
//Returns the audio clip based on the name (NULL if none found)
AudioClip *Audio_FindClip(AudioManager* audioManager, const char* name){
  for(int i = 0; i < audioManager->clipCount; i++){
    if(strcmp(audioManager->clips[i].name, name) == 0 && audioManager->clips[i].isLoaded){
      return &audioManager->clips[i];
    }
  }
  return NULL;
}
//Plays a loaded music clip
bool Audio_PlayMusic(AudioManager *audioManager, const char* name, float volume, bool loop){
  AudioClip *clip = Audio_FindClip(audioManager, name);
  if(clip == NULL || !clip->isMusic || !clip->music){
    printf("Music clip not found, make shure names match or the music is loaded!\n");
    return false;
  }
  int sdlVolume = (int)(volume * clip->volume * audioManager->masterVolume * 128);
  Mix_VolumeMusic(sdlVolume);

  if(Mix_PlayMusic(clip->music, loop ? 0 : -1 == -1)){
    printf("Failed to play music: %s\n", Mix_GetError());
    return false;
  }
   return true;
}
//Plays a sound effect by name
void Audio_PlayOneShot(AudioManager *audioManager, const char* name, float volume){
  Audio_PlaySound(audioManager, name, false, volume);
}
void Audio_StopMusic(){
  Mix_HaltMusic();
}
void Audio_PauseMusic(){
  Mix_PauseMusic();
}
void Audio_ResumeMusic(){
  Mix_ResumeMusic();
}
//returns true if music is playing currently
bool Audio_IsPlayingMusic(){
  return Mix_PlayingMusic();
}
//Set master (world) volume 0.0f = no sound 1.0f = full volume
void Audio_SetMasterVolume(AudioManager *audioManager, float newVolume){
   audioManager->masterVolume = newVolume;
  if(newVolume < 0.0f) audioManager->masterVolume = 0.0f;
  if(newVolume > 1.0f) audioManager->masterVolume = 1.0f;
}
