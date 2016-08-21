/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <math.h>
#include <time.h>
#include <GL/glut.h>

#include "SdlAudio.hpp"
#include "AudioExceptions.hpp"
#include "xException.hpp"
#include "Game.hpp"
#include "Counter.hpp"
#include "Grid.hpp"
#include "InfoBar.hpp"
#include "Menu.hpp"
#include "Image.hpp"
#include "Player.hpp"
#include "Resource.hpp"
#include "TextBox.hpp"
#include "ItemFactory.hpp"


float Game::masterVol = 0.75;
float Game::musicVol = 1.0;
float Game::sfxVol = 1.0;

/*
=====================================
Game::Game
=====================================
*/
Game::Game(const std::string& appPath) {
   appPath_ = appPath;
   Item::setAppPath(appPath);
   AnimSequence::setAppPath(appPath);
   Image::setGlTex(0);
   player_.reset();
   grid_.reset();
   info_.reset();
   frameRate_ = 99;
   currentLevel_ = 1;
   lvlSelected_ = 1;
   sessionMaxLevel_ = 1;
   totalElapsedPaused_ = 0;
   for (int i = 0; i < 256; i++) keyState_[i] = false;
}

/*
=====================================
Game::renew
=====================================
*/
void Game::renew(const gameInitOpt_t& opts) {
   state_ = ST_END;
   nextGame_.winW = opts.winW;
   nextGame_.winH = opts.winH;
   nextGame_.level = opts.level;
   nextGame_.maxLevel = opts.maxLevel;
   nextGame_.state = opts.state;
}

/*
=====================================
Game::keyDown
=====================================
*/
void Game::keyDown(int key, int x, int y) {
   keyState_[key] = true;

   switch (state_) {
      case ST_RUNNING:
         switch (key) {
            case 'r': {
               gameInitOpt_t opts;
               opts.level = currentLevel_;
               opts.maxLevel = sessionMaxLevel_;
               opts.winW = windowDimension_[0];
               opts.winH = windowDimension_[1];
               opts.state = ST_LEVEL_START;

               renew(opts);
            }
            break;
            case 'q': {
               gameInitOpt_t opts;
               opts.level = currentLevel_;
               opts.maxLevel = sessionMaxLevel_;
               opts.winW = windowDimension_[0];
               opts.winH = windowDimension_[1];
               opts.state = ST_MAIN_MNU;

               renew(opts);
            }
            break;
            case GLUT_KEY_F1: {  // enter/f1
               if (!player_->isAlive()) {
                  gameInitOpt_t opts;
                  opts.level = currentLevel_;
                  opts.maxLevel = sessionMaxLevel_;
                  opts.winW = windowDimension_[0];
                  opts.winH = windowDimension_[1];
                  opts.state = ST_LEVEL_START;

                  renew(opts);
                  break;
               }
            }
            case 27:             // esc
            case 'p':
               state_ = ST_PAUSE_MNU;
               Menu::activeMenu = mnuPaused_;
               timePaused_ = static_cast<int>(time(NULL));
            break;
            case 'c':
               player_->toggleEquipped();
         }
      break;
      case ST_LEVEL_START:
         keyState_['z'] = false;
         keyState_[key] = false;
         state_ = ST_RUNNING;
         timeStarted_ = static_cast<int>(time(NULL));
      break;
      case ST_MAIN_MNU:
      case ST_PAUSE_MNU: {
         switch (key) {
            case GLUT_KEY_F1: {  // Is actually enter key
               switch (Menu::activeMenu->getId()) {
                  case MNU_ROOT_MAIN:
                     switch (Menu::activeMenu->getSelected()) {
                        case 2:
                           gameInitOpt_t opts;
                           opts.level = lvlSelected_;
                           opts.maxLevel = sessionMaxLevel_;
                           opts.winW = windowDimension_[0];
                           opts.winH = windowDimension_[1];
                           opts.state = ST_LEVEL_START;

                           renew(opts);
                        break;
                        case 6: exit(0); break;
                     }
                  break;
                  case MNU_ROOT_PAUSE:
                     switch (Menu::activeMenu->getSelected()) {
                        case 2:
                           state_ = ST_RUNNING;
                           elapsedPaused_ = static_cast<int>(time(NULL)) - timePaused_;
                           totalElapsedPaused_ += elapsedPaused_;
                        break;
                        case 5: {
                           gameInitOpt_t opts;
                           opts.level = currentLevel_;
                           opts.maxLevel = sessionMaxLevel_;
                           opts.winW = windowDimension_[0];
                           opts.winH = windowDimension_[1];
                           opts.state = ST_MAIN_MNU;

                           renew(opts);
                        } break;
                     }
                  break;
                  case MNU_SUB_CREDITS:
                     switch (Menu::activeMenu->getSelected()) {
                        case 0: {
                           gameInitOpt_t opts;
                           opts.level = currentLevel_;
                           opts.maxLevel = sessionMaxLevel_;
                           opts.winW = windowDimension_[0];
                           opts.winH = windowDimension_[1];
                           opts.state = ST_MAIN_MNU;

                           renew(opts);
                           return;
                        } break;
                     }
                  break;
               }

               Menu::activeMenu->goThroughLink();
            }
            break;
            case GLUT_KEY_DOWN: Menu::activeMenu->goDown(); break;
            case GLUT_KEY_UP: Menu::activeMenu->goUp(); break;
            case GLUT_KEY_LEFT: {
               switch (Menu::activeMenu->getId()) {
                  case MNU_ROOT_MAIN: {
                     switch (Menu::activeMenu->getSelected()) {
                        case 2:
                           if (lvlSelected_ > 1) {
                              lvlSelected_ -= 1;
                           }
                        break;
                     }
                  }
                  break;
                  case MNU_SUB_OPTIONS:
                     switch (Menu::activeMenu->getSelected()) {
                        case 2: if (Game::masterVol >= 0.009) Game::masterVol -= 0.01; break;
                        case 3: if (Game::musicVol >= 0.009) Game::musicVol -= 0.01; break;
                        case 4: if (Game::sfxVol >= 0.009) Game::sfxVol -= 0.01; break;
                     }
                     updateVolume();
                  break;
               }
            }
            break;
            case GLUT_KEY_RIGHT: {
               switch (Menu::activeMenu->getId()) {
                  case MNU_ROOT_MAIN: {
                     switch (Menu::activeMenu->getSelected()) {
                        case 2:
                           if (lvlSelected_ < sessionMaxLevel_) {
                              lvlSelected_ += 1;
                           }
                        break;
                     }
                  }
                  break;
                  case MNU_SUB_OPTIONS:
                     switch (Menu::activeMenu->getSelected()) {
                        case 2: if (Game::masterVol <= 0.991) Game::masterVol += 0.01; break;
                        case 3: if (Game::musicVol <= 0.991) Game::musicVol += 0.01; break;
                        case 4: if (Game::sfxVol <= 0.991) Game::sfxVol += 0.01; break;
                     }
                     updateVolume();
                  break;
               }
            }
            break;
         }
         updateMenu();
      }
      break;
   }
}

