/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#include <boost/shared_ptr.hpp>
#include <string>
#include <GL/glut.h>

#include "xException.hpp"
#include "TextBox.hpp"


/*
=====================================
TextBox::TextBox
=====================================
*/
TextBox::TextBox(pImage_t font, int charWidth, int charHeight, int x, int y) {
   font_ = font;
   charDimension_[0] = charWidth;
   charDimension_[1] = charHeight;
   pos_[0] = x;
   pos_[1] = y;
   dimension_[0] = 0;
   dimension_[1] = 0;
   visible_ = true;
}

/*
=====================================
TextBox::render
=====================================
*/
void TextBox::render(int ox, int oy) const {
   glBindTexture(GL_TEXTURE_2D, font_->getGlTex());

   int ibeam = 0;
   for (unsigned int i = 0; i < text_.length(); i++) {

      int w = charDimension_[0];
      int h = charDimension_[1];
      int x = ox + pos_[0] + ibeam;
      int y = oy + pos_[1];
      int bw = font_->getDimensions()[0];
      int bh = font_->getDimensions()[1];
      float tx = (float)(w * (text_[i] - 32)) / (float)bw;
      float ty = 0.0;
      float tw = (float)w / (float)bw;
      float th = (float)h / (float)bh;

      glBegin(GL_QUADS);
         glTexCoord2d(tx,        ty       );   glVertex2f(x,     y      );
         glTexCoord2d(tx + tw,   ty       );   glVertex2f(x + w, y      );
         glTexCoord2d(tx + tw,   ty + th  );   glVertex2f(x + w, y + h  );
         glTexCoord2d(tx,        ty + th  );   glVertex2f(x,     y + h  );
      glEnd();
      ibeam += w;
   }
}
