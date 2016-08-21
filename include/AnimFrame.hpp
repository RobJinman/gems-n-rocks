/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#ifndef __ANIM_FRAME_HPP__
#define __ANIM_FRAME_HPP__


class AnimFrame {
   friend class AnimSequence;

   private:
      int changeInPos[2];  // Change in game world coords
      int imgOffset[2];    // Coordinates and dimensions of the section of bitmap
      int imgDimension[2];
};


#endif /*!__ANIM_FRAME_HPP__*/