/*
=====================================
Game::updateVolume
=====================================
*/
void Game::updateVolume() {
   SdlAudio::setMasterVolume(Game::masterVol);
   SdlAudio::setMusicVolume(Game::musicVol);
   SdlAudio::setAllSoundVolume(Game::sfxVol);
}

/*
=====================================
Game::updateMenu
=====================================
*/
void Game::updateMenu() {
   switch (Menu::activeMenu->getId()) {
      case MNU_SUB_OPTIONS: {
         std::stringstream str;

         str << "<- Master (" << std::setiosflags(std::ios::fixed) << std::setprecision(2) << fabs(Game::masterVol) << ") ->";
         Menu::activeMenu->setItemText(2, str.str().data());

         str.str("");
         str << "<- Music  (" << std::setiosflags(std::ios::fixed) << std::setprecision(2) << fabs(Game::musicVol) << ") ->";
         Menu::activeMenu->setItemText(3, str.str().data());

         str.str("");
         str << "<- SFX    (" << std::setiosflags(std::ios::fixed) << std::setprecision(2) << fabs(Game::sfxVol) << ") ->";
         Menu::activeMenu->setItemText(4, str.str().data());
      }
      break;
      case MNU_ROOT_MAIN: {
         std::stringstream str;
         str << "<- Level " << lvlSelected_ << ": Start! ->";
         Menu::activeMenu->setItemText(2, str.str().data());
      }
      break;
   }
}

/*
=====================================
Game::keyBoard
=====================================
*/
void Game::keyboard() {
   if (keyState_[GLUT_KEY_UP]) {
      if (keyState_['z']) {
         viewCentre_[1] += 500.0 / (float)frameRate_;
      }
      else {
         if (!player_->isVisible()) return;
         if (keyState_[GLUT_KEY_F2])   // Space key
            player_->whipUp();
         else if (keyState_['x'])
            player_->actionUp();
         else
            player_->goUp();
      }
   }
   else if (keyState_[GLUT_KEY_LEFT]) {
      if (keyState_['z']) {
         viewCentre_[0] -= 500.0 / (float)frameRate_;
      }
      else {
         if (!player_->isVisible()) return;
         if (keyState_[GLUT_KEY_F2])
            player_->whipLeft();
         else if (keyState_['x'])
            player_->actionLeft();
         else
            player_->goLeft();
      }
   }
   else if (keyState_[GLUT_KEY_DOWN]) {
      if (keyState_['z']) {
         viewCentre_[1] -= 500.0 / (float)frameRate_;
      }
      else {
         if (!player_->isVisible()) return;
         if (keyState_[GLUT_KEY_F2])
            player_->whipDown();
         else if (keyState_['x'])
            player_->actionDown();
         else
            player_->goDown();
      }
   }
   else if (keyState_[GLUT_KEY_RIGHT]) {
      if (keyState_['z']) {
         viewCentre_[0] += 500.0 / (float)frameRate_;
      }
      else {
         if (!player_->isVisible()) return;
         if (keyState_[GLUT_KEY_F2])
            player_->whipRight();
         else if (keyState_['x'])
            player_->actionRight();
         else
            player_->goRight();
      }
   }
}

