/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#include <fstream>
#include <stdio.h>
#include <cstdlib>
#include <stack>
#include <string.h>
#include <GL/glut.h>

#include "Grid.hpp"
#include "AnimSequence.hpp"
#include "AnimFrame.hpp"
#include "xException.hpp"
#include "ItemFactory.hpp"
#include "Image.hpp"
#include "SdlAudio.hpp"


std::string AnimSequence::appPath = std::string();

/*
=====================================
AnimSequence::AnimSequence
=====================================
*/
AnimSequence::AnimSequence() {
   status_ = ANIM_IDLE;
   vanishes_ = false;
   frame_ = NULL;
}

/*
=====================================
AnimSequence::AnimSequence
=====================================
*/
AnimSequence::AnimSequence(const AnimSequence& anim) {
   name_.assign(anim.name_);
   soundName_.assign(anim.soundName_);
   nFrames_ = anim.nFrames_;
   frameRate_ = anim.frameRate_;
   vanishes_ = anim.vanishes_;
   prevFrame_ = anim.prevFrame_;
   currentFrame_ = anim.currentFrame_;
   status_ = anim.status_;

   try {
      frame_ = NULL;
      frame_ = new AnimFrame[nFrames_];
   }
   catch (std::bad_alloc&) {
      throw xMemAllocFail("Could not construct AnimSequence", __FILE__, __LINE__);
   }

   for (int i = 0; i < nFrames_; i++)
      frame_[i] = AnimFrame(anim.frame_[i]);
}

/*
=====================================
AnimSequence::loadFromFile
=====================================
*/
void AnimSequence::loadFromFile(const std::string& path) {
   std::ifstream file(path.data());
   char buffer[128];

   if (!file.is_open()) throw xFileNotFound("Could not open animation file", __FILE__, __LINE__, path);

   file.getline(buffer, 128);
   name_.assign(buffer);

   file.getline(buffer, 128);
   int vnsh;
   if (sscanf(buffer, "%d %f %d", &nFrames_, &frameRate_, &vnsh) != 3)
      throw xBadFile("Could not read from animation file", __FILE__, __LINE__, path);

   vanishes_ = (bool)vnsh;

   try {
      frame_ = new AnimFrame[nFrames_];
   }
   catch (std::bad_alloc&) {
      throw xMemAllocFail("Could not create AnimFrame", __FILE__, __LINE__);
   }

   int f = 0;
   file.getline(buffer, 128);
   while (f < nFrames_) {

      if (sscanf(buffer, "%d %d %d %d %d %d", &frame_[f].changeInPos[0], &frame_[f].changeInPos[1],
      &frame_[f].imgOffset[0], &frame_[f].imgOffset[1], &frame_[f].imgDimension[0], &frame_[f].imgDimension[1]) != 6) {

         file.close();
         delete[] frame_;
         frame_ = NULL;

         throw xBadFile("Could not read from animation file", __FILE__, __LINE__, path);
      }

      for (int i = 0; i < 128; i++) buffer[i] = '\0';
      file.getline(buffer, 128);
      f++;
   }

   soundName_.assign(buffer);

   std::stringstream strPath, strName;
   strPath << AnimSequence::appPath << "/" << soundName_;
   strName << soundName_ << this;
   soundName_ = strName.str();

   SdlAudio::addSound(strPath.str(), soundName_);

   file.close();
   status_ = ANIM_IDLE;
}

/*
=====================================
AnimSequence::start
=====================================
*/
int AnimSequence::start() {
/*
   Only a single animation may play at a time on any one item.
   prevFrame_ is set to ensure that the first frame draws
   on the next update.
*/

   if (status_ == ANIM_PLAYING) { return 0; }

   long int now = glutGet(GLUT_ELAPSED_TIME);
   double sNow = static_cast<double>(now) / 1000.0;
   prevFrame_ = sNow - (1.0 / static_cast<double>(frameRate_));

   currentFrame_ = 0;
   status_ = ANIM_PLAYING;

   if (soundName_[0] != '\0') SdlAudio::playSound(soundName_);

   return 1;
}

/*
=====================================
AnimSequence::update
   Set up the item for next frame if frame is due for rendering
=====================================
*/
void AnimSequence::update(boost::shared_ptr<Item> item, boost::shared_ptr<Grid> grid) {
   if (!status_ == ANIM_PLAYING) return;
   if (!grid) throw xBadParameter("Invalid NULL pointer argument", __FILE__, __LINE__);

   if (currentFrame_ <= nFrames_) {
      static long int now;
      now = glutGet(GLUT_ELAPSED_TIME);
      double sNow = static_cast<double>(now) / 1000.0;

      double diff = (sNow - prevFrame_) - (1.0 / static_cast<double>(frameRate_));
      if (diff >= 0.0) {
         prevFrame_ = sNow - diff;

         if (currentFrame_ == nFrames_) {
            status_ = ANIM_FINISHED;
            return;
         }

         if (frame_[currentFrame_].imgOffset[0] < 0 || frame_[currentFrame_].imgOffset[1] < 0
            || frame_[currentFrame_].imgOffset[0] + frame_[currentFrame_].imgDimension[0] > item->getImage()->getDimensions()[0]
            || frame_[currentFrame_].imgOffset[1] + frame_[currentFrame_].imgDimension[1] > item->getImage()->getDimensions()[1])
               throw xException("Frame's bitmap offset out of range", __FILE__, __LINE__);

         int x = item->getPosition()[0] + frame_[currentFrame_].changeInPos[0];
         int y = item->getPosition()[1] + frame_[currentFrame_].changeInPos[1];
         item->setPosition(x, y, grid);
         item->setOffset(frame_[currentFrame_].imgOffset[0], frame_[currentFrame_].imgOffset[1]);
         item->setDimensions(frame_[currentFrame_].imgDimension[0], frame_[currentFrame_].imgDimension[1]);

         currentFrame_++;
      }
   }
}

/*
=====================================
AnimSequence::~AnimSequence
=====================================
*/
AnimSequence::~AnimSequence() {
   if (frame_) {
      delete[] frame_;
      frame_ = NULL;
   }
}
