/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#include "ItemFactory.hpp"


bool ItemFactory::init_ = false;
std::vector<std::string> ItemFactory::itemNames_ = std::vector<std::string>();
itemList_t ItemFactory::items_ = itemList_t();
itemList_t ItemFactory::itemTypes_ = itemList_t();

/*
=====================================
ItemFactory::init
=====================================
*/
void ItemFactory::init() {
   itemNames_.clear();

   itemNames_.push_back(std::string("roundRock"));
   itemNames_.push_back(std::string("soil"));
   itemNames_.push_back(std::string("squareRock"));
   itemNames_.push_back(std::string("enemy"));
   itemNames_.push_back(std::string("redGem"));
   itemNames_.push_back(std::string("greenGem"));
   itemNames_.push_back(std::string("blueGem"));
   itemNames_.push_back(std::string("redKey"));
   itemNames_.push_back(std::string("greenKey"));
   itemNames_.push_back(std::string("blueKey"));
   itemNames_.push_back(std::string("redDoor"));
   itemNames_.push_back(std::string("greenDoor"));
   itemNames_.push_back(std::string("blueDoor"));
   itemNames_.push_back(std::string("wall"));
   itemNames_.push_back(std::string("hardWall"));
   itemNames_.push_back(std::string("exit"));
   itemNames_.push_back(std::string("man"));
   itemNames_.push_back(std::string("bomb"));
   itemNames_.push_back(std::string("pistol1"));
   itemNames_.push_back(std::string("pistol2"));
   itemNames_.push_back(std::string("pistol3"));
   itemNames_.push_back(std::string("dynamite"));
   itemNames_.push_back(std::string("dynamite_lit"));

   init_ = true;
}

/*
=====================================
ItemFactory::createItem
=====================================
*/
pItem_t ItemFactory::createItem(const std::string& kind) {
   check();

   pItem_t item;

   try {
      if (kind.compare("roundRock") == 0) {
         item = pItem_t(new RoundRock());
      }
      else if (kind.compare("soil") == 0) {
         item = pItem_t(new Soil());
      }
      else if (kind.compare("squareRock") == 0) {
         item = pItem_t(new SquareRock());
      }
      else if (kind.compare("enemy") == 0) {
         item = pItem_t(new Enemy());
      }
      else if (kind.compare("redGem") == 0) {
         item = pItem_t(new Gem(1));
      }
      else if (kind.compare("greenGem") == 0) {
         item = pItem_t(new Gem(2));
      }
      else if (kind.compare("blueGem") == 0) {
         item = pItem_t(new Gem(3));
      }
      else if (kind.compare("redKey") == 0) {
         item = pItem_t(new Key(RED_KEY));
      }
      else if (kind.compare("greenKey") == 0) {
         item = pItem_t(new Key(GREEN_KEY));
      }
      else if (kind.compare("blueKey") == 0) {
         item = pItem_t(new Key(BLUE_KEY));
      }
      else if (kind.compare("redDoor") == 0) {
         item = pItem_t(new Door(RED_DOOR));
      }
      else if (kind.compare("greenDoor") == 0) {
         item = pItem_t(new Door(GREEN_DOOR));
      }
      else if (kind.compare("blueDoor") == 0) {
         item = pItem_t(new Door(BLUE_DOOR));
      }
      else if (kind.compare("wall") == 0) {
         item = pItem_t(new Wall());
      }
      else if (kind.compare("hardWall") == 0) {
         item = pItem_t(new HardWall());
      }
      else if (kind.compare("exit") == 0) {
         item = pItem_t(new Exit());
      }
      else if (kind.compare("man") == 0) {
         item = pItem_t(new Man());
      }
      else if (kind.compare("bomb") == 0) {
         item = pItem_t(new Bomb());
      }
      else if (kind.compare("pistol1") == 0) {
         item = pItem_t(new Pistol(1));
      }
      else if (kind.compare("pistol2") == 0) {
         item = pItem_t(new Pistol(2));
      }
      else if (kind.compare("pistol3") == 0) {
         item = pItem_t(new Pistol(3));
      }
      else if (kind.compare("dynamite") == 0) {
         item = pItem_t(new Dynamite());
      }
      else if (kind.compare("dynamite_lit") == 0) {
         item = pItem_t(new Dynamite_lit());
      }
      else
         throw xBadParameter("Could not create item", __FILE__, __LINE__);
   }
   catch (std::bad_alloc&) {
      throw xMemAllocFail("Could not create Item", __FILE__, __LINE__);
   }

   return item;
}

