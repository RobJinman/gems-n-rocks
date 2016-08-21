/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#include <stdio.h>
#include <string.h>
#include <sstream>

#include "Menu.hpp"
#include "InfoBar.hpp"
#include "Image.hpp"
#include "TextBox.hpp"


double Menu::speed_ = 0.2;
pMenu_t Menu::activeMenu = pMenu_t();

/*
=====================================
Menu::Menu
=====================================
*/
Menu::Menu(int x, int y, int w, int h, pImage_t font, int charWidth, int charHeight, unsigned int id, pImage_t image)
   : InfoBar(x, y, w, h, image) {

   font_ = font;

   selFont_ = pImage_t();
   selImage_ = pImage_t();

   id_ = id;
   charWidth_ = charWidth;
   charHeight_ = charHeight;
   itemHeight_ = charHeight_ + 20;
   topMargin_ = 0;
   dimension_[0] = w;
   dimension_[1] = h;
   nItems_ = 0;
   nSelectables_ = 0;
   currentSelection_ = 0;

   status_.transitioning = false;
}

/*
=====================================
Menu::selectFirst
=====================================
*/
void Menu::selectFirst() {
   if (nSelectables_ > 0) {
      currentSelection_ = 0;
      while (!items_[currentSelection_].selectable) currentSelection_++;
   }
}

/*
=====================================
Menu::goUp
=====================================
*/
void Menu::goUp() {
   if (status_.transitioning) return;
   if (nSelectables_ <= 1) return;

   int t = currentSelection_;
   do {
      currentSelection_ = (currentSelection_ + nItems_ - 1) % nItems_;
   } while (!items_[currentSelection_].selectable);

   status_.dy = (static_cast<double>((t - currentSelection_) * itemHeight_) / Menu::speed_) / status_.frameRate;
   status_.transitioning = true;
   status_.direction = t > currentSelection_ ? 1 : 0;
}

/*
=====================================
Menu::goDown
=====================================
*/
void Menu::goDown() {
   if (status_.transitioning) return;
   if (nSelectables_ <= 1) return;

   int t = currentSelection_;
   do {
      currentSelection_ = (currentSelection_ + 1) % nItems_;
   } while (!items_[currentSelection_].selectable);

   status_.dy = (static_cast<double>((t - currentSelection_) * itemHeight_) / Menu::speed_) / status_.frameRate;
   status_.transitioning = true;
   status_.direction = t > currentSelection_ ? 1 : 0;
}

/*
=====================================
Menu::render
=====================================
*/
void Menu::render() const {
   InfoBar::render();

   if (nSelectables_ > 0) {
      if (selImage_) {
         glBindTexture(GL_TEXTURE_2D, selImage_->getGlTex());

         int h = selImage_->getDimensions()[1];
         int y = dimension_[1] - ((currentSelection_ + 1) * itemHeight_) + (itemHeight_ / 2) - topMargin_ - ((h - charHeight_) / 2);
         int w = selImage_->getDimensions()[0];
         int x = ((dimension_[0] - w) / 2);

         if (status_.transitioning) {
            status_.y += status_.dy;

            if (status_.direction == 0) { // Down
               if (status_.y < y) {
                  status_.y = y;
                  status_.transitioning = false;
               }
            }
            else {
               if (status_.y > y) {
                  status_.y = y;
                  status_.transitioning = false;
               }
            }

            y = status_.y;
         }
         else {
            status_.y = y;
         }

         glBegin(GL_QUADS);
            glTexCoord2d(0.0, 0.0);   glVertex2f(x,     y      );
            glTexCoord2d(1.0, 0.0);   glVertex2f(x + w, y      );
            glTexCoord2d(1.0, 1.0);   glVertex2f(x + w, y + h  );
            glTexCoord2d(0.0, 1.0);   glVertex2f(x,     y + h  );
         glEnd();
      }
   }

   computeFrameRate();
}

/*
=====================================
Menu::computeFrameRate
=====================================
*/
void Menu::computeFrameRate() const {
   static long int i = 0; i++;
   if (i % 10 == 0) {
      long int now;
      static double sPrev = 0;

      now = glutGet(GLUT_ELAPSED_TIME);
      double sNow = static_cast<double>(now) / 1000.0; // Current time in seconds
      double fr = 10.0 / (sNow - sPrev);
      status_.frameRate = static_cast<int>(fr);

      sPrev = sNow;
   }
}

/*
=====================================
Menu::addItem
=====================================
*/
void Menu::addItem(std::string text, bool selectable) {
   nItems_++;
   int x = (dimension_[0] - (text.length() * charWidth_)) / 2;
   int y = dimension_[1] - (nItems_ * itemHeight_) + (itemHeight_ / 2) - topMargin_;

   std::stringstream str;
   str << "mnuItem" << getTextBoxes().size();

   addTextBox(font_, charWidth_, charHeight_, x, y, str.str());
   getTextBoxes().back()->setText(text);

   menuItem_t item;
   item.textbox = getTextBoxes().back();
   item.link = pMenu_t();
   item.selectable = selectable;
   item.backLink = false;
   items_.push_back(item);

   if (selectable) nSelectables_++;
}

/*
=====================================
Menu::addItem
=====================================
*/
void Menu::addItem(std::string text, pMenu_t link) {
   addItem(text);
   items_.back().link = link;
}

/*
=====================================
Menu::addBackLink
=====================================
*/
void Menu::addBackLink(std::string text) {
   addItem(text);
   items_.back().backLink = true;
}

/*
=====================================
Menu::goThroughLink
=====================================
*/
void Menu::goThroughLink() {
   if (currentSelection_ >= items_.size() || currentSelection_ < 0) return;

   if (items_[currentSelection_].backLink) {
      Menu::activeMenu = parent_;
      return;
   }

   if (items_[currentSelection_].link) {
      items_[currentSelection_].link->setParent(shared_from_this());
      Menu::activeMenu = items_[currentSelection_].link;
      Menu::activeMenu->selectFirst();
   }
}