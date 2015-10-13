#include <GL/glut.h>
#include "OpenGLhandler.h"
#include "userInterface.h"

#include <iostream>
#include <list>
using namespace std;

float* OpenGLhandler::PixelBuffer;
OpenGLhandler::algMode OpenGLhandler::aMode;
int OpenGLhandler::MainWindow;
OpenGLhandler::drawMode OpenGLhandler::dMode;
int OpenGLhandler::xMin;
int OpenGLhandler::yMin;
int OpenGLhandler::xMax;
int OpenGLhandler::yMax;
unsigned int OpenGLhandler::width;
unsigned int OpenGLhandler::height;

void OpenGLhandler::init(int* argc, char** argv)
{
  if (*argc == 3){
    int val1 = atoi(argv[1]);
    int val2 = atoi(argv[2]);
    if(val1 > 0 && val2 > 0){
      width = val1;
      height = val2;
    }
    else{
      width = 200;
      height = 200;
    }
  }
  else{
    width = 200;
    height = 200;
  }

  
  PixelBuffer = new float[width * height * 3];

  aMode = DDA;
  dMode = fill;
  xMin = 0;
  yMin = 0;
  xMax = width;
  yMax = height;

  //Draw shapes
  bufferObjects();


  glutInit(argc, argv);
  glutInitDisplayMode(GLUT_SINGLE);

  glutInitWindowSize(width, height);

  glutInitWindowPosition(100, 100);

  MainWindow = glutCreateWindow("Blake Tacklind - 997051049 - Project 1");
  glClearColor(0, 0, 0, 0);
  glutDisplayFunc(display);

  glutKeyboardFunc(Keystroke);
  
  glutMainLoop();

  return;
}

/*
 */
void OpenGLhandler::onClose(void){
  delete [] PixelBuffer;
  obj::freeAll();
  userInterface::endUI();
}

void OpenGLhandler::Keystroke(unsigned char key, int x, int y){
  if(key == 27){
    onClose();
    glutDestroyWindow(MainWindow);
  }
  else userInterface::keypressed(key);
}

void OpenGLhandler::MakePix(int x, int y){
  PixelBuffer[(y*width + x) * 3 ]    = 1;
  PixelBuffer[(y*width + x) * 3 + 1] = 1;
  PixelBuffer[(y*width + x) * 3 + 2] = 1;
}

void OpenGLhandler::display(){
  glClear(GL_COLOR_BUFFER_BIT);
  glLoadIdentity();

  glDrawPixels(width, height, GL_RGB, GL_FLOAT, PixelBuffer);

  glEnd();
  glFlush();

  return;
}

void OpenGLhandler::clearBuffer(){
  for (int i = 0; i < width*height*3; i++)
    PixelBuffer[i] = 0.0;
}

/*
 * Draw objects depending on draw mode
 */
void OpenGLhandler::bufferObjects(drawMode m){
  clearBuffer();
  obj::clipObjects(xMin, xMax, yMin, yMax);


  //Draw object vertexes
  if (m == points){
    for(int i = 0; i < obj::getNumClippedObjects(); i++){
      obj o = obj::getClippedObject(i);
      
      for(int j = 0; j < o.getNumPoints(); j++){
        pnt p = o.getPoints()[j];
        MakePix(p.x, p.y);
      }
    }
  }
  //Draw object with wireframe
  else if (m == lines){
    for(int i = 0; i < obj::getNumClippedObjects(); i++){
      obj o = obj::getClippedObject(i);
      pnt p1;
      pnt p2 = o.getPoints()[0];
      
      for(int j = 1; j < o.getNumPoints(); j++){
        p1 = p2;
        p2 = o.getPoints()[j];
        drawLine(new line(p1,p2,aMode==BA));
      }
     
      //close shape (only if more then a line)
      if(o.getNumPoints() > 2){
        drawLine(new line(o.getPoints()[0], p2,aMode==BA));
      }
      //draw object if its a line
      else if (o.getNumPoints() == 1)
        MakePix(p2.x, p2.y);
    }
  }
  //Rasterize objects
  else if (m == fill){
    //iterate through all objects (after clipping)
    for(int i = 0; i < obj::getNumClippedObjects(); i++){
      obj::getClippedObject(i).fill(MakePix,  aMode==BA);
    }
  }
  
  obj::cleanClippedObects();
}

void OpenGLhandler::drawLine(line* l){
  const int dist = l->getNumPoints();
  for(int i = 0; i <= dist; i++){
    pnt p = l->getPoint(i);
    MakePix(p.x, p.y);
  }
  
  delete l;
}


void OpenGLhandler::reDraw(){
  glutPostRedisplay();
}

const char* OpenGLhandler::getDrawMode(){
  if (dMode == points) return "Points";
  if (dMode == lines ) return "Lines";
  if (dMode == fill  ) return "Fill";
}

const char* OpenGLhandler::getAlgMode(){
  if (aMode == DDA) return "DDA";
  if (aMode == BA ) return "BA";
}

void OpenGLhandler::tglDrawMode(){
       if (dMode == points) dMode = lines;
  else if (dMode == lines ) dMode = fill;
  else if (dMode == fill  ) dMode = points;
}

void OpenGLhandler::tglAlgMode(){
       if (aMode == DDA) aMode = BA;
  else if (aMode == BA ) aMode = DDA;
}
