/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#include "AudioExceptions.hpp"
#include "SdlAudio.hpp"


Mix_Music* SdlAudio::m_music = NULL;
std::map<std::string, sdlSound_t> SdlAudio::m_sounds;
float SdlAudio::m_masterVol = 1.0;
int SdlAudio::m_musicVol = MIX_MAX_VOLUME;

/*
=====================================
SdlAudio::init
=====================================
*/
void SdlAudio::init() {
   SDL_Init(SDL_INIT_AUDIO);

   int audioRate = 44100;
   Uint16 audioFormat = AUDIO_S16; 
   int audioChannels = 2;
   int audioBuffers = 2048;

   if(Mix_OpenAudio(audioRate, audioFormat, audioChannels, audioBuffers))
      throw xAudioException("Could not open audio", __FILE__, __LINE__);
}

/*
=====================================
SdlAudio::quit
=====================================
*/
void SdlAudio::quit() {
   Mix_HaltChannel(-1);

   freeAllSounds();
   freeMusic();

   Mix_CloseAudio();
   SDL_Quit();
}

/*
=====================================
SdlAudio::playMusic
=====================================
*/
void SdlAudio::playMusic(bool rep) {
   if (!Mix_PlayingMusic()) Mix_PlayMusic(SdlAudio::m_music, rep ? -1 : 0);
   if (Mix_PausedMusic()) Mix_ResumeMusic();
}

/*
=====================================
SdlAudio::addSound
=====================================
*/
bool SdlAudio::addSound(std::string file, std::string name) {
   std::map<std::string, sdlSound_t>::iterator it = SdlAudio::m_sounds.find(name);
   if (it != SdlAudio::m_sounds.end()) return false;

   Mix_Chunk* chunk = Mix_LoadWAV(file.data());
   if (chunk == NULL) return false;

   SdlAudio::m_sounds[name].chunk = chunk;
   SdlAudio::m_sounds[name].channel = -1;
   SdlAudio::m_sounds[name].volume = MIX_MAX_VOLUME;

   return true;
}

/*
=====================================
SdlAudio::setSoundVolume
=====================================
*/
bool SdlAudio::setSoundVolume(std::string name, float vol) {
   std::map<std::string, sdlSound_t>::iterator it = SdlAudio::m_sounds.find(name);
   if (it == SdlAudio::m_sounds.end()) return false;

   it->second.volume = static_cast<int>(vol * static_cast<float>(MIX_MAX_VOLUME));
   Mix_VolumeChunk(it->second.chunk, static_cast<int>(static_cast<float>(it->second.volume) * m_masterVol));

   return true;
}

/*
=====================================
SdlAudio::freeSound
=====================================
*/
bool SdlAudio::freeSound(std::string name) {
   std::map<std::string, sdlSound_t>::iterator it = SdlAudio::m_sounds.find(name);
   if (it == SdlAudio::m_sounds.end()) return false;

   Mix_FreeChunk(it->second.chunk);
   m_sounds.erase(it);

   return true;
}

/*
=====================================
SdlAudio::freeAllSounds
=====================================
*/
void SdlAudio::freeAllSounds() {
   std::map<std::string, sdlSound_t>::iterator it = SdlAudio::m_sounds.begin();
   while (it != SdlAudio::m_sounds.end()) {
      Mix_FreeChunk(it->second.chunk);
      ++it;
   }
   m_sounds.clear();
}

/*
=====================================
SdlAudio::setAllSoundVolume
=====================================
*/
void SdlAudio::setAllSoundVolume(float vol) {
   std::map<std::string, sdlSound_t>::iterator it = SdlAudio::m_sounds.begin();
   while (it != SdlAudio::m_sounds.end()) {
      it->second.volume = static_cast<int>(vol * static_cast<float>(MIX_MAX_VOLUME));
      Mix_VolumeChunk(it->second.chunk, static_cast<int>(static_cast<float>(it->second.volume) * m_masterVol));
      ++it;
   }
}

/*
=====================================
SdlAudio::setMasterVolume
=====================================
*/
void SdlAudio::setMasterVolume(float vol) {
   m_masterVol = vol;

   std::map<std::string, sdlSound_t>::iterator it = SdlAudio::m_sounds.begin();
   while (it != SdlAudio::m_sounds.end()) {
      Mix_VolumeChunk(it->second.chunk, static_cast<int>(static_cast<float>(it->second.volume) * m_masterVol));
      ++it;
   }

   Mix_VolumeMusic(static_cast<int>(static_cast<float>(m_musicVol) * m_masterVol));
}

/*
=====================================
SdlAudio::playSound
=====================================
*/
bool SdlAudio::playSound(std::string name) {
   std::map<std::string, sdlSound_t>::iterator it = SdlAudio::m_sounds.find(name);
   if (it == SdlAudio::m_sounds.end()) return false;

   it->second.channel = Mix_PlayChannel(-1, it->second.chunk, 0);

   return true;
}

/*
=====================================
SdlAudio::stopSound
=====================================
*/
bool SdlAudio::stopSound(std::string name) {
   std::map<std::string, sdlSound_t>::iterator it = SdlAudio::m_sounds.find(name);
   if (it == SdlAudio::m_sounds.end()) return false;

   if (it->second.channel != -1) Mix_HaltChannel(it->second.channel);

   return true;
}

/*
=====================================
SdlAudio::stopAllSounds
=====================================
*/
void SdlAudio::stopAllSounds() {
   std::map<std::string, sdlSound_t>::iterator it = SdlAudio::m_sounds.begin();

   while (it != m_sounds.end()) {
      if (it->second.channel != -1) Mix_HaltChannel(it->second.channel);
      ++it;
   }
}
