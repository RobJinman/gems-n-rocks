/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#include <cstdlib>
#include <iostream>
#include <sys/stat.h>
#include <GL/glut.h>

#include "Image.hpp"


int Image::NglTextures = 64;
GLuint* Image::GlTexture = new GLuint[Image::getNumGlTextures()];
int Image::GlTex = 0;

Image::Image(const std::string& filePath, int w, int h) { loadFromFile(filePath, w, h); }

/*
=====================================
Image::loadFromFile
=====================================
*/
void Image::loadFromFile(const std::string& filePath, int w, int h) {
   Resource::loadFromFile(filePath);
   data_ += 54; // Skip header

	glTex_ = GlTexture[GlTex];
	glBindTexture(GL_TEXTURE_2D, glTex_);
	GlTex++;

   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
   glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, data_);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   dimension_[0] = w;
   dimension_[1] = h;
}

/*
=====================================
Image::~Image
=====================================
*/
Image::~Image() {
   if (data_) data_ -= 54;
}
