#ifndef ModelClass_h
#define ModelClass_h

#include "InputClass.h"

class ModelClass : public InputClass
{
   public:
      std::string GetType()
      {
         return "Model" ;
      }
};

#endif
