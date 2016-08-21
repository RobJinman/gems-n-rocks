/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#ifndef __EXPLOSIVE_ITEM_HPP__
#define __EXPLOSIVE_ITEM_HPP__


#include "items/Item.hpp"
#include "Grid.hpp"


class ExplosiveItem : public virtual Item {
   public:
      ExplosiveItem() : Item() {}
      ExplosiveItem(const Item& item) : Item(item) {}

      virtual void squash(pGrid_t grid);
      inline virtual void burn(pGrid_t grid);

      virtual ~ExplosiveItem() = 0;

   protected:
      virtual void explode(pGrid_t grid);
      inline void setContainedItems(const std::vector<std::string>& vec);

   private:
      std::vector<std::string> containedItems_;
};

inline void ExplosiveItem::squash(pGrid_t grid) { explode(grid); }

inline void ExplosiveItem::burn(pGrid_t grid) { explode(grid); }

inline void ExplosiveItem::setContainedItems(const std::vector<std::string>& vec) { containedItems_.assign(vec.begin(), vec.end()); }


#endif /*!__EXPLOSIVE_ITEM_HPP__*/
