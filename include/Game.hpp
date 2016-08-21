/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#ifndef __GAME_HPP__
#define __GAME_HPP__


#include <vector>
#include <stdio.h>

#include "Grid.hpp"
#include "ItemFactory.hpp"
#include "Player.hpp"
#include "Image.hpp"
#include "AnimSequence.hpp"
#include "InfoBar.hpp"
#include "Menu.hpp"


enum gameState_t {
   ST_MAIN_MNU,
   ST_PAUSE_MNU,
   ST_LEVEL_START,
   ST_RUNNING,
   ST_END,
   ST_QUIT
};

struct gameInitOpt_t {
   int winW;
   int winH;
   int level;
   int maxLevel;
   gameState_t state;
};

class Game {
   public:
      Game(const std::string& appPath);
      ~Game();

      void init(gameInitOpt_t opts);

      void mainLoop();
      void render();

      void keyDown(int key, int x, int y);
      inline void keyUp(int key, int x, int y);

      inline int getFrameRate() const;
      inline const int* getGameDimensions() const;
      inline gameState_t getState() const;
      inline int getCurrentLevel() const;
      inline int getRequiredScore() const;
      inline const gameInitOpt_t& getNextGameOpt() const;

   private:
      static const int BLOCK_SIZE = 32;

      enum mnu_enum {
         MNU_ROOT_MAIN,
         MNU_ROOT_PAUSE,
         MNU_SUB_OPTIONS,
         MNU_SUB_CREDITS,
         MNU_SUB_CONTROLS
      };

      std::string                   appPath_;
      gameState_t                   state_;
      int                           windowDimension_[2];
      int                           gameDimension_[2];
      boost::shared_ptr<Grid>       grid_;
      int                           viewPos_[2];
      float                         viewCentre_[2];
      boost::shared_ptr<Player>     player_;
      pItem_t                       exit_;
      bool                          keyState_[256];   // Used for both glut special keys and for other keys - possible clashes
      int                           frameRate_;
      boost::shared_ptr<InfoBar>    info_;
      boost::shared_ptr<InfoBar>    lvlIntro_;
      pMenu_t                       mnuMain_, mnuPaused_, mnuCredits_;
      int                           lvlSelected_;

      std::string levelName_;
      std::string musicFile_;
      int currentLevel_;
      int sessionMaxLevel_;
      int nLevels_;
      int requiredScore_;

      int timeStarted_;
      int timeLimit_;
      int timePaused_;
      int elapsedPaused_;
      int totalElapsedPaused_;

      static float musicVol;
      static float masterVol;
      static float sfxVol;

      gameInitOpt_t nextGame_;

      void scrollLevel();
      void renderImage(boost::shared_ptr<Image> image) const;
      void updateScene();
      void centreView();
      void keyboard();
      void loadItems(std::string itms);
      void loadLevel(std::string path);
      void levelComplete();
      void updateVolume();
      void updateMenu();
      void saveProgress(int level);
      void loadProgress();
      void renew(const gameInitOpt_t& opts);
      void uiSetup(const gameInitOpt_t& opts);
      void findNumLevels();
};

inline const gameInitOpt_t& Game::getNextGameOpt() const { return nextGame_; }

inline int Game::getFrameRate() const { return frameRate_; }

inline void Game::keyUp(int key, int x, int y) { keyState_[key] = false; }

inline const int* Game::getGameDimensions() const { return gameDimension_; }

inline gameState_t Game::getState() const { return state_; }

inline int Game::getCurrentLevel() const { return currentLevel_; }

inline int Game::getRequiredScore() const { return requiredScore_; }


#endif /*!__GAME_HPP__*/