/*
=====================================
Game::findNumLevels
=====================================
*/
void Game::findNumLevels() {

   std::stringstream str;
   str << appPath_ << "/data/levels/1.lvl";
   std::ifstream file(str.str().data());

   int l = 2;
   while (file.is_open()) {
      file.close();

      std::stringstream str;
      str << appPath_ << "/data/levels/" << l << ".lvl";

      file.open(str.str().data());

      l++;
   }

   nLevels_ = l - 2;
}

/*
=====================================
Game::centreView
   Centre view around player's position
=====================================
*/
void Game::centreView() {
   if (!keyState_['z']) {
      int plx = player_->getPosition()[0];
      int ply = player_->getPosition()[1];
      viewCentre_[0] = (float)plx;
      viewCentre_[1] = (float)ply;
   }

   float minX = static_cast<float>(windowDimension_[0] / 2);
   if (viewCentre_[0] < minX) viewCentre_[0] = minX;
   float maxX = static_cast<float>(gameDimension_[0] - (windowDimension_[0] / 2));
   if (viewCentre_[0] > maxX) viewCentre_[0] = maxX;
   float minY = static_cast<float>(windowDimension_[1] / 2);
   if (viewCentre_[1] < minY) viewCentre_[1] = minY;
   float maxY = static_cast<float>(gameDimension_[1] - (windowDimension_[1] / 2) + BLOCK_SIZE);
   if (viewCentre_[1] > maxY) viewCentre_[1] = maxY;

   int x = static_cast<int>(viewCentre_[0]) - (windowDimension_[0] / 2);
   if (x >= 0 && x <= gameDimension_[0] - windowDimension_[0]) viewPos_[0] = x;

   int y = static_cast<int>(viewCentre_[1]) - (windowDimension_[1] / 2);
   if (y >= 0 && y <= gameDimension_[1] - windowDimension_[1] + info_->getDimensions()[1]) viewPos_[1] = y;
}

/*
=====================================
Game::levelComplete
=====================================
*/
void Game::levelComplete() {
   SdlAudio::playSound("congrats");

   gameInitOpt_t opts;
   opts.winW = windowDimension_[0];
   opts.winH = windowDimension_[1];
   opts.level = currentLevel_ + 1;
   opts.state = ST_MAIN_MNU;

   if (sessionMaxLevel_ == currentLevel_) {
      if (sessionMaxLevel_ + 1 > nLevels_) {
         state_ = ST_MAIN_MNU;
         Menu::activeMenu = mnuMain_;
         Menu::activeMenu->setSelected(5);
         Menu::activeMenu->goThroughLink();
         SdlAudio::stopSound("tick");
         return;
      }

      saveProgress(sessionMaxLevel_ + 1);
      opts.maxLevel = sessionMaxLevel_ + 1;
      opts.state = ST_MAIN_MNU;

      renew(opts);
   }
   else {
      opts.maxLevel = sessionMaxLevel_;
      renew(opts);
   }
}

/*
=====================================
Game::scrollLevel
=====================================
*/
void Game::scrollLevel() {
   static int toggle = 1;
   static double x = gameDimension_[0] / 2;
   static double y = gameDimension_[1] / 2;
   static double Y = y;

   if (y <= windowDimension_[1] / 2) toggle = 1;
   if (y >= gameDimension_[1] - (windowDimension_[1] / 2)) toggle = -1;

   double PI = 3.14159265;

   double vRange = static_cast<double>(gameDimension_[1] - windowDimension_[1]);
   double halfHRange = static_cast<double>((gameDimension_[0] - windowDimension_[0]) / 2);
   double min = static_cast<double>(windowDimension_[0] / 2);

   double dy = 1.0 * static_cast<double>(toggle);

   double theta;

   double x1 = x;
   theta = (Y / vRange) * 0.5 * PI; // Ranges from 0 to 2PI
   double x2 = ((sin(theta) + 1.0) * halfHRange) + min;

   double dx = x2 - x1;

   double d = 200.0 / static_cast<double>(frameRate_);
   double sf = d / sqrt((dy * dy) + (dx * dx));

   double sdy = dy * sf;
   double sdx = dx * sf;

   x += sdx;
   y += sdy;
   viewCentre_[0] = x;
   viewCentre_[1] = y;
   Y += fabs(sdy);
}

