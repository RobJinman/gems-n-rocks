/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#ifndef __MENU_HPP__
#define __MENU_HPP__


#include <boost/enable_shared_from_this.hpp>

#include "InfoBar.hpp"


class Menu;
typedef boost::shared_ptr<Menu> pMenu_t;

struct menuItem_t {
   pTextBox_t textbox;
   pMenu_t link;
   bool selectable;
   bool backLink;
};

class Menu : public InfoBar, public boost::enable_shared_from_this<Menu> {
   private:
      struct status_t {
         bool transitioning;
         double frameRate;
         double dy;
         double y;
         int direction; // 0 = down
      };

      unsigned int               id_;
      pImage_t                   font_;
      pImage_t                   selFont_;               // For the selected item
      pImage_t                   selImage_;              // "
      int                        charWidth_;
      int                        charHeight_;
      int                        itemHeight_;
      int                        dimension_[2];
      int                        nItems_;
      int                        topMargin_;
      int                        nSelectables_;

      pMenu_t                    parent_;
      std::vector<menuItem_t>    items_;
      int                        currentSelection_;

      static double speed_;

      mutable status_t status_;


      void computeFrameRate() const;

   protected:
      inline void setParent(pMenu_t parent);

   public:
      Menu(int x, int y, int w, int h, pImage_t font, int charWidth, int charHeight, unsigned int id,
         pImage_t image = pImage_t());

      void render() const;
      void goUp();
      void goDown();

      inline void setSelectedItemImage(pImage_t img);
      inline void setSelectedItemFont(pImage_t font);
      inline static void setTransitionTime(double speed);

      void addItem(std::string text, bool selectable = true);
      void addItem(std::string text, pMenu_t link);
      void addBackLink(std::string text);

      inline void setItemText(int i, std::string text);

      inline void setSelected(int n);
      inline void setTopMargin(int y);
      inline void setItemHeight(int h);

      void selectFirst();
      void goThroughLink();

      inline int getSelected() const;
      inline unsigned int getId() const;

      static pMenu_t activeMenu;
};

inline void Menu::setParent(pMenu_t parent) { parent_ = parent; }

inline void Menu::setTopMargin(int y) { topMargin_ = y; }

inline void Menu::setItemHeight(int h) { itemHeight_ = h; }

inline void Menu::setItemText(int i, std::string text) { if (i >= 0 && i < items_.size()) items_[i].textbox->setText(text); }

inline void Menu::setSelected(int n) { currentSelection_ = n; }

inline void Menu::setSelectedItemImage(pImage_t img) { selImage_ = img; }

inline void Menu::setSelectedItemFont(pImage_t font) { selFont_ = font; }

inline void Menu::setTransitionTime(double speed) { Menu::speed_ = speed; }

inline int Menu::getSelected() const { return currentSelection_; }

inline unsigned int Menu::getId() const { return id_; }


#endif /*!__MENU_HPP__*/
