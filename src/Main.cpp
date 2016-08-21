/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2010
 */

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
//#include "adpcppf.h"
#include <GL/glut.h>
//#include <windows.h>
#include <boost/shared_ptr.hpp>

#include "GemsNRocks.hpp"


//ApplicationId GemsNRocks::m_id = ApplicationId(0xA5C8E583,0x372C47A0,0x8780624E,0x19B2251C);
//ApplicationId GemsNRocks::m_id = ApplicationId(ADP_DEBUG_APPLICATIONID);
GemsNRocks* app = NULL;

const int w = 768;
const int h = 450;

bool logging = false;
std::ostream* errStream = NULL;
std::string appPath;

void specialKeyDown(int key, int x, int y) { app->specialKeyDown(key, x, y); }
void specialKeyUp(int key, int x, int y) { app->specialKeyUp(key, x, y); }
void keyDown(unsigned char key, int x, int y) { app->keyDown(key, x, y); }
void keyUp(unsigned char key, int x, int y) { app->keyUp(key, x, y); }
void displayFunc() { app->displayFunc(); }
void mainLoop() { app->mainLoop(); }

/*
=====================================
cleanUp
=====================================
*/
void cleanUp() {
   try {
//      app->EndEvent();

      if (logging) {
         std::ofstream* temp = dynamic_cast<std::ofstream*>(errStream);
         temp->close();
      }
   }
/*
   catch(AdpException& e) {
      if (logging) (*errStream) << "Caught exception: " << e.what() << "\n";
   }*/
   catch(...) {}
}

/*
=====================================
doOnce
=====================================
*/
void doOnce() {
   static bool b = false;

   if (!b) {
      app = new GemsNRocks(appPath, *errStream);
//      app->BeginEvent();
      app->init(w, h);

      atexit(cleanUp);

      glutSpecialFunc(specialKeyDown);
      glutSpecialUpFunc(specialKeyUp);
      glutDisplayFunc(displayFunc);
      glutKeyboardFunc(keyDown);
      glutKeyboardUpFunc(keyUp);
      glutDisplayFunc(displayFunc);
      glutIdleFunc(mainLoop);

      b = true;
   }
}

/*
=====================================
renderText
=====================================
*/
void renderText(const std::string& text, int x, int y, float sf) {
   glMatrixMode(GL_MODELVIEW);

   glPushMatrix();
   glLoadIdentity();

   for (unsigned int i = 0; i < text.length(); i++) {
      glPushMatrix();

      glTranslatef(x, y, 0.0);
      glScalef(sf, sf, 1.0);

      glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, text[i]);
      x += glutStrokeWidth(GLUT_STROKE_MONO_ROMAN, text[i]) * sf;

      glPopMatrix();
   }

   glPopMatrix();
}

/*
=====================================
loadScreenRender
=====================================
*/
void loadScreenRender() {/*
   static int n = 0;
   n++;

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   renderText("Authorizing, please wait...", 200, 250, 0.15);
   glutSwapBuffers();

   if (n > 100) */glutIdleFunc(doOnce);
}

/*
=====================================
main
=====================================
*/
int main(int argc, char** argv) {

   //------------Get path from registry------------
/*
   char buf[128];
   DWORD bufSize = 128;
   for (unsigned int i = 0; i < 128; i++) buf[i] = '\0';

   HKEY hkey;
   if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "Software\\Gems'n'Rocks", 0, KEY_QUERY_VALUE, &hkey) == ERROR_SUCCESS
      && RegQueryValueEx(hkey, "InstallationPath", NULL, NULL, (LPBYTE)buf, &bufSize) == ERROR_SUCCESS) {

      appPath = std::string(buf);
   }
   else {
      std::cerr << "Could not read installation path from registry---assuming current directory\n";
      GetCurrentDirectory(128, buf);
      appPath = std::string(buf);
   }
*/
  appPath = "."; // TODO

   //------------open error log------------
/*
   std::stringstream strLog;
   strLog << appPath << "/log.txt";
   std::ofstream log(strLog.str().data());
   if (log.is_open()) {
      errStream = &log;
      logging = true;
   }
   else*/
      errStream = &std::cerr;


   //------------glut init------------
   glutInitWindowSize(w, h);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
   glutInit(&argc, argv);
   glutCreateWindow("[v1.2.1] Gems'n'Rocks by Rob Jinman");

   glutDisplayFunc(loadScreenRender);
   glutIdleFunc(loadScreenRender);


   //------------openGL init------------
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0, w, 0, h, -1.0, 1.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   glEnable(GL_BLEND);
   glEnable(GL_ALPHA_TEST);
   glEnable(GL_TEXTURE_2D);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   glClearColor(0.0, 0.0, 0.0, 1.0);


   //------------launch game------------
   try {
      glutMainLoop();
   }
/*
   catch (AdpException& e) {
      switch (e.code()) {
         case 6:
            (*errStream) << "Caught an unauthorized exception: " << e.what() << "\n";
            MessageBox(NULL, "This software is not authorized to run", "Authorization failed", MB_OK);
         break;
         default:
            (*errStream) << "Caught exception: " << e.what() << "\n";
      }

      exit(1);
   }*/
   catch (std::bad_alloc& e) {
      (*errStream) << "Could not create GemsNRocks object: " << e.what() << "\n";

      exit(1);
   }
   catch (...) {
      (*errStream) << "Could not create GemsNRocks object: Unknown Error\n";

      exit(1);
   }

   return 0;
}
