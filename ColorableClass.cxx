#include "ColorableClass.h"

ColorableClass::ColorableClass()
{
   colorNode = vtkMRMLColorTableNode::New() ;
   colorNode->SetTypeToGrey() ;
   m_Opacity = 1.0 ;
   m_R = .5 ;
   m_G = .5 ;
   m_B = .5 ;
}

ColorableClass::~ColorableClass()
{
   colorNode->Delete() ;
}

int ColorableClass::SetOpacity( double value )
{
   int error ;
   CORRECTVALUEMACRO( value , error ) ;
   if( error )
   {
      return 1 ;
   }
   m_Opacity = value ;
   return 0 ;
}

float ColorableClass::GetOpacity()
{
   return m_Opacity ;
}

int ColorableClass::SetRGB( double R , double G , double B )
{
   int error1 , error2 , error3 ;
   CORRECTVALUEMACRO( R , error1 ) ;
   CORRECTVALUEMACRO( G , error2 ) ;
   CORRECTVALUEMACRO( R , error3 ) ;
   if( error1 || error2 || error3 )
   {
      return 1 ;
   }
   m_R = R ;
   m_G = G ;
   m_B = B ;
   return 0 ;
}


double ColorableClass::GetR()
{
   return m_R ;
}

double ColorableClass::GetG()
{
   return m_G ;
}


double ColorableClass::GetB()
{
   return m_B ;
}

void ColorableClass::Print()
{
   InputClass::Print() ;
   std::cout << "Color: " << colorNode->GetTypeAsIDString() << std::endl ;
}

std::string ColorableClass::GetColor()
{
   return colorNode->GetTypeAsIDString() ;
}

void ColorableClass::SetColor( int color )
{
   if( color >= colorNode->GetFirstType() && color <= colorNode->GetLastType() )
   {
      colorNode->SetType( color ) ;
      m_ColorString = "" ;
   }
}

void ColorableClass::SetColorString( std::string color )
{
   m_ColorString = color ;
}

const char* ColorableClass::GetColorString()
{
   return m_ColorString.c_str() ;
}

int ColorableClass::GetFirstColor()
{
   return colorNode->GetFirstType() ;
}

int ColorableClass::GetLastColor()
{
   return colorNode->GetLastType() ;
}

void ColorableClass::PrintColors()
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
