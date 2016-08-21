/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#include "items/Enemy.hpp"
#include "ItemFactory.hpp"


Enemy::Enemy() : Item(), player_(pPlayer_t()) { init(); }
Enemy::Enemy(pPlayer_t player) : Item(), player_(player) { init(); }
Enemy::Enemy(const Item& item) : Item(item), player_(pPlayer_t()) { init(); }

/*
=====================================
Enemy::loadSettings
=====================================
*/
void Enemy::loadSettings() {
   std::stringstream str;
   str << "data/theme" << theme_ << "/enemy.itm";
   loadFromFile(str.str());
}

/*
=====================================
Enemy::init
=====================================
*/
void Enemy::init() {
   setCollectable(false);
   setStackable(true);
   setImmaterial(false);
   setPass(MIDDLE_PASS);

   std::vector<std::string> vec;

   vec.push_back("greenGem");
   vec.push_back("redGem");
   vec.push_back("redGem");
   vec.push_back("redGem");
   vec.push_back("redGem");

   setContainedItems(vec);

   facing_ = DIR_LEFT;
}

/*
=====================================
Enemy::doIdle
=====================================
*/
void Enemy::doIdle(pGrid_t grid) {
   update(grid);

   if (!isVisible()) return;

   if (player_ ) {
      // Kill both enemy and player on contact
      if (player_->isAlive()) {
         int dx = player_->getPosition()[0] - getPosition()[0];
         int dy = player_->getPosition()[1] - getPosition()[1];
         if (dx * dx + dy * dy <= grid->getBlockSize() * grid->getBlockSize()) {
            player_->kill();
            explode(grid);
         }
      }
   }

   if (!isIdle()) return;

   itemList_t u;
   itemList_t r;;
   itemList_t d;
   itemList_t l;

   int x = centre()[0];
   int y = centre()[1];
   int gX = x / grid->getBlockSize();
   int gY = y / grid->getBlockSize();

   if (gY < grid->getDimensions()[1] - 1)    u = grid->getItems(gX, gY + 1);
   if (gX < grid->getDimensions()[0] - 1)    r = grid->getItems(gX + 1, gY);
   if (gY > 0)                               d = grid->getItems(gX, gY - 1);
   if (gX > 0)                               l = grid->getItems(gX - 1, gY);

   bool u_free = true;
   bool l_free = true;
   bool d_free = true;
   bool r_free = true;

   for (unsigned int i = 0; i < u.size(); i++) if (!u[i]->isImmaterial()) u_free = false;
   for (unsigned int i = 0; i < l.size(); i++) if (!l[i]->isImmaterial()) l_free = false;
   for (unsigned int i = 0; i < d.size(); i++) if (!d[i]->isImmaterial()) d_free = false;
   for (unsigned int i = 0; i < r.size(); i++) if (!r[i]->isImmaterial()) r_free = false;

   switch (facing_) {
      case DIR_LEFT: {
         if (u_free && gY < grid->getDimensions()[1] - 1) {
            queueAnim("spin_lu");
            queueAnim("run_up");
            grid->addItem(shared_from_this(), gX, gY + 1);
            facing_ = DIR_UP;
            break;
         }
         if (l_free && gX > 0) {
            queueAnim("run_left");
            grid->addItem(shared_from_this(), gX - 1, gY);
            facing_ = DIR_LEFT;
            break;
         }
         if (d_free && gY > 0) {
            queueAnim("spin_ld");
            queueAnim("run_down");
            grid->addItem(shared_from_this(), gX, gY - 1);
            facing_ = DIR_DOWN;
            break;
         }
         if (r_free && gX < grid->getDimensions()[0] - 1) {
            queueAnim("spin_lu");
            queueAnim("spin_ur");
            queueAnim("run_right");
            grid->addItem(shared_from_this(), gX + 1, gY);
            facing_ = DIR_RIGHT;
         }
      } break;
      case DIR_RIGHT: {
         if (d_free && gY > 0) {
            queueAnim("spin_rd");
            queueAnim("run_down");
            grid->addItem(shared_from_this(), gX, gY - 1);
            facing_ = DIR_DOWN;
            break;
         }
         if (r_free && gX < grid->getDimensions()[0] - 1) {
            queueAnim("run_right");
            grid->addItem(shared_from_this(), gX + 1, gY);
            facing_ = DIR_RIGHT;
            break;
         }
         if (u_free && gY < grid->getDimensions()[1] - 1) {
            queueAnim("spin_ru");
            queueAnim("run_up");
            grid->addItem(shared_from_this(), gX, gY + 1);
            facing_ = DIR_UP;
            break;
         }
         if (l_free && gX > 0) {
            queueAnim("spin_rd");
            queueAnim("spin_dl");
            queueAnim("run_left");
            grid->addItem(shared_from_this(), gX - 1, gY);
            facing_ = DIR_LEFT;
         }
      } break;
      case DIR_UP: {
         if (r_free && gX < grid->getDimensions()[0] - 1) {
            queueAnim("spin_ur");
            queueAnim("run_right");
            grid->addItem(shared_from_this(), gX + 1, gY);
            facing_ = DIR_RIGHT;
            break;
         }
         if (u_free && gY < grid->getDimensions()[1] - 1) {
            queueAnim("run_up");
            grid->addItem(shared_from_this(), gX, gY + 1);
            facing_ = DIR_UP;
            break;
         }
         if (l_free && gX > 0) {
            queueAnim("spin_ul");
            queueAnim("run_left");
            grid->addItem(shared_from_this(), gX - 1, gY);
            facing_ = DIR_LEFT;
            break;
         }
         if (d_free && gY > 0) {
            queueAnim("spin_ur");
            queueAnim("spin_rd");
            queueAnim("run_down");
            grid->addItem(shared_from_this(), gX, gY - 1);
            facing_ = DIR_DOWN;
         }
      } break;
      case DIR_DOWN: {
         if (l_free && gX > 0) {
            queueAnim("spin_dl");
            queueAnim("run_left");
            grid->addItem(shared_from_this(), gX - 1, gY);
            facing_ = DIR_LEFT;
            break;
         }
         if (d_free && gY > 0) {
            queueAnim("run_down");
            grid->addItem(shared_from_this(), gX, gY - 1);
            facing_ = DIR_DOWN;
            break;
         }
         if (r_free && gX < grid->getDimensions()[0] - 1) {
            queueAnim("spin_dr");
            queueAnim("run_right");
            grid->addItem(shared_from_this(), gX + 1, gY);
            facing_ = DIR_RIGHT;
            break;
         }
         if (u_free && gY < grid->getDimensions()[1] - 1) {
            queueAnim("spin_dr");
            queueAnim("spin_ru");
            queueAnim("run_up");
            grid->addItem(shared_from_this(), gX, gY + 1);
            facing_ = DIR_UP;
         }
      } break;
   }
}
