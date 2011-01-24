#ifndef TransformClass_h
#define TransformClass_h

#include "InputClass.h"

class TransformClass : public InputClass
{
   public:
      std::string GetType()
      {
         return "Transform" ;
      }
};

#endif