/*
=====================================
Game::mainLoop
=====================================
*/
void Game::mainLoop() {

   //-----Compute frame rate-----
   static long int i = 0; i++;
   if (i % 10 == 0) {
      long int now;
      static double sPrev = 0;

      now = glutGet(GLUT_ELAPSED_TIME);
      double sNow = static_cast<double>(now) / 1000.0; // Current time in seconds
      double fr = 10.0 / (sNow - sPrev);
      frameRate_ = static_cast<int>(fr);

      sPrev = sNow;
   }

   switch (state_) {
      case ST_PAUSE_MNU:
      case ST_MAIN_MNU: {
         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
         Menu::activeMenu->render();
         glutSwapBuffers();
      } break;
      case ST_LEVEL_START: {
         scrollLevel();
         keyState_['z'] = true;
         centreView();
         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
         render();
         lvlIntro_->render();
         glutSwapBuffers();
      } break;
      case ST_RUNNING: {
         try {
            updateScene();
            keyboard();
            centreView();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            render();
            glutSwapBuffers();
         }
         catch (...) { throw; }
      } break;
   }
}

/*
=====================================
Game::renderImage
=====================================
*/
void Game::renderImage(boost::shared_ptr<Image> image) const {
   glBindTexture(GL_TEXTURE_2D, image->getGlTex());
   glBegin(GL_QUADS);
      glTexCoord2d(0.0, 0.0); glVertex2f(0, 0);
      glTexCoord2d(1.0, 0.0); glVertex2f(windowDimension_[0], 0);
      glTexCoord2d(1.0, 1.0); glVertex2f(windowDimension_[0], windowDimension_[1]);
      glTexCoord2d(0.0, 1.0); glVertex2f(0, windowDimension_[1]);
   glEnd();
}

/*
=====================================
Game::updateScene
=====================================
*/
void Game::updateScene() {
   itemList_t items = ItemFactory::getItems();

   for (unsigned int i = 0; i < items.size(); i++) {
      if (items[i]->isVisible()) {
         items[i]->doIdle(grid_);
         if (items[i]->pendingDeletion()) {
            grid_->removeItem(items[i]);
            ItemFactory::eraseItem(items[i]);
         }
      }
   }

   if (player_) {
      info_->getCounter("score")->setValue(player_->getScore());

      int req = requiredScore_ - player_->getScore();
      player_->setRequired(requiredScore_ - player_->getScore());

      if (req <= 0)
         exit_->playAnimation("open_idle");

      if (player_->getPosition()[0] == exit_->getPosition()[0]
         && player_->getPosition()[1] == exit_->getPosition()[1])
            levelComplete();

      if (state_ == ST_RUNNING) {
         if (player_->isAlive()) {
            int t = static_cast<int>(time(NULL)) - timeStarted_ - totalElapsedPaused_;
            info_->getCounter("time")->setValue(t);

            if (t >= timeLimit_) player_->squash(grid_);

            static int prevT = 0;
            if (t == timeLimit_ - 10 && t == prevT + 1) SdlAudio::playSound("tick");

            prevT = t;
         }
      }
   }
}

/*
=====================================
Game::render
=====================================
*/
void Game::render() {

   for (unsigned int p = FIRST_PASS; p <= FINAL_PASS; p++) {
      for (unsigned int i = 0; i < ItemFactory::getItems().size(); i++) {
         if ((unsigned int)ItemFactory::getItems()[i]->getPass() == p/* && ItemFactory::getItems()[i]->isVisible()*/) {

            glBindTexture(GL_TEXTURE_2D, ItemFactory::getItems()[i]->getImage()->getGlTex());

            int x = ItemFactory::getItems()[i]->getPosition()[0] - viewPos_[0];
            int y = ItemFactory::getItems()[i]->getPosition()[1] - viewPos_[1];
            int w = ItemFactory::getItems()[i]->getDimensions()[0];
            int h = ItemFactory::getItems()[i]->getDimensions()[1];

            if ((x >= -w && x <= windowDimension_[0] + w) && (y >= -h && y <= windowDimension_[1] + h)) {
               int bW = ItemFactory::getItems()[i]->getImage()->getDimensions()[0];
               int bH = ItemFactory::getItems()[i]->getImage()->getDimensions()[1];
               int offX = ItemFactory::getItems()[i]->getOffset()[0];
               int offY = ItemFactory::getItems()[i]->getOffset()[1];
               GLfloat originX = static_cast<GLfloat>(offX) / static_cast<GLfloat>(bW);
               GLfloat originY = static_cast<GLfloat>(offY) / static_cast<GLfloat>(bH);
               GLfloat normW = static_cast<GLfloat>(w) / static_cast<GLfloat>(bW);
               GLfloat normH = static_cast<GLfloat>(h) / static_cast<GLfloat>(bH);

               glBegin(GL_QUADS);
                  glTexCoord2f(originX,         originY        );    glVertex2f(x,     y     );
                  glTexCoord2f(originX + normW, originY        );    glVertex2f(x + w, y     );
                  glTexCoord2f(originX + normW, originY + normH);    glVertex2f(x + w, y + h );
                  glTexCoord2f(originX,         originY + normH);    glVertex2f(x,     y + h );
               glEnd();
            }
         }
      }
   }

   info_->render();
}

