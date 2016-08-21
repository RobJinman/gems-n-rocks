/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#ifndef __SDL_AUDIO_HPP__
#define __SDL_AUDIO_HPP__


#include <string>
#include <map>
#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"


struct sdlSound_t {
   Mix_Chunk* chunk;
   int channel;
   int volume;
};

class SdlAudio {
   public:
      static void init();
      static void quit();

      static void setMasterVolume(float vol);

      inline static void loadMusicFile(std::string file);
      inline static void freeMusic();
      static void playMusic(bool rep);
      inline static void pauseMusic();
      inline static void stopMusic();
      inline static void setMusicVolume(float vol);

      static bool addSound(std::string file, std::string name);
      static bool freeSound(std::string name);
      static void freeAllSounds();
      static void setAllSoundVolume(float vol);
      static bool setSoundVolume(std::string name, float vol);
      static bool playSound(std::string name);
      static bool stopSound(std::string name);
      static void stopAllSounds();

      virtual ~SdlAudio() = 0;

   private:
      static float m_masterVol;
      static Mix_Music* m_music;
      static int m_musicVol;
      static std::map<std::string, sdlSound_t> m_sounds;
};

inline void SdlAudio::loadMusicFile(std::string file) { SdlAudio::m_music = Mix_LoadMUS(file.data()); }

inline void SdlAudio::freeMusic() {
   if (SdlAudio::m_music) Mix_FreeMusic(m_music);
   m_music = NULL;
}

inline void SdlAudio::pauseMusic() { Mix_PauseMusic(); }

inline void SdlAudio::stopMusic() { Mix_HaltMusic(); }

inline void SdlAudio::setMusicVolume(float vol) {
   m_musicVol = static_cast<int>(vol * static_cast<float>(MIX_MAX_VOLUME));
   Mix_VolumeMusic(static_cast<int>(static_cast<float>(m_musicVol) * m_masterVol));
}


#endif /*!__SDL_AUDIO_HPP__*/
