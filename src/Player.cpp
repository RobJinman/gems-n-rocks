/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#include <vector>
#include <sstream>

#include "xException.hpp"
#include "Player.hpp"
#include "InfoBar.hpp"
#include "ItemFactory.hpp"
#include "Grid.hpp"


/*
=====================================
Player::Player
=====================================
*/
Player::Player(Item& item, boost::shared_ptr<Grid> grid) : Item(item) {
   grid_ = grid;
   info_.reset();
   equipped_.reset();
   imgEquipped_.reset();
   score_ = 0;
   alive_ = true;
}

/*
=====================================
Player::setInfo
=====================================
*/
void Player::setInfo(boost::shared_ptr<InfoBar> info) {
   info_ = info;
   try {
      std::stringstream str;
      str << Item::appPath << "/data/images/equipped.bmp";
      imgEquipped_ = pImage_t(new Image(str.str(), 32, 32));
      info_->addIcon(imgEquipped_, 0, 0, "equipped");
      info_->getIcon("equipped")->setVisible(false);
   }
   catch (...) {}
}

/*
=====================================
Player::toggleEquipped
=====================================
*/
void Player::toggleEquipped() {
   if (inventory_.size() == 0) {
      if (equipped_) equipped_.reset();
      return;
   }

   int j = 0;
   if (equipped_) {
      for (j = 0; j < inventory_.size(); j++) {
         if (equipped_ == inventory_[j]) {
            j++;
            break;
         }
      }
   }

   j = j % inventory_.size();
   int n = 0;
   while (!inventory_[j]->isEquippable()) {
      if (n > inventory_.size()) return;

      j = (j + 1) % inventory_.size();
      n++;
   }

   equipped_ = inventory_[j];
   int x = info_->getDimensions()[0] - (imgEquipped_->getDimensions()[0] * (j + 1));
   info_->getIcon("equipped")->setPosition(x, 0);
}

/*
=====================================
Player::removeFromInventory
=====================================
*/
void Player::removeFromInventory(pItem_t item) {
   if (item == equipped_) toggleEquipped();

   itemList_t inv;
   inv.assign(inventory_.begin(), inventory_.end());

   for (unsigned int i = 0; i < inv.size(); i++) {
      std::stringstream str;
      str << inv[i].get();
      info_->removeIcon(str.str());

      if (item == inv[i]) {
         inv.erase(inv.begin() + i);
         i--;
      }
   }

   ItemFactory::eraseItem(item);

   int n = 0;
   for (unsigned int i = 0; i < inv.size(); i++) {
      if (inv[i]->isEquippable()) n++;
   }
   if (n == 0) {
      equipped_.reset();
      info_->getIcon("equipped")->setVisible(false);
   }

   inventory_.clear();
   for (unsigned int i = 0; i < inv.size(); i++) {
      addToInventory(inv[i]);

      if (inv[i] == equipped_) {
         int x = info_->getDimensions()[0] - (imgEquipped_->getDimensions()[0] * (i + 1));
         info_->getIcon("equipped")->setPosition(x, 0);
      }
   }
}

/*
=====================================
Player::actionUp
=====================================
*/
void Player::actionUp() {
   if (!isIdle()) return;
   if (!equipped_) return;

   if (equipped_->getType().compare("pistol1") == 0
      || equipped_->getType().compare("pistol2") == 0
      || equipped_->getType().compare("pistol3") == 0)
      shootUp();
   else if (equipped_->getType().compare("dynamite") == 0) {
      int x = centre()[0];
      int y = centre()[1];
      int gX = x / grid_->getBlockSize();
      int gY = y / grid_->getBlockSize();

      if (gY < grid_->getDimensions()[1] - 1) {
         if (isFree(gX, gY + 1)) {
            itemList_t itmTypes = ItemFactory::getItemTypes();

            int j = 0;
            while (itmTypes[j]->getType().compare("dynamite_lit") != 0) {
               j++;
               if (j == itmTypes.size()) throw xException("Item type not found", __FILE__, __LINE__);
            }

            pItem_t itm = ItemFactory::createItem("dynamite_lit", *itmTypes[j]);
            itm->setPosition(gX * grid_->getBlockSize(), (gY + 1) * grid_->getBlockSize(), grid_);
            ItemFactory::addItem(itm);

            playAnimation("layItem_up");
            removeFromInventory(equipped_);
         }
      }
   }
}

