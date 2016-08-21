/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#ifndef __MAN_HPP__
#define __MAN_HPP__


#include "items/ExplosiveItem.hpp"
#include "items/HeavyItem.hpp"


class Man : public ExplosiveItem {
   public:
      Man() : Item() { init(); }
      Man(const Item& item) : Item(item) { init(); }

      void loadSettings();

      virtual void squash(pGrid_t grid);
      virtual void burn(pGrid_t grid);

   private:
      void init();
};


#endif /*!__MAN_HPP__*/
