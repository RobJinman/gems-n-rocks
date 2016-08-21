/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#ifndef __COUNTER_HPP__
#define __COUNTER_HPP__


#include <string>

#include "TextBox.hpp"


class Counter;
typedef boost::shared_ptr<Counter> pCounter_t;

class Counter : public TextBox {
   private:
      void           updateText();
      int            value_;
      std::string    name_;
      bool           visible_;

   public:
      Counter(pImage_t font, int charWidth, int charHeight, int x, int y);

      void reset();
      void add(int n);
      void setValue(int n);
      inline void setName(const std::string& name);
      inline void setVisible(bool b);

      inline const std::string& getName() const;
};

inline void Counter::setName(const std::string& name) { name_ = name; }

inline const std::string& Counter::getName() const { return name_; }

inline void Counter::setVisible(bool b) { visible_ = b; }


#endif /*!__COUNTER_HPP__*/
