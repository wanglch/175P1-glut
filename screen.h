/* 
 * File:   screen.h
 * Author: btacklind
 *
 * Created on October 20, 2015, 12:36 PM
 */

#ifndef SCREEN_H
#define	SCREEN_H

#include "types.h"
#include "line.h"
#include "surface.h"
#include <list>

using namespace std;

class screen {
public:
  screen(int x, int y, int ofX, int ofY, pnt3 vec, float viewDist, void (*mkPix)(int, int, pnt3));
  inline screen(){};
  screen(const screen& orig);
  virtual ~screen();

  static void freeAll();
  
  static void bufferAllScreens();
  void bufferObjects();
  
  inline static screen* getLastScreen(){return screenList.back();};
  inline int getScreenWidth (){return width; };
  inline int getScreenHeight(){return height;};

  inline pnt3 getNormal(){return normal;};
  void setNormal(const pnt3& p);
  
  pnt3 getColor(pnt3 location, pnt3 norm);
  
private:
  static list<screen*> screenList;

  void fillLine(cpnt& a, cpnt& b);
  
  pntf convert3dPoint(pnt3 p);
  
  //bool compareSurfaces(const surface& first, const surface& second);
  class compareSurfaces{
  public:
    compareSurfaces(pnt3 view):viewVec(view){};

    bool operator()(surface*& first, surface*& second);

  private:
    const pnt3 viewVec;
  };

  void drawLine(line* l);
  
  float viewDistance;
  
  unsigned int width;
  unsigned int height;
  pnt3 normal, outy, outx;
  const int offsetX=0, offsetY=0;
  
  inline void MakePix(int x, int y, pnt3 color){mp(x+offsetX, y+offsetY, color);}; 

  void (*mp)(int, int, pnt3);
  
  
  struct holder{
    pnt3 real;
    pnt rel;
    pnt3 vec;
  };

};

#endif	/* SCREEN_H */