/*
=====================================
ItemFactory::createItem
=====================================
*/
pItem_t ItemFactory::createItem(const std::string& kind, const Item& itm) {
   check();

   pItem_t item;

   try {
      if (kind.compare("roundRock") == 0) {
         item = pItem_t(new RoundRock(itm));
      }
      else if (kind.compare("soil") == 0) {
         item = pItem_t(new Soil(itm));
      }
      else if (kind.compare("squareRock") == 0) {
         item = pItem_t(new SquareRock(itm));
      }
      else if (kind.compare("enemy") == 0) {
         item = pItem_t(new Enemy(itm));
      }
      else if (kind.compare("redGem") == 0) {
         item = pItem_t(new Gem(itm, 1));
      }
      else if (kind.compare("greenGem") == 0) {
         item = pItem_t(new Gem(itm, 2));
      }
      else if (kind.compare("blueGem") == 0) {
         item = pItem_t(new Gem(itm, 3));
      }
      else if (kind.compare("redKey") == 0) {
         item = pItem_t(new Key(itm, RED_KEY));
      }
      else if (kind.compare("greenKey") == 0) {
         item = pItem_t(new Key(itm, GREEN_KEY));
      }
      else if (kind.compare("blueKey") == 0) {
         item = pItem_t(new Key(itm, BLUE_KEY));
      }
      else if (kind.compare("redDoor") == 0) {
         item = pItem_t(new Door(itm, RED_DOOR));
      }
      else if (kind.compare("greenDoor") == 0) {
         item = pItem_t(new Door(itm, GREEN_DOOR));
      }
      else if (kind.compare("blueDoor") == 0) {
         item = pItem_t(new Door(itm, BLUE_DOOR));
      }
      else if (kind.compare("wall") == 0) {
         item = pItem_t(new Wall(itm));
      }
      else if (kind.compare("hardWall") == 0) {
         item = pItem_t(new HardWall(itm));
      }
      else if (kind.compare("exit") == 0) {
         item = pItem_t(new Exit(itm));
      }
      else if (kind.compare("man") == 0) {
         item = pItem_t(new Man(itm));
      }
      else if (kind.compare("bomb") == 0) {
         item = pItem_t(new Bomb(itm));
      }
      else if (kind.compare("pistol1") == 0) {
         item = pItem_t(new Pistol(itm, 1));
      }
      else if (kind.compare("pistol2") == 0) {
         item = pItem_t(new Pistol(itm, 2));
      }
      else if (kind.compare("pistol3") == 0) {
         item = pItem_t(new Pistol(itm, 3));
      }
      else if (kind.compare("dynamite") == 0) {
         item = pItem_t(new Dynamite(itm));
      }
      else if (kind.compare("dynamite_lit") == 0) {
         item = pItem_t(new Dynamite_lit(itm));
      }
      else
         throw xBadParameter("Could not create item", __FILE__, __LINE__);
   }
   catch (std::bad_alloc&) {
      throw xMemAllocFail("Could not create Item", __FILE__, __LINE__);
   }

   return item;
}

/*
=====================================
ItemFactory::loadItemTypes
=====================================
*/
void ItemFactory::loadItemTypes() {
   check();

   for (unsigned int i = 0; i < itemNames_.size(); i++) {
      itemTypes_.push_back(ItemFactory::createItem(itemNames_[i]));
      itemTypes_[i]->loadSettings();
   }
}

/*
=====================================
ItemFactory::eraseItem
=====================================
*/
void ItemFactory::eraseItem(pItem_t item) {
   check();

   for (unsigned int i = 0; i < items_.size(); i++) {
      if (item == items_[i])
         items_.erase(items_.begin() + i);
   }
}

/*
=====================================
ItemFactory::getItems
=====================================
*/
itemList_t ItemFactory::getItems(const std::string& kind) {
   check();

   itemList_t list;

   for (unsigned int i = 0; i < items_.size(); i++) {
      if (items_[i]->getType().compare(kind) == 0) list.push_back(items_[i]);
   }

   return list;
}