/*
=====================================
Player::actionLeft
=====================================
*/
void Player::actionLeft() {
   if (!isIdle()) return;
   if (!equipped_) return;

   if (equipped_->getType().compare("pistol1") == 0
      || equipped_->getType().compare("pistol2") == 0
      || equipped_->getType().compare("pistol3") == 0)
      shootLeft();
   else if (equipped_->getType().compare("dynamite") == 0) {
      int x = centre()[0];
      int y = centre()[1];
      int gX = x / grid_->getBlockSize();
      int gY = y / grid_->getBlockSize();

      if (gX > 0) {
         if (isFree(gX - 1, gY)) {
            itemList_t itmTypes = ItemFactory::getItemTypes();

            int j = 0;
            while (itmTypes[j]->getType().compare("dynamite_lit") != 0) {
               j++;
               if (j == itmTypes.size()) throw xException("Item type not found", __FILE__, __LINE__);
            }

            pItem_t itm = ItemFactory::createItem("dynamite_lit", *itmTypes[j]);
            itm->setPosition((gX - 1) * grid_->getBlockSize(), gY * grid_->getBlockSize(), grid_);
            ItemFactory::addItem(itm);

            playAnimation("layItem_left");
            removeFromInventory(equipped_);
         }
      }
   }
}

/*
=====================================
Player::actionDown
=====================================
*/
void Player::actionDown() {
   if (!isIdle()) return;
   if (!equipped_) return;

   if (equipped_->getType().compare("pistol1") == 0
      || equipped_->getType().compare("pistol2") == 0
      || equipped_->getType().compare("pistol3") == 0)
      shootDown();
   else if (equipped_->getType().compare("dynamite") == 0) {
      int x = centre()[0];
      int y = centre()[1];
      int gX = x / grid_->getBlockSize();
      int gY = y / grid_->getBlockSize();

      if (gY > 0) {
         if (isFree(gX, gY - 1)) {
            itemList_t itmTypes = ItemFactory::getItemTypes();

            int j = 0;
            while (itmTypes[j]->getType().compare("dynamite_lit") != 0) {
               j++;
               if (j == itmTypes.size()) throw xException("Item type not found", __FILE__, __LINE__);
            }

            pItem_t itm = ItemFactory::createItem("dynamite_lit", *itmTypes[j]);
            itm->setPosition(gX * grid_->getBlockSize(), (gY - 1) * grid_->getBlockSize(), grid_);
            ItemFactory::addItem(itm);

            playAnimation("layItem_down");
            removeFromInventory(equipped_);
         }
      }
   }
}

/*
=====================================
Player::actionRight
=====================================
*/
void Player::actionRight() {
   if (!isIdle()) return;
   if (!equipped_) return;

   if (equipped_->getType().compare("pistol1") == 0
      || equipped_->getType().compare("pistol2") == 0
      || equipped_->getType().compare("pistol3") == 0)
      shootRight();
   else if (equipped_->getType().compare("dynamite") == 0) {
      int x = centre()[0];
      int y = centre()[1];
      int gX = x / grid_->getBlockSize();
      int gY = y / grid_->getBlockSize();

      if (gX < grid_->getDimensions()[0] - 1) {
         if (isFree(gX + 1, gY)) {
            itemList_t itmTypes = ItemFactory::getItemTypes();

            int j = 0;
            while (itmTypes[j]->getType().compare("dynamite_lit") != 0) {
               j++;
               if (j == itmTypes.size()) throw xException("Item type not found", __FILE__, __LINE__);
            }

            pItem_t itm = ItemFactory::createItem("dynamite_lit", *itmTypes[j]);
            itm->setPosition((gX + 1) * grid_->getBlockSize(), gY * grid_->getBlockSize(), grid_);
            ItemFactory::addItem(itm);

            playAnimation("layItem_right");
            removeFromInventory(equipped_);
         }
      }
   }
}

