#ifndef _ModelClass_h
#define _ModelClass_h

#include "ColorableClass.h"

class ModelClass : public ColorableClass
{
   public:
      std::string GetType()
      {
         return "Model" ;
      }
};

#endif
