/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#ifndef __INFO_BAR_HPP__
#define __INFO_BAR_HPP__


#include <boost/shared_ptr.hpp>
#include <vector>
#include <string>

#include "Image.hpp"
#include "Counter.hpp"
#include "Icon.hpp"


// Essentially a container for text boxes
class InfoBar {
   private:
      int                      pos_[2];
      int                      dimension_[2];
      pImage_t                 image_;
      std::vector<pTextBox_t>  textBoxes_;
      std::vector<pCounter_t>  counters_;
      std::vector<pIcon_t>     icons_;
      double                   colour_[4];

   public:
      InfoBar(int x, int y, int w, int h, pImage_t image = pImage_t());

      void stretch(int padW, int padH);
      inline void centreX(int winW);
      inline void centreY(int winH);

      inline void setPosition(int x, int y);
      inline void setDimensions(int w, int h);
      inline void setColour(double r, double g, double b, double a);

      void addCounter(pImage_t font, int charWidth, int charHeight, int x, int y, const std::string& name = std::string());
      void addTextBox(pImage_t font, int charWidth, int charHeight, int x, int y, const std::string& name = std::string());
      void addIcon(pImage_t image, int x, int y, const std::string& name = std::string());
      bool removeIcon(const std::string& name);
      inline void clearIcons();

      inline const int* getPosition() const;
      inline const int* getDimensions() const;

      inline std::vector<pTextBox_t >& getTextBoxes();
      inline std::vector<pCounter_t >& getCounters();
      pCounter_t getCounter(std::string name) const;
      pTextBox_t getTextBox(std::string name) const;
      pIcon_t getIcon(std::string name) const;

      virtual void render() const;

      virtual ~InfoBar() {}
};

inline void InfoBar::centreX(int winW) { pos_[0] = (winW - dimension_[0]) / 2; }

inline void InfoBar::centreY(int winH) { pos_[1] = (winH - dimension_[1]) / 2; }

inline void InfoBar::setPosition(int x, int y) { pos_[0] = x; pos_[1] = y; }

inline void InfoBar::setDimensions(int w, int h) { dimension_[0] = w; dimension_[1] = h; }

inline void InfoBar::setColour(double r, double g, double b, double a) { colour_[0] = r; colour_[1] = g; colour_[2] = b; colour_[3] = a; }

inline const int* InfoBar::getPosition() const { return pos_; }

inline const int* InfoBar::getDimensions() const { return dimension_; }

inline std::vector<pTextBox_t >& InfoBar::getTextBoxes() { return textBoxes_; }

inline std::vector<pCounter_t >& InfoBar::getCounters() { return counters_; }

inline void InfoBar::clearIcons() { icons_.clear(); }


#endif /*!__INFO_BAR_HPP__*/
