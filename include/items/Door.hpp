/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#ifndef __DOOR_HPP__
#define __DOOR_HPP__


#include "items/Item.hpp"


typedef enum { RED_DOOR, GREEN_DOOR, BLUE_DOOR } doorColour_t;

class Door : public Item {
   public:
      Door(doorColour_t kind) : Item(), kind_(kind) { init(); }
      Door(const Item& item, doorColour_t kind) : Item(item), kind_(kind) { init(); }

      void loadSettings();

   private:
      doorColour_t kind_;

      void init();
};


#endif /*!__DOOR_HPP__*/
