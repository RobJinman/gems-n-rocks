/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#ifndef __ICON_HPP__
#define __ICON_HPP__


#include <boost/shared_ptr.hpp>

#include "Image.hpp"


class Icon;
typedef boost::shared_ptr<Icon> pIcon_t;

class Icon {
   private:
      int         pos_[2];
      pImage_t    image_;
      std::string name_;
      bool        visible_;

   public:
      Icon(pImage_t image, int x, int y);

      void render(int ox, int oy) const;

      inline void setName(const std::string& name);
      inline void setPosition(int x, int y);
      inline void setVisible(bool b);

      inline const int* getPosition() const;
      inline pImage_t getImage() const;
      inline const std::string& getName() const;
      inline bool isVisible() const;
};

inline const int* Icon::getPosition() const { return pos_; }

inline pImage_t Icon::getImage() const { return image_; }

inline const std::string& Icon::getName() const { return name_; }

inline void Icon::setName(const std::string& name) { name_ = name; }

inline void Icon::setPosition(int x, int y) { pos_[0] = x; pos_[1] = y; }

inline void Icon::setVisible(bool b) { visible_ = b; }

inline bool Icon::isVisible() const { return visible_; }


#endif /*!__ICON_HPP__*/
