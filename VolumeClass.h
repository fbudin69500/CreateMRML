#ifndef VolumeClass_h
#define VolumeClass_h

#include "InputClass.h"
#include <vtkMRMLColorTableNode.h>

//Copied from vtkMRMLColorTableNode.h
  /// 
  /// The list of valid table types
 
  /// Grey - greyscale ramp
  /// Iron - neutral
  /// Rainbow - red-orange-yellow-blue-indigo-violet
  /// Ocean - bluish ramp
  /// Desert - orange ramp
  /// InvGrey - inverted greyscale ramp
  /// ReverseRainbow - inverted Rainbow
  /// FMRI - fMRI map
  /// FMRIPA - fMRI Positive Activation map
  /// Labels - the Slicer2 FullRainbow editor labels
  /// Random - 255 random colors
  /// User - user defined in the GUI
  /// File - read in from file
  /// Red - red ramp (like greyscale but with red, meant for layering with cyan)
  /// Green - green ramp (like greyscale but with green, layering with magenta)
  /// Blue - blue ramp (like greyscale but with blue, layering with yellow)
  /// Yellow - yellow ramp (complementary ramp to blue, layering yeilds gray)
  /// Cyan - cyan ramp (complementary ramp to red, layering yeilds gray)
  /// Magenta - magenta ramp (complementary ramp to green, layering yeilds gray)
  /// Warm# - ramps of warm colors that are complimentary to Cool#
  /// WarmShade# - ramps of warm colors with variation in value that are
  ///       complimentary to CoolShade# 
  /// WarmTint# - ramps of warm colors with variation in saturation that are
  ///       complimentary to CoolTint# 
  /// Cool# - ramps of cool colors that are complimentary to Warm#
  /// CoolShade# - ramps of cool colors with variation in value that are
  ///       complimentary to WarmShade# 
  /// CoolTint# - ramps of cool colors with variation in saturation that are
  ///       complimentary to WarmSTint# 

/*
      FullRainbow = 0,
      Grey = 1,
      Iron = 2,
      Rainbow = 3,
      Ocean = 4,
      Desert = 5,
      InvGrey = 6,
      ReverseRainbow = 7,
      FMRI = 8,
      FMRIPA = 9,
      Labels = 10,
      Random = 12,
      User = 13,
      File = 14,
      Red = 15,
      Green = 16,
      Blue = 17,
      Yellow = 18,
      Cyan = 19,
      Magenta = 20,
      Warm1 = 21,
      Warm2 = 22,
      Warm3 = 23,
      Cool1 = 24,
      Cool2 = 25,
      Cool3 = 26,
      WarmShade1 = 27,
      WarmShade2 = 28,
      WarmShade3 = 29,
      CoolShade1 = 30,
      CoolShade2 = 31,
      CoolShade3 = 32,
      WarmTint1 = 33,
      WarmTint2 = 34,
      WarmTint3 = 35,
      CoolTint1 = 36,
      CoolTint2 = 37,
      CoolTint3 = 38
*/


class VolumeClass : public InputClass
{
   public:
      VolumeClass()
      {
         colorNode = vtkMRMLColorTableNode::New() ;
         colorNode->SetTypeToGrey() ;
         m_VolumeType = "scalar" ;
      }
      ~VolumeClass()
      {
         colorNode->Delete() ;
      }
      void Print()
      {
         InputClass::Print() ;
         std::cout << "Image Type: " << m_VolumeType << std::endl ;
         std::cout << "Color: " << colorNode->GetTypeAsIDString() << std::endl ;
      }
      std::string GetType()
      {
         return "Volume" ;
      }
      std::string GetColor()
      {
         return colorNode->GetTypeAsIDString() ;
      }
      void SetColor( int color )
      {
         if( color >= colorNode->GetFirstType() && color <= colorNode->GetLastType() )
         {
           colorNode->SetType( color ) ;
         }
      }
      int GetFirstColor()
      {
         return colorNode->GetFirstType() ;
      }
      int GetLastColor()
      {
         return colorNode->GetLastType() ;
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
      void PrintColors()
      {
         std::cout<< "FullRainbow = 0" << std::endl ;
         std::cout<< "Grey = 1" << std::endl ;
         std::cout<< "Iron = 2" << std::endl ;
         std::cout<< "Rainbow = 3" << std::endl ;
         std::cout<< "Ocean = 4" << std::endl ;
         std::cout<< "Desert = 5" << std::endl ;
         std::cout<< "InvGrey = 6" << std::endl ;
         std::cout<< "ReverseRainbow = 7" << std::endl ;
         std::cout<< "FMRI = 8" << std::endl ;
         std::cout<< "FMRIPA = 9" << std::endl ;
         std::cout<< "Labels = 10" << std::endl ;
         std::cout<< "Random = 12" << std::endl ;
         std::cout<< "User = 13" << std::endl ;
         std::cout<< "File = 14" << std::endl ;
         std::cout<< "Red = 15" << std::endl ;
         std::cout<< "Green = 16" << std::endl ;
         std::cout<< "Blue = 17" << std::endl ;
         std::cout<< "Yellow = 18" << std::endl ;
         std::cout<< "Cyan = 19" << std::endl ;
         std::cout<< "Magenta = 20" << std::endl ;
         std::cout<< "Warm1 = 21" << std::endl ;
         std::cout<< "Warm2 = 22" << std::endl ;
         std::cout<< "Warm3 = 23" << std::endl ;
         std::cout<< "Cool1 = 24" << std::endl ;
         std::cout<< "Cool2 = 25" << std::endl ;
         std::cout<< "Cool3 = 26" << std::endl ;
         std::cout<< "WarmShade1 = 27" << std::endl ;
         std::cout<< "WarmShade2 = 28" << std::endl ;
         std::cout<< "WarmShade3 = 29" << std::endl ;
         std::cout<< "CoolShade1 = 30" << std::endl ;
         std::cout<< "CoolShade2 = 31" << std::endl ;
         std::cout<< "CoolShade3 = 32" << std::endl ;
         std::cout<< "WarmTint1 = 33" << std::endl ;
         std::cout<< "WarmTint2 = 34" << std::endl ;
         std::cout<< "WarmTint3 = 35" << std::endl ;
         std::cout<< "CoolTint1 = 36" << std::endl ;
         std::cout<< "CoolTint2 = 37" << std::endl ;
         std::cout<< "CoolTint3 = 38" << std::endl ;
      }
   private:
      vtkMRMLColorTableNode *colorNode ;
      std::string m_VolumeType ;
};

#endif