/*
=====================================
Player::shootUp
=====================================
*/
void Player::shootUp() {
   boost::shared_ptr<Pistol> pistol = boost::dynamic_pointer_cast<Pistol>(equipped_);
   if (!pistol) return;

   playAnimation("pistol_up");

   int x = centre()[0];
   int y = centre()[1];
   int gridX = x / grid_->getBlockSize();
   int gridY = y / grid_->getBlockSize();

   while (gridY < grid_->getDimensions()[1] - 1) {
      gridY++;

      if (!isFree(gridX, gridY)) {
         itemList_t items = grid_->getItems(gridX, gridY);
         for (unsigned int i = 0; i < items.size(); i++) {
            if (pistol->getPower() >= items[i]->getStrength()) items[i]->burn(grid_);
         }
         break;
      }
   }
}

/*
=====================================
Player::shootDown
=====================================
*/
void Player::shootDown() {
   boost::shared_ptr<Pistol> pistol = boost::dynamic_pointer_cast<Pistol>(equipped_);
   if (!pistol) return;

   playAnimation("pistol_down");

   int x = centre()[0];
   int y = centre()[1];
   int gridX = x / grid_->getBlockSize();
   int gridY = y / grid_->getBlockSize();

   while (gridY > 0) {
      gridY--;

      if (!isFree(gridX, gridY)) {
         itemList_t items = grid_->getItems(gridX, gridY);
         for (unsigned int i = 0; i < items.size(); i++) {
            if (pistol->getPower() >= items[i]->getStrength()) items[i]->burn(grid_);
         }
         break;
      }
   }
}

/*
=====================================
Player::shootLeft
=====================================
*/
void Player::shootLeft() {
   boost::shared_ptr<Pistol> pistol = boost::dynamic_pointer_cast<Pistol>(equipped_);
   if (!pistol) return;

   playAnimation("pistol_left");

   int x = centre()[0];
   int y = centre()[1];
   int gridX = x / grid_->getBlockSize();
   int gridY = y / grid_->getBlockSize();

   while (gridX > 0) {
      gridX--;

      if (!isFree(gridX, gridY)) {
         itemList_t items = grid_->getItems(gridX, gridY);
         for (unsigned int i = 0; i < items.size(); i++) {
            if (pistol->getPower() >= items[i]->getStrength()) items[i]->burn(grid_);
         }
         break;
      }
   }
}

/*
=====================================
Player::shootRight
=====================================
*/
void Player::shootRight() {
   boost::shared_ptr<Pistol> pistol = boost::dynamic_pointer_cast<Pistol>(equipped_);
   if (!pistol) return;

   playAnimation("pistol_right");

   int x = centre()[0];
   int y = centre()[1];
   int gridX = x / grid_->getBlockSize();
   int gridY = y / grid_->getBlockSize();

   while (gridX < grid_->getDimensions()[0] - 1) {
      gridX++;

      if (!isFree(gridX, gridY)) {
         itemList_t items = grid_->getItems(gridX, gridY);
         for (unsigned int i = 0; i < items.size(); i++) {
            if (pistol->getPower() >= items[i]->getStrength()) items[i]->burn(grid_);
         }
         break;
      }
   }
}

