/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#include <fstream>
#include <sstream>
#include <string>
#include <queue>
#include <boost/shared_ptr.hpp>

#include "items/Item.hpp"
#include "AnimSequence.hpp"
#include "Image.hpp"
#include "Grid.hpp"
#include "xException.hpp"


int Item::theme_ = 1;
std::string Item::appPath = std::string();

Item::Item() { init(); }
void Item::clearPendingAnims() { for (unsigned int i = 0; i < pendingAnims_.size(); i++) pendingAnims_.pop(); }

/*
=====================================
Item::Item
=====================================
*/
Item::Item(const Item& item) {
   pos_[0] = item.pos_[0];
   pos_[1] = item.pos_[1];
   visible_ = item.visible_;
   equippable_ = item.equippable_;
   collectable_ = item.collectable_;
   stackable_ = item.stackable_;
   immaterial_ = item.immaterial_;
   strength_ = item.strength_;
   currentlyPlaying_ = item.currentlyPlaying_;
   pendingDeletion_ = item.pendingDeletion_;
   image_ = item.image_;
   dimension_[0] = item.dimension_[0];
   dimension_[1] = item.dimension_[1];
   offset_[0] = item.offset_[0];
   offset_[1] = item.offset_[1];
   renderPass_ = item.renderPass_;
   type_.assign(item.type_);

   try {
      for (unsigned int i = 0; i < item.animations_.size(); i++)
         animations_.push_back(boost::shared_ptr<AnimSequence>(new AnimSequence(*item.animations_[i])));
   }
   catch (std::bad_alloc&) {
      throw xMemAllocFail("Could not create Item", __FILE__, __LINE__);
   }
}

/*
=====================================
Item::init
=====================================
*/
void Item::init() {
   type_ = std::string("");
   pos_[0] = 0; pos_[1] = 0;
   setOffset(0, 0);
   setDimensions(0, 0);
   renderPass_ = FIRST_PASS;
   pendingDeletion_ = false;
   visible_ = true;
   collectable_ = false;
   equippable_ = false;
   stackable_ = false;
   immaterial_ = false;
   strength_ = 1;
   image_.reset();
   currentlyPlaying_.reset();
}

/*
=====================================
Item::isIdle
=====================================
*/
bool Item::isIdle() const {
   if (!currentlyPlaying_ && pendingAnims_.size() == 0)
      return true;

   return false;
}

/*
=====================================
Item::getAnimation
=====================================
*/
const boost::shared_ptr<AnimSequence> Item::getAnimation(const std::string& name) const {
   for (unsigned int i = 0; i < animations_.size(); i++) {
      if (animations_[i]->getName().compare(name) == 0)
         return animations_[i];
   }
   throw xException("No animation with name '" + name + "'", __FILE__, __LINE__);
}

/*
=====================================
Item::queueAnim
=====================================
*/
void Item::queueAnim(const char* name) {
   if (isIdle()) {
      if (getAnimation(name)->start())
         currentlyPlaying_ = getAnimation(name);
   }
   else
      pendingAnims_.push(getAnimation(name));
}

/*
=====================================
Item::loadFromFile
=====================================
*/
void Item::loadFromFile(const std::string& path) {
   try {
      std::stringstream str;
      str << Item::appPath << "/" << path;

      std::ifstream file(str.str().data());
      char buffer[128];
      for (int i = 0; i < 128; i++) buffer[i] = '\0';

      if (!file.is_open()) throw xFileNotFound("Could not open item data file", __FILE__, __LINE__, str.str());

      int imW, imH, itW, itH;
      char name[32], imPath[64];

      file.getline(buffer, 128);
      if (sscanf(buffer, "%s %s %d %d %d %d", name, imPath, &imW, &imH, &itW, &itH) != 6)
         throw xBadFile("Could not read from item data file", __FILE__, __LINE__, str.str());

      type_.assign(name);
      setDimensions(itW, itH);

      std::stringstream imStr;
      imStr << Item::appPath << "/" << imPath;
      boost::shared_ptr<Image> image(new Image(imStr.str(), imW, imH));
      setImage(image);

      int nAnims;

      file.getline(buffer, 128);
      if (sscanf(buffer, "%d", &nAnims) != 1)
         throw xBadFile("Could not read from item data file", __FILE__, __LINE__, str.str());

      for (int a = 0; a < nAnims; a++) {
         char animPath[64];

         file.getline(buffer, 128);
         if (sscanf(buffer, "%s", animPath) != 1)
            throw xBadFile("Could not read from item data file", __FILE__, __LINE__, str.str());

         boost::shared_ptr<AnimSequence> anim = boost::shared_ptr<AnimSequence>(new AnimSequence());

         std::stringstream animStr;
         animStr << Item::appPath << "/" << animPath;

         anim->loadFromFile(animStr.str());
         addAnimation(anim);
      }

      file.close();
   }
   catch (std::bad_alloc&) {
      throw xMemAllocFail("Error loading item from file", __FILE__, __LINE__);
   }
}

/*
=====================================
Item::SetPosition
=====================================
*/
void Item::setPosition(int x, int y, boost::shared_ptr<Grid> g) {
   g->removeItem(shared_from_this(), centre()[0] / g->getBlockSize(), centre()[1] / g->getBlockSize());
   pos_[0] = x; pos_[1] = y;
   g->addItem(shared_from_this(), centre()[0] / g->getBlockSize(), centre()[1] / g->getBlockSize());
}

/*
=====================================
Item::update
   If an animation finishes during this update, it's name is returned.
=====================================
*/
std::string Item::update(boost::shared_ptr<Grid> grid) {
   std::string str = std::string();

   if (currentlyPlaying_) {
      currentlyPlaying_->update(shared_from_this(), grid);

      if (currentlyPlaying_->status() == ANIM_FINISHED) {
         currentlyPlaying_->setStatus(ANIM_IDLE);
         str = currentlyPlaying_->getName();

         if (currentlyPlaying_->vanishes()) { // If the object is meant to vanish after the final frame
            grid->removeItem(shared_from_this());
            deleteMe();
            setVisible(false);
            setImmaterial(true);
         }

         currentlyPlaying_.reset();   // Set to NULL

         if (pendingAnims_.size() > 0) {
            if (pendingAnims_.front()->start()) {
               currentlyPlaying_ = pendingAnims_.front();
               pendingAnims_.pop();
            }
         }

      }
   }

   return str;
}

/*
=====================================
Item::doIdle
=====================================
*/
void Item::doIdle(boost::shared_ptr<Grid> grid) {
   update(grid);
}

/*
=====================================
Item::playAnimation
=====================================
*/
int Item::playAnimation(const char* name) {
   clearPendingAnims();
   for (unsigned int i = 0; i < animations_.size(); i++) {
      if (animations_[i]->getName().compare(name) == 0) {
         if (!currentlyPlaying_) {
            if (animations_[i]->start()) {
               currentlyPlaying_ = animations_[i];
               return 1;
            }
            else
               return 0;
         }
         else
            return 0;
      }
   }
   throw xBadParameter("Could not find animation", __FILE__, __LINE__);
}

/*
=====================================
Item::stopAnimations
=====================================
*/
void Item::stopAnimations() {
   clearPendingAnims();
   if (currentlyPlaying_) currentlyPlaying_->stop();
   currentlyPlaying_.reset();
}
