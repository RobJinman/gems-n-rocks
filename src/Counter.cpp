/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#include <string>
#include <sstream>

#include "Counter.hpp"
#include "TextBox.hpp"


/*
=====================================
Counter::Counter
=====================================
*/
Counter::Counter(pImage_t font, int charWidth, int charHeight, int x, int y)
   : TextBox(font, charWidth, charHeight, x, y) {

   visible_ = true;
   reset();
}

/*
=====================================
Counter::reset
=====================================
*/
void Counter::reset() {
   value_ = 0;
   updateText();
}

/*
=====================================
Counter::setValue
=====================================
*/
void Counter::setValue(int n) {
   value_ = n;
   updateText();
}

/*
=====================================
Counter::add
=====================================
*/
void Counter::add(int n) {
   value_ += n;
   updateText();
}

/*
=====================================
Counter::updateText
=====================================
*/
void Counter::updateText() {
   std::stringstream str;
   str << value_;
   setText(str.str());
}