/*
=====================================
Player::addToInventory
=====================================
*/
void Player::addToInventory(pItem_t item) {
   inventory_.push_back(item);
   int x = info_->getDimensions()[0] - ((item->getDimensions()[0]) * inventory_.size());

   std::stringstream str;
   str << item.get();

   if (info_) info_->addIcon(item->getImage(), x, 0, str.str());

   if (!equipped_) {
      if (item->isEquippable()) {
         equipped_ = item;
         toggleEquipped();
         info_->getIcon("equipped")->setVisible(true);
      }
   }
}

/*
=====================================
Player::isFree
=====================================
*/
bool Player::isFree(int gridX, int gridY) const {
   itemList_t items = grid_->getItems(gridX, gridY);

   bool spaceFree = true;

   for (unsigned int i = 0; i < items.size(); i++) {
      if (!items[i]->isImmaterial()) {
         bool sf = spaceFree;
         spaceFree = false;

         //-----Doors-----
         if (items[i]->getType().compare("blueDoor") == 0) {
            for (unsigned int j = 0; j < inventory_.size(); j++) {
               if (inventory_[j]->getType().compare("blueKey") == 0) spaceFree = sf;
            }
         }
         if (items[i]->getType().compare("redDoor") == 0) {
            for (unsigned int j = 0; j < inventory_.size(); j++) {
               if (inventory_[j]->getType().compare("redKey") == 0) spaceFree = sf;
            }
         }
         if (items[i]->getType().compare("greenDoor") == 0) {
            for (unsigned int j = 0; j < inventory_.size(); j++) {
               if (inventory_[j]->getType().compare("greenKey") == 0) spaceFree = sf;
            }
         }
         if (items[i]->getType().compare("exit") == 0) {
            if (required_ <= 0) spaceFree = sf;
         }
      }
   }

   return spaceFree;
}

/*
=====================================
Player::isCollectable
   Returns true if there is a collectable item in this space
   no non-collectable, non-immaterial items in the way.
=====================================
*/
bool Player::isCollectable(int gridX, int gridY) const {
   itemList_t items = grid_->getItems(gridX, gridY);

   for (unsigned int i = 0; i < items.size(); i++) {
      if (!items[i]->isCollectable()) {
         if (items[i]->isImmaterial() == false)
            return false;
      }
   }

   return true;
}

/*
=====================================
Player::goUp
=====================================
*/
void Player::goUp() {
   if (!grid_) throw xException("Grid pointer not set", __FILE__, __LINE__);
   if (!isIdle()) return;

   int x = centre()[0];
   int y = centre()[1];
   int gridX = x / grid_->getBlockSize();
   int gridY = y / grid_->getBlockSize();

   if (gridY < grid_->getDimensions()[1] - 1) {

      if (isFree(gridX, gridY + 1)) {
         if (playAnimation("run_up")) {
            grid_->addItem(shared_from_this(), gridX, gridY + 1); // Reserve space on the grid
            return;
         }
      }

      if (isCollectable(gridX, gridY + 1)) {
         playAnimation("dig_up");
         grid_->addItem(shared_from_this(), gridX, gridY + 1);
      }
      grid_->hitFromBottom(gridX, gridY + 1, shared_from_this());
   }
}

/*
=====================================
Player::goLeft
=====================================
*/
void Player::goLeft() {
   if (!grid_) throw xException("Grid pointer not set", __FILE__, __LINE__);
   if (!isIdle()) return;

   int x = centre()[0];
   int y = centre()[1];
   int gridX = x / grid_->getBlockSize();
   int gridY = y / grid_->getBlockSize();

   if (gridX > 0) {

      if (isFree(gridX - 1, gridY)) {
         if (playAnimation("run_left")) {
            grid_->addItem(shared_from_this(), gridX - 1, gridY); // Reserve space on the grid
            return;
         }
      }

      if (isCollectable(gridX - 1, gridY)) {
         playAnimation("dig_left");
         grid_->addItem(shared_from_this(), gridX - 1, gridY);
      }
      grid_->hitFromRight(gridX - 1, gridY, shared_from_this());
   }
}

