/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */


#include "items/CollectableItem.hpp"
#include "Grid.hpp"


/*
=====================================
CollectableItem::collect
=====================================
*/
void CollectableItem::collect(pGrid_t grid, pItem_t item) {
   pPlayer_t player = boost::dynamic_pointer_cast<Player>(item);

   if (!player) throw xException("Bad Cast - expected Player item", __FILE__, __LINE__);

   stopAnimations();

   if (playAnimation("collect")) {
      player->setScore(player->getScore() + value_);

      if (inventoryItem_)
         player->addToInventory(shared_from_this());
   }
}

/*
=====================================
CollectableItem::~CollectableItem
=====================================
*/
CollectableItem::~CollectableItem() {}
