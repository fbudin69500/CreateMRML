#ifndef _VolumeClass_h
#define _VolumeClass_h

#include "ColorableClass.h"


class VolumeClass : public ColorableClass
{
   public:
      VolumeClass()
      {
         m_VolumeType = "scalar" ;
         m_LabelMap = false ;
      }
      void Print()
      {
         ColorableClass::Print() ;
         std::cout << "Image Type: " << m_VolumeType << std::endl ;
      }
      void LabelMap( bool label )
      {
        m_LabelMap = label ;
      }
      bool IsLabelMap()
      {
        return m_LabelMap ;
      }
      std::string GetType()
      {
         return "Volume" ;
      }
      int SetVolumeType( std::string type )
      {
         if( type.compare( "scalar" )
             && type.compare( "DTI" )
             && type.compare( "DWI" )
             && type.compare( "vector" )
           )
         {
            return 1 ;
         }
         m_VolumeType = type ;
         return 0 ;
      }
      std::string GetVolumeType()
      {
         return m_VolumeType ;
      }
      void PrintVolumeTypes()
      {
         std::cout << "scalar" << std::endl ;
         std::cout << "DTI" << std::endl ;
         std::cout << "DWI" << std::endl ;
         std::cout << "vector" << std::endl ;
      }
   private:
      std::string m_VolumeType ;
      bool m_LabelMap ;
};

#endif
