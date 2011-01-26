#ifndef _FiducialClass_h
#define _FiducialClass_h

#include "ColorableClass.h"

class FiducialClass : public ColorableClass
{
public:

  FiducialClass();
  std::string GetType() ;
  void Print() ;
  void SetId( std::string ) ;
  std::string GetId() ;
  void SetLabelText( std::string ) ;
  std::string GetLabel() ;
  void SetPosition( float , float , float ) ;
  std::vector< float > GetPosition() ;
  void SetOrientation( float , float , float , float ) ;
  std::vector< float > GetOrientation() ;
  int SetSelectedColor( float , float , float ) ;
  std::vector< float > GetSelectedColor() ;
  void Selected( bool ) ;
  bool IsSelected() ;
  void Visibility( bool ) ;
  bool IsVisible() ;
  void SetTextScale( double ) ;
  double GetTextScale() ;
private:
  std::string m_Id ;
  std::string m_Label ;
  float m_X, m_Y, m_Z ;
  float m_oW, m_oX, m_oY, m_oZ ;
  bool m_Selected ;
  bool m_Visible ;
  double m_TextScale ;
  std::vector< float > m_RGB ;
};

#endif
