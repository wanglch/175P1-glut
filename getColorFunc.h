

#ifndef GETCOLORFUNC_H
#define GETCOLORFUNC_H

#include "types.h"
#include "OpenGLhandler.h"
#include <math.h>

class getColorFunc{
public:
  getColorFunc(pnt3 viewVec, float viewDist): view(viewVec), viewDistance(viewDist){};

  pnt3 operator()(pnt3 location, pnt3 norm){  
    //light vector
    pnt3 l = ~(OpenGLhandler::getLpos() - location);
    //reflection vector
    pnt3 r = l - (2 * ((norm) << l));
            //Ka*Ia +
    pnt3 c= OpenGLhandler::getAmbiant() * OpenGLhandler::getIa() + 
            //Il / (||f - p|| + K) *
            OpenGLhandler::getIl() / (viewDistance + OpenGLhandler::getK()) * 
            //(kd * (L dot n) +
            (OpenGLhandler::getDiffuse() * (l * norm) + 
            //ks * (r dot v)^n)
            OpenGLhandler::getSpecular() * pow(posORzero(r * view), OpenGLhandler::getLightSize()));
  
    //in case color is is not an element of [0,1]
    flatten(c);

    return c;
  };

private:
  const pnt3 view;
  const float viewDistance;
};

#endif