/*
=====================================
Game::saveProgress
=====================================
*/
void Game::saveProgress(int level) {
   std::stringstream str;
   str << appPath_ << "/x.x";

   std::ofstream file(str.str().data(), std::ios::out | std::ios::binary);

   if (!file.is_open())
      throw xBadFile("Could not open file", __FILE__, __LINE__, str.str());

   file.write((char*)&level, sizeof(int));

   file.close();
}

/*
=====================================
Game::loadProgress
=====================================
*/
void Game::loadProgress() {
   std::stringstream str;
   str << appPath_ << "/x.x";

   std::ifstream file(str.str().data(), std::ios::in | std::ios::binary);

   if (!file.is_open()) return;

   file.read((char*)&sessionMaxLevel_, sizeof(int));

   if (sessionMaxLevel_ > nLevels_) sessionMaxLevel_ = nLevels_;

   file.close();
}

/*
=====================================
Game::init
=====================================
*/
void Game::init(gameInitOpt_t opts) {

   findNumLevels();

   currentLevel_ = opts.level;
   sessionMaxLevel_ = opts.maxLevel;

   loadProgress();

   windowDimension_[0] = opts.winW;
   windowDimension_[1] = opts.winH;

//   state_ = ST_MAIN_MNU;


   glClearColor(0.1, 0.0625, 0.025, 1.0);
	glGenTextures(Image::getNumGlTextures(), Image::getGlTextures());



   ItemFactory::init();
   ItemFactory::loadItemTypes();

   //----Load level data-----

   std::stringstream strLvl;
   strLvl << appPath_ << "/data/levels/" << opts.level << ".lvl";

   try { loadLevel(strLvl.str().data()); }
   catch (xBadFile& ex) {
      currentLevel_ = 1;
      opts.level = 1;

      std::stringstream str;
      str << appPath_ << "/data/levels/1.lvl";
      loadLevel(str.str());
   }

   lvlSelected_ = currentLevel_;
   uiSetup(opts);


   // Create Player object from first item
   grid_->removeItem(ItemFactory::getItems()[0]);
   try {
      player_ = boost::shared_ptr<Player>(new Player(*ItemFactory::getItems()[0], grid_));

      int gX = player_->getPosition()[0] / grid_->getBlockSize();
      int gY = player_->getPosition()[1] / grid_->getBlockSize();
      pItem_t plyr = boost::dynamic_pointer_cast<Item>(player_);

      if (plyr)
         grid_->addItem(plyr, gX, gY);
      else
         throw xException("Error creating Player", __FILE__, __LINE__);
   }
   catch (std::bad_alloc&) {
      throw xMemAllocFail("Could not create Player", __FILE__, __LINE__);
   }
   player_->setInfo(info_);
   itemList_t items;
   items.assign(ItemFactory::getItems().begin(), ItemFactory::getItems().end());

   items.erase(items.begin());
   items.insert(items.begin(), player_);
   ItemFactory::clearItems();
   ItemFactory::addItems(items);

   exit_ = ItemFactory::getItems().back();

   itemList_t enemies = ItemFactory::getItems("enemy");
   for (unsigned int i = 0; i < enemies.size(); i++)
      boost::dynamic_pointer_cast<Enemy>(enemies[i])->setPlayer(player_);


   //-----Set initial view position-----
   centreView();


   //-----Load Sounds-----
   std::stringstream str;

   str << appPath_ << "/data/sounds/clink.wav";
   SdlAudio::addSound(str.str(), "clink");
   str.str("");

   str << appPath_ << "/data/sounds/thud.wav";
   SdlAudio::addSound(str.str(), "thud");
   str.str("");

   str << appPath_ << "/data/sounds/congrats.wav";
   SdlAudio::addSound(str.str(), "congrats");
   str.str("");

   str << appPath_ << "/data/sounds/tick.wav";
   SdlAudio::addSound(str.str(), "tick");
   str.str("");

   SdlAudio::stopSound("tick");
   updateVolume();


   state_ = opts.state;
   switch (state_) {
      case ST_MAIN_MNU: {
         SdlAudio::freeMusic();

         std::stringstream str;
         str << appPath_ << "/data/sounds/menu.ogg";

         SdlAudio::loadMusicFile(str.str());
         SdlAudio::playMusic(true);
      } break;
      case ST_LEVEL_START:
         SdlAudio::freeMusic();
         SdlAudio::loadMusicFile(musicFile_);
         SdlAudio::playMusic(true);
      break;
      case ST_PAUSE_MNU:
         Menu::activeMenu = mnuPaused_;
      break;
   }
}

