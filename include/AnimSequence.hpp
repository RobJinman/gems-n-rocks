/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#ifndef __ANIM_SEQUENCE_HPP__
#define __ANIM_SEQUENCE_HPP__


#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <cstdlib>

#include "AnimFrame.hpp"
#include "SdlAudio.hpp"


enum AnimStatus {
   ANIM_IDLE,
   ANIM_PLAYING,
   ANIM_FINISHED
};

class Item;
class Grid;

class AnimSequence : public boost::enable_shared_from_this<AnimSequence> {
   private:
      std::string    name_;
      int            nFrames_;
      float          frameRate_;
      AnimFrame*     frame_;
      bool           vanishes_;
      double         prevFrame_;       // When the last frame was drawn
      int            currentFrame_;
      AnimStatus     status_;
      std::string    soundName_;

      static std::string appPath;

   public:
      AnimSequence();
      AnimSequence(const AnimSequence& anim);
      ~AnimSequence();

      inline static void setAppPath(const std::string& path);

      void loadFromFile(const std::string& file);
      int start();                              // Returns whether or not the animation was successfully started (1 or 0)
      inline void stop();
      void update(boost::shared_ptr<Item> item, boost::shared_ptr<Grid> grid);

      inline const std::string& getName() const;
      inline AnimStatus status() const;
      inline void setStatus(AnimStatus status);

      inline void setName(const std::string& name);
      inline bool vanishes() const;
};

inline void AnimSequence::setAppPath(const std::string& path) { appPath = path; }

inline void AnimSequence::setName(const std::string& name) { name_.assign(name); }

inline const std::string& AnimSequence::getName() const { return name_; }

inline AnimStatus AnimSequence::status() const { return status_; }

inline void AnimSequence::setStatus(AnimStatus status) { status_ = status; }

inline bool AnimSequence::vanishes() const { return vanishes_; }

inline void AnimSequence::stop() { SdlAudio::stopSound(soundName_); status_ = ANIM_IDLE; }


#endif /*!__ANIM_SEQUENCE_HPP__*/
