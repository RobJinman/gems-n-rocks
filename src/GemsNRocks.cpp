/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#include <stdio.h>
#include <iostream>
#include <boost/shared_ptr.hpp>

#include "SdlAudio.hpp"
#include "AudioExceptions.hpp"
#include "Image.hpp"
#include "Game.hpp"
#include "xException.hpp"
#include "GemsNRocks.hpp"


boost::shared_ptr<Game> game = boost::shared_ptr<Game>();


void GemsNRocks::displayFunc() { game->render(); }

/*
=====================================
GemsNRocks::mainLoop
=====================================
*/
void GemsNRocks::mainLoop() {
   try {
      game->mainLoop();

      if (game->getState() == ST_END) {
         gameInitOpt_t gameOpt = game->getNextGameOpt();

         game.reset();
         try {
            game = boost::shared_ptr<Game>(new Game(m_appPath));
         }
         catch (std::bad_alloc&) {
            throw xMemAllocFail("Could not create Game", __FILE__, __LINE__);
         }

         game->init(gameOpt);
      }
   }
   catch (xException& ex) {
      ex.print(&m_log);
      exit(1);
   }
   catch (...) {
      m_log << "Unknown Error\n";
   }
}

/*
=====================================
GemsNRocks::specialKeyDown
=====================================
*/
void GemsNRocks::specialKeyDown(int key, int x, int y) {
   game->keyDown(key, x, y);
}

/*
=====================================
GemsNRocks::specialKeyUp
=====================================
*/
void GemsNRocks::specialKeyUp(int key, int x, int y) {
   game->keyUp(key, x, y);
}

/*
=====================================
GemsNRocks::keyDown
=====================================
*/
void GemsNRocks::keyDown(unsigned char key, int x, int y) {
   if (key == 13) {
      game->keyDown(GLUT_KEY_F1, x, y);   // Pass the Enter key code through as F1
      return;
   }

   if (key == 32) {
      game->keyDown(GLUT_KEY_F2, x, y);   // Change spacebar to F2
      return;
   }

   if (key == 'f') return; // Because 'f' clashes with ->

   game->keyDown(key, x, y);
}

/*
=====================================
GemsNRocks::keyUp
=====================================
*/
void GemsNRocks::keyUp(unsigned char key, int x, int y) {
   if (key == 13) {
      game->keyUp(GLUT_KEY_F1, x, y);   // Pass the Enter key code through as F1
      return;
   }

   if (key == 32) {
      game->keyUp(GLUT_KEY_F2, x, y);   // Change spacebar to F2
      return;
   }

   if (key == 'f') {
      m_log << "Frame Rate: " << game->getFrameRate() << "\n";
      return; // Because 'f' clashes with ->
   }

   game->keyUp(key, x, y);
}

/*
=====================================
GemsNRocks::init
=====================================
*/
void GemsNRocks::init(int w, int h) throw() {
   try {
      try {
         game = boost::shared_ptr<Game>(new Game(m_appPath));
      }
      catch (std::bad_alloc&) {
         throw xMemAllocFail("Could not create Game", __FILE__, __LINE__);
      }
   }
   catch (xException& e) {
      e.print(&m_log);
      exit(1);
   }
   catch (...) {
      m_log << "Unknown Error\n";
      exit(1);
   }

   gameInitOpt_t opts;
   opts.winW = w;
   opts.winH = h;
   opts.level = 1;
   opts.maxLevel = 1;
   opts.state = ST_MAIN_MNU;

   try {
      SdlAudio::init();
      game->init(opts);
   }
   catch (xAudioException& ex) {
      ex.print(&m_log);
      SdlAudio::quit();
      exit(1);
   }
   catch (xFileIO& ex) {
      ex.print(&m_log);
      m_log << "\"" << ex.getFileName() << "\"\n";
      SdlAudio::quit();
      exit(1);
   }
   catch (xException& ex) {
      ex.print(&m_log);
      SdlAudio::quit();
      exit(1);
   }
   catch (std::exception& e) {
      m_log << e.what() << "\n";
      SdlAudio::quit();
      exit(1);
   }
   catch (...) {
      m_log << "Unknown Error\n";
      SdlAudio::quit();
      exit(1);
   }
}
