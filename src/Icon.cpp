/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#include "Icon.hpp"
#include "Image.hpp"


/*
=====================================
Icon::Icon
=====================================
*/
Icon::Icon(pImage_t image, int x, int y) {
   image_ = image;
   pos_[0] = x;
   pos_[1] = y;
   visible_ = true;
}

/*
=====================================
Icon::render
=====================================
*/
void Icon::render(int ox, int oy) const {
   glBindTexture(GL_TEXTURE_2D, image_->getGlTex());

   int x = ox + pos_[0];
   int y = oy + pos_[1];
   int w = image_->getDimensions()[0];
   int h = image_->getDimensions()[1];

   glBegin(GL_QUADS);
      glTexCoord2d(0.0, 0.0);   glVertex2f(x,     y      );
      glTexCoord2d(1.0, 0.0);   glVertex2f(x + w, y      );
      glTexCoord2d(1.0, 1.0);   glVertex2f(x + w, y + h  );
      glTexCoord2d(0.0, 1.0);   glVertex2f(x,     y + h  );
   glEnd();
}
