/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */


#include "xException.hpp"
#include "InfoBar.hpp"
#include "Icon.hpp"
#include "Image.hpp"
#include "Counter.hpp"
#include "TextBox.hpp"

#include <GL/glut.h>


/*
=====================================
InfoBar::InfoBar
=====================================
*/
InfoBar::InfoBar(int x, int y, int w, int h, pImage_t image) {
   pos_[0] = x;
   pos_[1] = y;
   dimension_[0] = w;
   dimension_[1] = h;
   image_ = image;
   colour_[0] = 0.0;
   colour_[1] = 0.0;
   colour_[2] = 0.0;
   colour_[3] = 0.0;
}

/*
=====================================
InfoBar::stretch
   Only stretches; does not shrink.
=====================================
*/
void InfoBar::stretch(int padW, int padH) {
   int maxX = 0, maxY = 0;
   for (unsigned int i = 0; i < textBoxes_.size(); i++) {
      int x = textBoxes_[i]->getPosition()[0];
      int y = textBoxes_[i]->getPosition()[1];
      int w = textBoxes_[i]->getDimensions()[0];
      int h = textBoxes_[i]->getDimensions()[1];

      if (x + w > maxX) maxX = x + w;
      if (y + h > maxY) maxY = y + h;
   }
   for (unsigned int i = 0; i < counters_.size(); i++) {
      int x = counters_[i]->getPosition()[0];
      int y = counters_[i]->getPosition()[1];
      int w = counters_[i]->getDimensions()[0];
      int h = counters_[i]->getDimensions()[1];

      if (x + w > maxX) maxX = x + w;
      if (y + h > maxY) maxY = y + h;
   }
   for (unsigned int i = 0; i < icons_.size(); i++) {
      int x = icons_[i]->getPosition()[0];
      int y = icons_[i]->getPosition()[1];
      int w = icons_[i]->getImage()->getDimensions()[0];
      int h = icons_[i]->getImage()->getDimensions()[1];

      if (x + w > maxX) maxX = x + w;
      if (y + h > maxY) maxY = y + h;
   }
   dimension_[0] = maxX - pos_[0] + padW;
   dimension_[1] = maxY - pos_[1] + padH;
}

/*
=====================================
InfoBar::render
=====================================
*/
void InfoBar::render() const {
   int x = pos_[0];
   int y = pos_[1];
   int w = dimension_[0];
   int h = dimension_[1];

   if (image_) {
      glBindTexture(GL_TEXTURE_2D, image_->getGlTex());

      glBegin(GL_QUADS);
         glTexCoord2d(0.0, 0.0); glVertex2f(x,     y     );
         glTexCoord2d(1.0, 0.0); glVertex2f(x + w, y     );
         glTexCoord2d(1.0, 1.0); glVertex2f(x + w, y + h );
         glTexCoord2d(0.0, 1.0); glVertex2f(x,     y + h );
      glEnd();
   }
   else {
      glDisable(GL_TEXTURE_2D);
      glColor4d(colour_[0], colour_[1], colour_[2], colour_[3]);
      glBegin(GL_QUADS);
         glVertex2f(x,     y     );
         glVertex2f(x + w, y     );
         glVertex2f(x + w, y + h );
         glVertex2f(x,     y + h );
      glEnd();
      glColor4d(1.0, 1.0, 1.0, 1.0);
      glEnable(GL_TEXTURE_2D);
   }

   for (unsigned int t = 0; t < textBoxes_.size(); t++) {
      if (textBoxes_[t]->isVisible()) textBoxes_[t]->render(pos_[0], pos_[1]);
   }

   for (unsigned int c = 0; c < counters_.size(); c++) {
      if (counters_[c]->isVisible()) counters_[c]->render(pos_[0], pos_[1]);
   }

   for (unsigned int i = 0; i < icons_.size(); i++) {
      if (icons_[i]->isVisible()) icons_[i]->render(pos_[0], pos_[1]);
   }
}

/*
=====================================
InfoBar::getCounter
=====================================
*/
pCounter_t InfoBar::getCounter(std::string name) const {
   for (unsigned int c = 0; c < counters_.size(); c++) {
      if (name.compare(counters_[c]->getName()) == 0) return counters_[c];
   }
   throw xBadParameter("Counter object not found", __FILE__, __LINE__);
}

/*
=====================================
InfoBar::getTextBox
=====================================
*/
pTextBox_t InfoBar::getTextBox(std::string name) const {
   for (unsigned int i = 0; i < textBoxes_.size(); i++) {
      if (name.compare(textBoxes_[i]->getName()) == 0) return textBoxes_[i];
   }
   throw xBadParameter("TextBox object not found", __FILE__, __LINE__);
}

/*
=====================================
InfoBar::getIcon
=====================================
*/
pIcon_t InfoBar::getIcon(std::string name) const {
   for (unsigned int i = 0; i < icons_.size(); i++) {
      if (name.compare(icons_[i]->getName()) == 0) return icons_[i];
   }
   throw xBadParameter("Icon object not found", __FILE__, __LINE__);
}

/*
=====================================
InfoBar::addIcon
=====================================
*/
void InfoBar::addIcon(pImage_t image, int x, int y, const std::string& name) {
   try {
      pIcon_t icon(new Icon(image, x, y));
      icon->setName(name);
      icons_.push_back(icon);
   }
   catch (std::bad_alloc&) {
      throw xMemAllocFail("Error adding Icon to InfoBar", __FILE__, __LINE__);
   }
}

/*
=====================================
InfoBar::addIcon
   Returns true if icon is removed
=====================================
*/
bool InfoBar::removeIcon(const std::string& name) {
   for (unsigned int i = 0; i < icons_.size(); i++) {
      if (icons_[i]->getName().compare(name) == 0) {
         icons_.erase(icons_.begin() + i);
         return true;
      }
   }

   return false;
}

/*
=====================================
InfoBar::addCounter
=====================================
*/
void InfoBar::addCounter(pImage_t font, int charWidth, int charHeight, int x, int y, const std::string& name) {
   try {
      pCounter_t counter(new Counter(font, charWidth, charHeight, x, y));
      counter->setName(name);
      counters_.push_back(counter);
   }
   catch (std::bad_alloc&) {
      throw xMemAllocFail("Error adding Counter to InfoBar", __FILE__, __LINE__);
   }
}

/*
=====================================
InfoBar::addTextBox
=====================================
*/
void InfoBar::addTextBox(pImage_t font, int charWidth, int charHeight, int x, int y, const std::string& name) {
   try {
      pTextBox_t textBox(new TextBox(font, charWidth, charHeight, x, y));
      textBox->setName(name);
      textBoxes_.push_back(textBox);
   }
   catch (std::bad_alloc&) {
      throw xMemAllocFail("Error adding TextBox to InfoBar", __FILE__, __LINE__);
   }
}
