/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#ifndef __ITEM_FACTORY_HPP__
#define __ITEM_FACTORY_HPP__


#include <vector>
#include <string>

#include "xException.hpp"
#include "items/Item.hpp"
#include "items/Soil.hpp"
#include "items/RoundRock.hpp"
#include "items/SquareRock.hpp"
#include "items/Enemy.hpp"
#include "items/Gem.hpp"
#include "items/Key.hpp"
#include "items/Door.hpp"
#include "items/Wall.hpp"
#include "items/HardWall.hpp"
#include "items/Exit.hpp"
#include "items/Man.hpp"
#include "items/Bomb.hpp"
#include "items/Pistol.hpp"
#include "items/Dynamite.hpp"
#include "items/Dynamite_lit.hpp"


class ItemFactory {
   public:
      static void init();

      static pItem_t createItem(const std::string& kind);
      static pItem_t createItem(const std::string& kind, const Item& itm);

      inline static void addItem(pItem_t item);
      inline static void addItems(const itemList_t& items);
      inline static const itemList_t& getItems();
      static itemList_t getItems(const std::string& kind);
      static void eraseItem(pItem_t item);
      static void loadItemTypes();
      inline static const itemList_t& getItemTypes();
      inline static void clearItemTypes();
      inline static void clearItems();
      inline static void reset();

      virtual ~ItemFactory() = 0;

   private:
      static inline void check();

      static bool init_;
      static std::vector<std::string> itemNames_;
      static itemList_t items_;
      static itemList_t itemTypes_;
};

inline void ItemFactory::check() { if (!init_) throw xException("ItemFactory not initialised", __FILE__, __LINE__); }

inline void ItemFactory::addItem(pItem_t item) { check(); items_.push_back(item); }

inline void ItemFactory::addItems(const itemList_t& items) { check(); items_.insert(items_.end(), items.begin(), items.end()); }

inline const itemList_t& ItemFactory::getItems() { check(); return items_; }

inline const itemList_t& ItemFactory::getItemTypes() { check(); return itemTypes_; }

inline void ItemFactory::clearItemTypes() { check(); itemTypes_.clear(); }

inline void ItemFactory::clearItems() { check(); items_.clear(); }

inline void ItemFactory::reset() { check(); clearItems(); clearItemTypes(); init_ = false; }


#endif /*!__ITEM_FACTORY_HPP__*/