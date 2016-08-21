/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#ifndef __GRID_HPP__
#define __GRID_HPP__


#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "items/Item.hpp"
#include "xException.hpp"


class Player;

class Grid;
typedef boost::shared_ptr<Grid> pGrid_t;

class Grid : public boost::enable_shared_from_this<Grid> {
   private:
      int            dimension_[2];
      int            blockSize_;
      itemList_t**   items_;
   public:
      Grid(int w, int h, int blockSize);
      ~Grid();

      void clear();
      void addItem(pItem_t item, int x, int y);
      void removeItem(pItem_t item, int x, int y) const;
      bool removeItem(pItem_t item) const;
      inline void setDimensions(int w, int h);
      inline void setBlockSize(int b);

      inline const int* getDimensions() const;
      inline int getBlockSize() const;
      inline itemList_t& getItems(int x, int y) const;
      itemList_t getItems(int x, int y, const char* type) const;

      void hitFromLeft(int x, int y, boost::shared_ptr<Item> item);
      void hitFromTop(int x, int y, boost::shared_ptr<Item> item);
      void hitFromRight(int x, int y, boost::shared_ptr<Item> item);
      void hitFromBottom(int x, int y, boost::shared_ptr<Item> item);
      void squash(int x, int y, boost::shared_ptr<Item> item);
      void burn(int x, int y, boost::shared_ptr<Item> item);
};

inline itemList_t& Grid::getItems(int x, int y) const {
   if (x >= dimension_[0] || y >= dimension_[1] || x < 0 || y < 0)
      throw xBadParameter("Grid coordinates out of range", __FILE__, __LINE__);

   return items_[x][y];
}

inline void Grid::setDimensions(int w, int h) { dimension_[0] = w; dimension_[1] = h; }

inline void Grid::setBlockSize(int b) { blockSize_ = b; }

inline const int* Grid::getDimensions() const { return dimension_; }

inline int Grid::getBlockSize() const { return blockSize_; }


#endif /*!__GRID_HPP__*/
