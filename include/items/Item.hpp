/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#ifndef __ITEM_HPP__
#define __ITEM_HPP__


#include <vector>
#include <queue>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "Image.hpp"
#include "AnimSequence.hpp"


class Grid;
class Item;

typedef boost::shared_ptr<Item> pItem_t;
typedef std::vector<pItem_t> itemList_t;

typedef enum { FIRST_PASS, MIDDLE_PASS, FINAL_PASS } renderPass_t;

class Item : public boost::enable_shared_from_this<Item> {
   public:
      Item();
      Item(const Item& item);

      void init();

      virtual void hitFromLeft(boost::shared_ptr<Grid> grid, boost::shared_ptr<Item> item) {}
      virtual void hitFromTop(boost::shared_ptr<Grid> grid, boost::shared_ptr<Item> item) {}
      virtual void hitFromRight(boost::shared_ptr<Grid> grid, boost::shared_ptr<Item> item) {}
      virtual void hitFromBottom(boost::shared_ptr<Grid> grid, boost::shared_ptr<Item> item) {}
      virtual void squash(boost::shared_ptr<Grid> grid) {}
      virtual void burn(boost::shared_ptr<Grid> grid) {}
      virtual void doIdle(boost::shared_ptr<Grid> grid);

      virtual void loadSettings() = 0;  // Should call loadFromFile

      void stopAnimations();

      inline void setImmaterial(bool b);
      void setPosition(int x, int y, boost::shared_ptr<Grid> g);
      inline void setImage(boost::shared_ptr<Image> image);
      inline void addAnimation(boost::shared_ptr<AnimSequence> animation);
      inline void setOffset(int x, int y);
      inline void setDimensions(int w, int h);
      inline void setVisible(bool vis);

      inline bool isVisible() const;
      inline bool isEquippable() const;
      inline bool isCollectable() const;
      inline bool isStackable() const;
      inline bool isImmaterial() const;
      inline int getStrength() const;
      inline const int* getPosition() const;
      inline const int* centre();
      inline boost::shared_ptr<Image> getImage() const;
      inline renderPass_t getPass() const;
      inline const int* getOffset() const;
      inline const int* getDimensions() const;
      inline const std::string getType() const;
      inline bool pendingDeletion() const;

      const boost::shared_ptr<AnimSequence> getAnimation(const std::string& name) const;
      inline AnimStatus getAnimStatus(const char* name) const;
      int playAnimation(const char* name);
      void queueAnim(const char* name);
      void clearPendingAnims();
      inline const boost::shared_ptr<AnimSequence> getCurrentlyPlaying() const;
      bool isIdle() const;

      inline static void setTheme(int t);
      inline static void setAppPath(const std::string& path);

      virtual ~Item() {}

   protected:
      static std::string appPath;
      static int theme_;

      virtual void loadFromFile(const std::string& path);

      inline void setCollectable(bool b);
      inline void setStackable(bool b);
      inline void setPass(renderPass_t p);
      inline void setStrength(int i);
      inline void setEquippable(bool b);

      std::string update(boost::shared_ptr<Grid> grid);
      inline void deleteMe();

   private:
      bool equippable_;
      bool collectable_;
      bool stackable_;
      bool immaterial_;
      int strength_;
      bool pendingDeletion_;

      int                                             pos_[2];
      int                                             centre_[2];
      boost::shared_ptr<Image>                        image_;
      int                                             dimension_[2];
      int                                             offset_[2];
      renderPass_t                                    renderPass_;
      std::vector<boost::shared_ptr<AnimSequence> >   animations_;
      std::queue<boost::shared_ptr<AnimSequence> >    pendingAnims_;
      boost::shared_ptr<AnimSequence>                 currentlyPlaying_;
      std::string                                     type_;
      bool                                            visible_;
};

inline void Item::setTheme(int t) { theme_ = t; }

inline void Item::setAppPath(const std::string& path) { appPath = path; }



inline void Item::deleteMe() { pendingDeletion_ = true; }



inline void Item::setImmaterial(bool b) { immaterial_ = b; }

inline void Item::setImage(boost::shared_ptr<Image> image) { image_ = image; }

inline void Item::setPass(renderPass_t p) { renderPass_ = p; }

inline void Item::setOffset(int x, int y) { offset_[0] = x; offset_[1] = y; }

inline void Item::setDimensions(int w, int h) { dimension_[0] = w; dimension_[1] = h; }

inline void Item::setVisible(bool vis) { visible_ = vis; }

inline void Item::setCollectable(bool b) { collectable_ = b; }

inline void Item::setStackable(bool b) { stackable_ = b; }

inline void Item::setStrength(int i) { strength_ = i; }

inline void Item::setEquippable(bool b) { equippable_ = b; }



inline bool Item::isVisible() const { return visible_; }

inline bool Item::isCollectable() const { return collectable_; }

inline bool Item::isStackable() const { return stackable_; }

inline bool Item::isImmaterial() const { return immaterial_; }

inline int Item::getStrength() const { return strength_; }

inline bool Item::isEquippable() const { return equippable_; }

inline bool Item::pendingDeletion() const { return pendingDeletion_; }

inline const int* Item::getPosition() const { return pos_; }

inline boost::shared_ptr<Image> Item::getImage() const { return image_; }

inline renderPass_t Item::getPass() const { return renderPass_; }

inline const int* Item::getOffset() const { return offset_; }

inline const int* Item::getDimensions() const { return dimension_; }

inline const std::string Item::getType() const { return type_; }

inline const int* Item::centre() {
   centre_[0] = pos_[0] + dimension_[0] / 2;
   centre_[1] = pos_[1] + dimension_[1] / 2;

   return centre_;
}



inline void Item::addAnimation(boost::shared_ptr<AnimSequence> animation) { animations_.push_back(animation); }

inline AnimStatus Item::getAnimStatus(const char* name) const { return getAnimation(name)->status(); }

inline const boost::shared_ptr<AnimSequence> Item::getCurrentlyPlaying() const { return currentlyPlaying_; }


#endif /*!__ITEM_HPP__*/