/*
=====================================
Player::goDown
=====================================
*/
void Player::goDown() {
   if (!grid_) throw xException("Grid pointer not set", __FILE__, __LINE__);
   if (!isIdle()) return;

   int x = centre()[0];
   int y = centre()[1];
   int gridX = x / grid_->getBlockSize();
   int gridY = y / grid_->getBlockSize();

   if (gridY > 0) {

      if (isFree(gridX, gridY - 1)) {
         if (playAnimation("run_down")) {
            grid_->addItem(shared_from_this(), gridX, gridY - 1); // Reserve space on the grid
            return;
         }
      }

      if (isCollectable(gridX, gridY - 1)) {
         playAnimation("dig_down");
         grid_->addItem(shared_from_this(), gridX, gridY - 1);
      }
      grid_->hitFromTop(gridX, gridY - 1, shared_from_this());
   }
}

/*
=====================================
Player::goRight
=====================================
*/
void Player::goRight() {
   if (!grid_) throw xException("Grid pointer not set", __FILE__, __LINE__);
   if (!isIdle()) return;

   int x = centre()[0];
   int y = centre()[1];
   int gridX = x / grid_->getBlockSize();
   int gridY = y / grid_->getBlockSize();

   if (gridX < grid_->getDimensions()[0] - 1) {

      if (isFree(gridX + 1, gridY)) {
         if (playAnimation("run_right")) {
            grid_->addItem(shared_from_this(), gridX + 1, gridY); // Reserve space on the grid
            return;
         }
      }

      if (isCollectable(gridX + 1, gridY)) {
         playAnimation("dig_right");
         grid_->addItem(shared_from_this(), gridX + 1, gridY);
      }
      grid_->hitFromLeft(gridX + 1, gridY, shared_from_this());
   }
}

/*
=====================================
Player::whipUp
=====================================
*/
void Player::whipUp() {
   if (!grid_) throw xException("Grid pointer not set", __FILE__, __LINE__);
   if (!isIdle()) return;

   int x = centre()[0];
   int y = centre()[1];
   int gridX = x / grid_->getBlockSize();
   int gridY = y / grid_->getBlockSize();

   if (gridY < grid_->getDimensions()[1] - 1) {
      playAnimation("whip_up");
      grid_->hitFromBottom(gridX, gridY + 1, shared_from_this());
   }
}

/*
=====================================
Player::whipLeft
=====================================
*/
void Player::whipLeft() {
   if (!grid_) throw xException("Grid pointer not set", __FILE__, __LINE__);
   if (!isIdle()) return;

   int x = centre()[0];
   int y = centre()[1];
   int gridX = x / grid_->getBlockSize();
   int gridY = y / grid_->getBlockSize();

   if (gridX > 0) {
      playAnimation("whip_left");
      grid_->hitFromRight(gridX - 1, gridY, shared_from_this());
   }
}

/*
=====================================
Player::whipDown
=====================================
*/
void Player::whipDown() {
   if (!grid_) throw xException("Grid pointer not set", __FILE__, __LINE__);
   if (!isIdle()) return;

   int x = centre()[0];
   int y = centre()[1];
   int gridX = x / grid_->getBlockSize();
   int gridY = y / grid_->getBlockSize();

   if (gridY > 0) {
      playAnimation("whip_down");
      grid_->hitFromTop(gridX, gridY - 1, shared_from_this());
   }
}

/*
=====================================
Player::whipRight
=====================================
*/
void Player::whipRight() {
   if (!grid_) throw xException("Grid pointer not set", __FILE__, __LINE__);
   if (!isIdle()) return;

   int x = centre()[0];
   int y = centre()[1];
   int gridX = x / grid_->getBlockSize();
   int gridY = y / grid_->getBlockSize();

   if (gridX < grid_->getDimensions()[0] - 1) {
      playAnimation("whip_right");
      grid_->hitFromLeft(gridX + 1, gridY, shared_from_this());
   }
}
