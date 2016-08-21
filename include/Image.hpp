/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#ifndef __IMAGE_HPP__
#define __IMAGE_HPP__


#include <GL/glut.h>
#include <boost/shared_ptr.hpp>

#include "Resource.hpp"


class Game;

class Image;
typedef boost::shared_ptr<Image> pImage_t;

class Image : public Resource {
   private:
      int      dimension_[2];
      GLuint   glTex_;         // Unique texture identifier for openGL

		static GLuint* GlTexture;
		static int     NglTextures;
		static int     GlTex;

   public:
      Image() { }
      Image(const std::string& filePath, int w, int h);
      ~Image();

      virtual void loadFromFile(const std::string& filePath, int w, int h);
      inline const int* getDimensions() const;
      inline unsigned int getGlTex() const;

		static GLuint* getGlTextures() { return GlTexture; }
		static int getNumGlTextures() { return NglTextures; }
		static void setGlTex(int t) { GlTex = t; }
};

inline const int* Image::getDimensions() const { return dimension_; }

inline unsigned int Image::getGlTex() const { return glTex_; }


#endif /*!__IMAGE_HPP__*/