/*
=====================================
Game::uiSetup
=====================================
*/
void Game::uiSetup(const gameInitOpt_t& opts) {

   try {
      std::stringstream str;

      str << appPath_ << "/data/images/font1.bmp";
      pImage_t font = pImage_t(new Image(str.str(), 1024, 16));
      str.str("");

      str << appPath_ << "/data/images/mnuSelection.bmp";
      pImage_t mnuSel = pImage_t(new Image(str.str(), 256, 32));
      str.str("");

      str << appPath_ << "/data/images/menu.bmp";
      pImage_t mnuBackground = pImage_t(new Image(str.str(), 512, 512));
      str.str("");


      Menu::setTransitionTime(0.1);


      //-----Create and set up credits and controls screens-----

      {
//         int w = 300;
         int h = 20;
         int x = 220;
         int y = opts.winH - 162;

         mnuCredits_ = pMenu_t(new Menu(0, 0, opts.winW, opts.winH, font, 10, 16, MNU_SUB_CREDITS, mnuBackground));
         mnuCredits_->setColour(0.0, 0.0, 0.0, 1.0);
         mnuCredits_->setSelectedItemImage(mnuSel);
         mnuCredits_->setItemHeight(30);
         mnuCredits_->setTopMargin(390);
         mnuCredits_->addTextBox(font, 10, 16, x, y);
         mnuCredits_->getTextBoxes().back()->setText("Graphics & Programming:");
         mnuCredits_->addTextBox(font, 10, 16, x, y - (h * 1));
         mnuCredits_->getTextBoxes().back()->setText("                       Rob Jinman");
         mnuCredits_->addTextBox(font, 10, 16, x, y - (h * 2));
         mnuCredits_->getTextBoxes().back()->setText("Music:");
         mnuCredits_->addTextBox(font, 10, 16, x, y - (h * 3));
         mnuCredits_->getTextBoxes().back()->setText("  [Licensed by www.loopsound.com]");
         mnuCredits_->addTextBox(font, 10, 16, x, y - (h * 4));
         mnuCredits_->getTextBoxes().back()->setText("                   Bob Richardson");
         mnuCredits_->addTextBox(font, 10, 16, x, y - (h * 5));
         mnuCredits_->getTextBoxes().back()->setText("                      Chris Worth");
         mnuCredits_->addTextBox(font, 10, 16, x, y - (h * 6));
         mnuCredits_->getTextBoxes().back()->setText("               Michael Schwendler");
         mnuCredits_->addTextBox(font, 10, 16, x, y - (h * 7));
         mnuCredits_->getTextBoxes().back()->setText("          Stephen William Cornish");
         mnuCredits_->addTextBox(font, 10, 16, x, y - (h * 8));
         mnuCredits_->getTextBoxes().back()->setText("SFX:");
         mnuCredits_->addTextBox(font, 10, 16, x, y - (h * 9));
         mnuCredits_->getTextBoxes().back()->setText("                www.freesound.org");

         mnuCredits_->addItem("back");
      }

      pMenu_t mnuControls;

      {
//         int w = 300;
         int h = 14;
         int x = 210;
         int y = opts.winH - 162;

         mnuControls = pMenu_t(new Menu(0, 0, opts.winW, opts.winH, font, 10, 16, MNU_SUB_CONTROLS, mnuBackground));
         mnuControls->setColour(0.0, 0.0, 0.0, 1.0);
         mnuControls->setSelectedItemImage(mnuSel);
         mnuControls->setItemHeight(30);
         mnuControls->setTopMargin(390);
         mnuControls->addTextBox(font, 10, 16, x, y);
         mnuControls->getTextBoxes().back()->setText("Arrow keys:         move / dig");      y -= 8;
         mnuControls->addTextBox(font, 10, 16, x, y - (h * 1));
         mnuControls->getTextBoxes().back()->setText("Space + arrow keys: dig");
         mnuControls->addTextBox(font, 10, 16, x, y - (h * 2));
         mnuControls->getTextBoxes().back()->setText("                    adjacent space");  y -= 8;
         mnuControls->addTextBox(font, 10, 16, x, y - (h * 3));
         mnuControls->getTextBoxes().back()->setText("x + arrow keys:     use item");        y -= 8;
         mnuControls->addTextBox(font, 10, 16, x, y - (h * 4));
         mnuControls->getTextBoxes().back()->setText("c:                  change equipped");
         mnuControls->addTextBox(font, 10, 16, x, y - (h * 5));
         mnuControls->getTextBoxes().back()->setText("                    item");            y -= 8;
         mnuControls->addTextBox(font, 10, 16, x, y - (h * 6));
         mnuControls->getTextBoxes().back()->setText("z + arrow keys:     look around map"); y -= 8;
         mnuControls->addTextBox(font, 10, 16, x, y - (h * 7));
         mnuControls->getTextBoxes().back()->setText("r:                  restart level");   y -= 8;
         mnuControls->addTextBox(font, 10, 16, x, y - (h * 8));
         mnuControls->getTextBoxes().back()->setText("p / enter / esc:    pause game");      y -= 8;
         mnuControls->addTextBox(font, 10, 16, x, y - (h * 9));
         mnuControls->getTextBoxes().back()->setText("q:                  main menu");
         mnuControls->addBackLink("back");
      }


      //-----Create and set up menus-----

      pMenu_t mnuOpts = pMenu_t(new Menu(0, 0, opts.winW, opts.winH, font, 10, 16, MNU_SUB_OPTIONS, mnuBackground));
      mnuOpts->setColour(0.0, 0.0, 0.0, 1.0);
      mnuOpts->setSelectedItemImage(mnuSel);
      mnuOpts->setItemHeight(30);
      mnuOpts->setTopMargin(140);
      mnuOpts->addItem("Sound Options", false);
      mnuOpts->addItem("", false);
      mnuOpts->addItem("<- Master (0.50) ->");
      mnuOpts->addItem("<- Music  (0.50) ->");
      mnuOpts->addItem("<- SFX    (0.50) ->");
      mnuOpts->addBackLink("back");
      mnuOpts->selectFirst();

      {
         mnuMain_ = pMenu_t(new Menu(0, 0, opts.winW, opts.winH, font, 10, 16, MNU_ROOT_MAIN, mnuBackground));
         mnuMain_->setColour(0.0, 0.0, 0.0, 1.0);
         mnuMain_->setSelectedItemImage(mnuSel);
         mnuMain_->setItemHeight(30);
         mnuMain_->setTopMargin(140);
         mnuMain_->addItem("Main Menu", false);
         mnuMain_->addItem("", false);
         std::stringstream str;
         str << "<- Level " << lvlSelected_ << ": Start! ->";
         mnuMain_->addItem(str.str().data());
         mnuMain_->addItem("Sound Options", mnuOpts);
         mnuMain_->addItem("Controls", mnuControls);
         mnuMain_->addItem("Credits", mnuCredits_);
         mnuMain_->addItem("Quit");
         mnuMain_->selectFirst();
      }

      {
         mnuPaused_ = pMenu_t(new Menu(0, 0, opts.winW, opts.winH, font, 10, 16, MNU_ROOT_PAUSE, mnuBackground));
         mnuPaused_->setColour(0.0, 0.0, 0.0, 1.0);
         mnuPaused_->setSelectedItemImage(mnuSel);
         mnuPaused_->setItemHeight(30);
         mnuPaused_->setTopMargin(140);
         mnuPaused_->addItem("Game Paused", false);
         mnuPaused_->addItem("", false);
         std::stringstream str;
         str << "Level " << opts.level << ": Resume";
         mnuPaused_->addItem(str.str().data());
         mnuPaused_->addItem("Sound Options", mnuOpts);
         mnuPaused_->addItem("Controls", mnuControls);
         mnuPaused_->addItem("Main Menu");
         mnuPaused_->selectFirst();
      }

      Menu::activeMenu = mnuMain_;


      //-----Create and set up info bar-----

      {
         info_ = boost::shared_ptr<InfoBar>(new InfoBar(0, opts.winH - BLOCK_SIZE, opts.winW, BLOCK_SIZE));
         info_->setColour(0.0, 0.0, 0.0, 1.0);

         info_->addTextBox(font, 10, 16, 10, 8, "lblScore");
         info_->getTextBoxes().back()->setText("Gems:");
         info_->addCounter(font, 10, 16, 70, 8, "score");

         info_->addTextBox(font, 10, 16, 110, 8, "lblScore_slash");
         std::stringstream str;
         str << "/ " << requiredScore_;
         info_->getTextBoxes().back()->setText(str.str().data());

         info_->addTextBox(font, 10, 16, 200, 8, "lblTime");
         info_->getTextBoxes().back()->setText("Time:");
         info_->addCounter(font, 10, 16, 260, 8, "time");

         info_->addTextBox(font, 10, 16, 300, 8, "lblTime_slash");
         str.str("");
         str << "/ " << timeLimit_;
         info_->getTextBoxes().back()->setText(str.str().data());
      }


      //-----Create and set up level intro-----

      {
         std::stringstream str;
         str << appPath_ << "/data/images/levelCaption.bmp";
         pImage_t imgIntro = pImage_t(new Image(str.str(), 512, 64));

         lvlIntro_ = boost::shared_ptr<InfoBar>(new InfoBar(0, 0, 0, 0, imgIntro));
         lvlIntro_->setColour(0.0, 0.0, 0.0, 1.0);

         lvlIntro_->addTextBox(font, 10, 16, 50, 50, "lblLevel");
         str.str("");
         str << "Level " << currentLevel_ << ": " << levelName_.data();
         lvlIntro_->getTextBoxes().back()->setText(str.str().data());

         lvlIntro_->stretch(50, 50);
         lvlIntro_->centreX(windowDimension_[0]);
         lvlIntro_->centreY(windowDimension_[1]);
      }
   }
   catch (std::bad_alloc&) {
      // (hoping boost::shared_ptr cleans up here)

      throw xMemAllocFail("Error setting up UI", __FILE__, __LINE__);
   }
}

