/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#ifndef __HEAVY_ITEM_HPP__
#define __HEAVY_ITEM_HPP__


#include "items/Item.hpp"
#include "Grid.hpp"


class HeavyItem : public virtual Item {
   public:
      HeavyItem() : Item() {}
      HeavyItem(const Item& item) : Item(item) {}

      virtual void doIdle(pGrid_t grid);

      virtual ~HeavyItem() = 0;

   protected:
      virtual void fall(pGrid_t grid);
};


#endif /*!__HEAVY_ITEM_HPP__*/
