/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#ifndef __DYNAMITE_LIT_HPP__
#define __DYNAMITE_LIT_HPP__


#include <sstream>

#include "items/ExplosiveItem.hpp"
#include "Grid.hpp"


class Dynamite_lit : public ExplosiveItem {
   public:
      Dynamite_lit();
      Dynamite_lit(const Item& item);

      void doIdle(pGrid_t grid);

      void loadSettings();

      virtual ~Dynamite_lit() {}

   private:
      void init();
};


#endif /*!__DYNAMITE_LIT_HPP__*/