/*
=====================================
Game::loadLevel
=====================================
*/
void Game::loadLevel(std::string path) {
   try {
      std::ifstream file(path.data());
      if (!file.is_open()) throw xBadFile("Could not read from level data file", __FILE__, __LINE__, path.data());

      char buffer[128];
      for (int i = 0; i < 128; i++) buffer[i] = '\0';

      file.getline(buffer, 128);
      levelName_ = std::string(buffer);

      file.getline(buffer, 128);
      std::stringstream str;
      str << appPath_ << "/" << std::string(buffer);
      musicFile_ = str.str();

      //-----Theme-----
      int theme;
      file.getline(buffer, 128);
      if (sscanf(buffer, "%d", &theme) != 1)
         throw xBadFile("Could not read from level data file", __FILE__, __LINE__, path.data());

      Item::setTheme(theme);

      //-----Time limit-----
      file.getline(buffer, 128);
      if (sscanf(buffer, "%d", &timeLimit_) != 1)
         throw xBadFile("Could not read from level data file", __FILE__, __LINE__, path.data());

      //-----Map dimensions-----
      file.getline(buffer, 128);
      if (sscanf(buffer, "%d %d", &gameDimension_[0], &gameDimension_[1]) != 2)
         throw xBadFile("Could not read from level data file", __FILE__, __LINE__, path.data());

      grid_ = boost::shared_ptr<Grid>(new Grid(gameDimension_[0] / BLOCK_SIZE, gameDimension_[1] / BLOCK_SIZE, BLOCK_SIZE));

      //-----No. gems required-----
      file.getline(buffer, 128);
      if (sscanf(buffer, "%d", &requiredScore_) != 1)
         throw xBadFile("Could not read from level data file", __FILE__, __LINE__, path.data());

      //-----No. item types-----
      int nItemTypes;
      file.getline(buffer, 128);
      if (sscanf(buffer, "%d", &nItemTypes) != 1)
         throw xBadFile("Could not read from level data file", __FILE__, __LINE__, path.data());

      //-----For each type...-----
      for (int type = 0; type < nItemTypes; type++) {
         int nItems;
         char kind[64];

         //-----Quantity of particular item type, and path of .itm file-----
         file.getline(buffer, 128);
         if (sscanf(buffer, "%d %s", &nItems, kind) != 2)
            throw xBadFile("Could not read from level data file", __FILE__, __LINE__, path.data());

         pItem_t item = ItemFactory::createItem(kind);
         item->loadSettings();

         //-----Coordinates of each item-----
         for (int i = 0; i < nItems; i++) {
            int x, y;

            file.getline(buffer, 128);
            if (sscanf(buffer, "%d %d", &x, &y) != 2)
               throw xBadFile("Could not read from level data file", __FILE__, __LINE__, path.data());

            pItem_t temp = ItemFactory::createItem(item->getType(), *item);
            ItemFactory::addItem(temp);
            ItemFactory::getItems().back()->setPosition(x, y, grid_);
         }
      }

      file.close();
   }
   catch (std::bad_alloc&) {
      throw xMemAllocFail("Error loading level", __FILE__, __LINE__);
   }
}

/*
=====================================
Game::~Game
=====================================
*/
Game::~Game() {
//   SdlAudio::freeMusic();
//   SdlAudio::freeAllSounds();
   ItemFactory::reset();
   glDeleteTextures(Image::getNumGlTextures(), Image::getGlTextures());
}
