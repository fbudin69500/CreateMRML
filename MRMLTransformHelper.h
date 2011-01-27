#ifndef _MRMLTransformHelper_h
#define _MRMLTransformHelper_h

#include "MRMLNodeHelper.h"

class MRMLTransformHelper : public MRMLNodeHelper
{
   public:
      std::string GetType()
      {
         return "Transform" ;
      }
};

#endif
