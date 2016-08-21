/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#ifndef __PLAYER_HPP__
#define __PLAYER_HPP__


#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "Grid.hpp"
#include "InfoBar.hpp"
#include "items/Man.hpp"
#include "items/ExplosiveItem.hpp"


class Player;
typedef boost::shared_ptr<Player> pPlayer_t;

class Player : public Man {
   public:
      Player(Item& item, boost::shared_ptr<Grid> grid);

      void setInfo(boost::shared_ptr<InfoBar> info);
      void addToInventory(pItem_t item);
      void removeFromInventory(pItem_t item);
      inline void setGrid(boost::shared_ptr<Grid> grid);
      inline void setRequired(int n);
      inline void setScore(int score);
      void toggleEquipped();
      void kill();

      void goUp();
      void goLeft();
      void goDown();
      void goRight();
      void whipUp();
      void whipLeft();
      void whipDown();
      void whipRight();
      void actionUp();
      void actionLeft();
      void actionDown();
      void actionRight();

      inline void explode(boost::shared_ptr<Grid> grid);

      inline const itemList_t& getInventory() const;
      inline int getScore() const;
      inline bool isAlive() const;

   private:
      int                        score_;
      int                        required_;
      boost::shared_ptr<Grid>    grid_;
      bool                       alive_;
      itemList_t                 inventory_;
      boost::shared_ptr<InfoBar> info_;
      pItem_t                    equipped_;
      pImage_t                   imgEquipped_;

      void collect(pItem_t item);

      bool isCollectable(int gX, int gY) const;
      bool isFree(int gX, int gY) const;
      inline pItem_t getEquipped() const;

      void shootUp();
      void shootLeft();
      void shootDown();
      void shootRight();
};

inline void Player::setGrid(boost::shared_ptr<Grid> grid) { grid_ = grid; }

inline void Player::setScore(int score) { score_ = score; }

inline int Player::getScore() const { return score_; }

inline bool Player::isAlive() const { return alive_; }

inline void Player::explode(pGrid_t grid) { kill(); ExplosiveItem::explode(grid); }

inline void Player::kill() { alive_ = false; }

inline const itemList_t& Player::getInventory() const { return inventory_; }

inline void Player::setRequired(int n) { required_ = n; }

inline pItem_t Player::getEquipped() const { return equipped_; }


#endif /*!__PLAYER_HPP__*/
