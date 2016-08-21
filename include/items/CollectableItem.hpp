/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#ifndef __COLLECTABLE_ITEM_HPP__
#define __COLLECTABLE_ITEM_HPP__


#include "items/Item.hpp"
#include "Grid.hpp"
#include "Player.hpp"


class CollectableItem : public virtual Item {
   public:
      CollectableItem() : Item(), value_(0), inventoryItem_(false) {}
      CollectableItem(const Item& item) : Item(item), value_(0), inventoryItem_(false) {}

      inline virtual void hitFromLeft(boost::shared_ptr<Grid> grid, boost::shared_ptr<Item> item);
      inline virtual void hitFromTop(pGrid_t grid, pItem_t player);
      inline virtual void hitFromRight(pGrid_t grid, pItem_t player);
      inline virtual void hitFromBottom(pGrid_t grid, pItem_t player);

      virtual void collect(pGrid_t grid, pItem_t player);

      virtual ~CollectableItem() = 0;

   protected:
      inline int getValue() const;
      inline bool isInventoryItem() const;

      inline void setValue(int val);
      inline void setAsInventoryItem(bool b);

   private:
      int value_;
      bool inventoryItem_;
};

inline void CollectableItem::hitFromLeft(boost::shared_ptr<Grid> grid, boost::shared_ptr<Item> player) { collect(grid, player); }

inline void CollectableItem::hitFromTop(pGrid_t grid, pItem_t player) { collect(grid, player); }

inline void CollectableItem::hitFromRight(pGrid_t grid, pItem_t player) { collect(grid, player); }

inline void CollectableItem::hitFromBottom(pGrid_t grid, pItem_t player) { collect(grid, player); }

inline int CollectableItem::getValue() const { return value_; }

inline bool CollectableItem::isInventoryItem() const { return inventoryItem_; }

inline void CollectableItem::setValue(int val) { value_ = val; }

inline void CollectableItem::setAsInventoryItem(bool b) { inventoryItem_ = b; }


#endif /*!__COLLECTABLE_ITEM_HPP__*/
