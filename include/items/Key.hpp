/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#ifndef __KEY_HPP__
#define __KEY_HPP__


#include "items/Item.hpp"
#include "items/CollectableItem.hpp"
#include "items/HeavyItem.hpp"
#include "Grid.hpp"


typedef enum { RED_KEY, GREEN_KEY, BLUE_KEY } keyColour_t;

class Key : public CollectableItem {
   public:
      Key(keyColour_t kind);
      Key(const Item& item, keyColour_t kind);

      inline keyColour_t getKind() const;

      void loadSettings();

   private:
      keyColour_t kind_;

      void init();
};

inline keyColour_t Key::getKind() const { return kind_; }


#endif /*!__KEY_HPP__*/
