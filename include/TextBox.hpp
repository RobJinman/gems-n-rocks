/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#ifndef __TEXT_BOX_HPP__
#define __TEXT_BOX_HPP__


#include <string>
#include <boost/shared_ptr.hpp>

#include "Image.hpp"


class TextBox;
typedef boost::shared_ptr<TextBox> pTextBox_t;

class TextBox {
   private:
      pImage_t       font_;
      int            dimension_[2];
      int            pos_[2];
      int            charDimension_[2];
      std::string    text_;
      std::string    name_;
      bool           visible_;

   public:
      TextBox(pImage_t font, int charWidth, int charHeight, int x, int y);

      void render(int x, int y) const;

      inline void setName(const std::string& name);
      inline void setVisible(bool b);
      inline void setText(std::string text);
      inline void setPosition(int x, int y);
      inline void setDimensions(int w, int h);
      inline void setCharDimenions(int w, int h);

      inline bool isVisible() const;
      inline const int* getPosition() const;
      inline const int* getDimensions() const;
      inline const std::string& getName() const;

      virtual ~TextBox() {}
};

inline void TextBox::setText(std::string text) {
   text_ = text;
   dimension_[0] = charDimension_[0] * text_.length();
   dimension_[1] = charDimension_[1];
}

inline const int* TextBox::getDimensions() const { return dimension_; }

inline const int* TextBox::getPosition() const { return pos_; }

inline void TextBox::setPosition(int x, int y) { pos_[0] = x; pos_[1] = y; }

inline void TextBox::setCharDimenions(int w, int h) { charDimension_[0] = w; charDimension_[1] = h; }

inline void TextBox::setName(const std::string& name) { name_ = name; }

inline const std::string& TextBox::getName() const { return name_; }

inline bool TextBox::isVisible() const { return visible_; }

inline void TextBox::setVisible(bool b) { visible_ = b; }


#endif /*!__TEXT_BOX_HPP__*/
