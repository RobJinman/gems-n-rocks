/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#ifndef __EXIT_HPP__
#define __EXIT_HPP__


#include "items/Item.hpp"


class Exit : public Item {
   public:
      Exit() : Item() { init(); }
      Exit(const Item& item) : Item(item) { init(); }

      void loadSettings();

   private:
      void init();
};


#endif /*!__EXIT_HPP__*/
