#include <vtkMRMLScene.h>
#include <vtkMRMLLinearTransformNode.h>
#include <vtkMRMLTransformStorageNode.h>
#include <vtkMRMLScalarVolumeNode.h>
#include <vtkMRMLVolumeArchetypeStorageNode.h>
#include <vtkMRMLScalarVolumeDisplayNode.h>
#include <vtkMRMLDiffusionWeightedVolumeDisplayNode.h>
#include <vtkMRMLVectorVolumeNode.h>
#include <vtkMRMLVectorVolumeDisplayNode.h>
#include <vtkMRMLDiffusionTensorVolumeNode.h>
#include <vtkMRMLDiffusionTensorVolumeDisplayNode.h>
#include <vtkMRMLDiffusionWeightedVolumeNode.h>
#include <vtkMRMLNRRDStorageNode.h>
#include <vtkMRMLModelStorageNode.h>
#include <vtkMRMLModelNode.h>
#include <vtkMRMLDisplayNode.h>
#include <vtkMRMLColorTableNode.h>
#include <vtkMRMLColorTableStorageNode.h>

#include "ModelClass.h"
#include "TransformClass.h"
#include "VolumeClass.h"
#include <vector>
#include <string.h>
/*
//files
-t transform
-v volume
-m model
-h help
-p parent
-n Node name
-f File name
-cc color code
-dc displayed color
-op opacity
-y volume type
format: 
t: transform
v: volume
m: model
-t or -v or -m or -h
if -t or -v or -m
    -f compulsory (file)
    -p optional (default: no parent) ( parent)
    -n optional (default: name of the file stripped from extension and directory) (node name)
if -v
    -c optional (default: grey)
    -y type

If error: prints exec_name -h to print help
//visualization
****
*/

void PrintHelp( const char* arg0 , bool extended )
{
   std::cout << "usage: " << std::endl ;
   std::cout << arg0 << " SceneFileName [-t/-v/-m][-f/-p/-n][-dc/-op][-cc][-y][-h]" << std::endl ;
   if( !extended )
   {
      return ;
   }
   std::cout << std::endl ;
   std::cout << "-t: Adds a transform" << std::endl ;
   std::cout << "-v: Adds a volume" << std::endl ;
   std::cout << "-m: Adds a model" << std::endl ;
   std::cout << "-h: Prints help" << std::endl ;
   std::cout << std::endl ;
   std::cout << "For each object added to the scene:" << std::endl ;
   std::cout << "-f FileName (compulsory!)[relative path fomr object file to scene file]" << std::endl ;
   std::cout << "-p ParentNodeName (default: no parent)" << std::endl ;
   std::cout << "-n NodeName (default: name of the file stripped from extension and directory)" << std::endl ;
   std::cout << std::endl ;
   std::cout << "If node is a volume or a model:" << std::endl ;
   std::cout << "-cc ColorCode (default: 1 [grey])" << std::endl ;
   std::cout << "-op Opacity (default: 1)" << std::endl ;
   std::cout << "-dc r,g,b (default: 0.5,0.5,0.5)" << std::endl ;
   std::cout << std::endl ;
   std::cout << "If node is a volume:" << std::endl ;
   std::cout << "-y VolumeType (default: scalar)" << std::endl ;
   std::cout << "-l: Label map (only for scalar volumes)" << std::endl ;
   std::cout << std::endl ;
   std::cout << "Color Table:" << std::endl ;
   VolumeClass volume ;
   volume.PrintColors() ;
   std::cout << std::endl ;
   std::cout << "Volume types:" << std::endl ;
   volume.PrintVolumeTypes() ;
}


int ReadCommonSubArguments( std::string arg , std::string value , InputClass* ptr )
{
   if( !arg.compare("-f") )
   {
      if( ptr->GetFileName().compare( "" ) )
      {
        std::cerr << "Error: Multiple filenames for one object" << std::endl ;
         return 1 ;
      }
      if( value[ 0 ] == '/' || ( value.size() >= 3  && value[ 1 ] == ':'  && value[ 2 ] == '\\' ) )
      {
        std::cerr << "Error: Path to file has to be relative" << std::endl ;
      }
      ptr->SetFileName( value ) ;
      return -1 ;
   }
   else if( !arg.compare( "-p" ) )
   {
      if( ptr->GetParentName().compare( "" ) )
      {
        std::cerr << "Error: Multiple parents for one object" << std::endl ;
         return 1 ;
      }
      ptr->SetParentName( value ) ;
      return -1 ;
   }
   else if( !arg.compare( "-n" ) )
   {
      if( ptr->GetNodeName().compare( "" ) )
      {
        std::cerr << "Error: Multiple node names for one object" << std::endl ;
         return 1 ;
      }
      ptr->SetNodeName( value ) ;
      return -1 ;
   }
   else if( !arg.compare( "-t" )
         || !arg.compare( "-v" )
         || !arg.compare( "-m" )
         || !arg.compare( "-h" )
          )
   {
      return 2 ;
   }
   return 0 ;
}


int ReadColorCodeArgument( bool &colorCodeSet ,
                        const char *argv ,
                        ColorableClass* ptr
                      )
{
  if( colorCodeSet )
  {
    std::cerr << "Error: Multiple color codes given for one object" << std::endl ;
    return 1 ;
  }
  colorCodeSet = true ;
  std::istringstream stream ;
  stream.str( argv ) ;
  int nb ;
  stream >> nb ;
  if( !stream.fail() )
  {
    if( nb < ptr->GetFirstColor() || nb > ptr->GetLastColor() )
    {
       std::cerr << "Color Table: " << std::endl ;
       ptr->PrintColors() ;
       return 1 ;
    }
    ptr->SetColor( nb ) ;
  }
  else
  {
     ptr->SetColorString( argv ) ;
  }
  return 0 ;
}

int ReadColorFindValue( std::string argv , double &val )
{
   std::istringstream stream( argv ) ;
   stream >> val ;
   if( stream.fail() )
   {
      std::cerr << "Error: Problem reading color values" << std::endl ;
      return 1 ;
   }
   return 0 ;
}

int ReadColorArgument( bool &colorSet ,
                       std::string argv ,
                       ColorableClass* ptr
                     )
{
   if( colorSet )
   {
      std::cerr << "Error: Multiple colors given for one object" << std::endl ;
      return 1 ;
   }
   double RGB[ 3 ] ;
   std::vector< std::string > RGBstring( 3, "" ) ;
   std::size_t pos ;
   for( int i = 0 ; i < 2 ; i++ )
   {
      pos = argv.find( "," ) ;
      if( std::string::npos != pos )
      {
         RGBstring[ i ] = argv.substr( 0 , pos ) ;
         argv = argv.substr( pos + 1 , argv.size() - pos  - 1 ) ;
      }
   }
   pos = argv.find( "," ) ;
   if( pos != std::string::npos )
   {
     std::cerr << "Error: Their should be only 3 values given for the colors" << std::endl ;
   }
   RGBstring[ 2 ] = argv ;
   for( int i = 0 ; i < 3 ; i++ )
   {
     if( ReadColorFindValue( RGBstring[ i ] , RGB[ i ] ) )
     {
       return 1 ;
     }
   }
   if( ptr->SetRGB( RGB[ 0 ] , RGB[ 1 ] , RGB[ 2 ] ) )
   {
     std::cerr << "Error: Color values must be between 0.0 and 1.0" << std::endl ;
     return 1 ;
   }
   return 0 ;
}


int ReadOpacityArgument( bool &opacitySet , const char* str , ColorableClass *object )
{
   if( opacitySet )
   {
      std::cerr << "Multiple opacities given for one object" << std::endl ;
      return 1 ;
   }
   opacitySet = true ;
   std::istringstream stream ;
   stream.str( str ) ;
   double val ;
   stream >> val ;
   if( stream.fail() || object->SetOpacity( val ) )
   { 
      std::cerr << "Error: Opacity value must be between 0.0 and 1.0" << std::endl ;
      return 1 ;
   }
   object->SetOpacity( val ) ;
   return 0 ;
}


int ReadModelSubArguments( int argc ,
                           const char *argv[] ,
                           int &pos ,
                           std::vector< InputClass* > &arguments
                         )
{
   ModelClass* ptr = new ModelClass ;
   int exit = 0 ;
   bool colorCodeSet = false ;
   bool colorSet = false ;
   bool opacitySet = false ;
   while( pos < argc - 2 )
   {
      pos++ ;
      exit = ReadCommonSubArguments( argv[ pos ] , argv[ pos + 1 ] , ptr ) ;
      if( exit > 0 )
      {
         break ;
      }
      else if( exit < 0 )
      {
        pos++ ;
         continue ;
      }
      if( !strcmp( argv[ pos ] , "-cc" ) )
      {
         if( ReadColorCodeArgument( colorCodeSet , argv[ pos + 1 ] , ptr ) )
         {
            exit = 1 ;
            break ;
         }
         pos++ ;
      }
      else if( !strcmp( argv[ pos ] , "-op" ) )
      {
         if( ReadOpacityArgument( opacitySet , argv[ pos + 1 ] , ptr ) )
         {
            exit = 1 ;
            break ;
         }
         pos++ ;
      }
      else if( !strcmp( argv[ pos ] , "-dc" ) )
      {
         if( ReadColorArgument( colorSet , argv[ pos + 1 ] , ptr ) )
         {
            exit = 1 ;
            break ;
         }
         pos++ ;
      }
      else
      {
        std::cerr << "Error: No attributes found for one object" << std::endl ;
         exit = 1 ;
         break ;
      }
   }
   if( exit == 1 || !ptr->GetFileName().compare( "" ) )
   {
      delete ptr ;
      return 1 ;
   }
   if( exit == 2 )
   {
      pos-- ;
   }
   arguments.push_back( ptr ) ;
   return 0 ;
}

int ReadTransformSubArguments( int argc ,
                           const char *argv[] ,
                           int &pos ,
                           std::vector< InputClass* > &arguments
                         )
{
   TransformClass* ptr = new TransformClass ;
   int exit = 0 ;
   while( pos < argc - 2 )
   {
      pos++ ;
      exit = ReadCommonSubArguments( argv[ pos ] , argv[ pos + 1 ] , ptr ) ;
      if( exit > 0 )
      {
         break ;
      }
      else if( exit < 0 )
      {
        pos++ ;
         continue ;
      }
      else
      {
        std::cerr << "Error: No attributes found for one object" << std::endl ;
         exit = 1 ;
         break ;
      }
   }
   if( exit == 1 || !ptr->GetFileName().compare( "" ) )
   {
      delete ptr ;
      return 1 ;
   }
   if( exit == 2 )
   {
      pos-- ;
   }
   arguments.push_back( ptr ) ;
   return 0 ;
}


int ReadVolumeSubArguments( int argc ,
                            const char *argv[] ,
                            int &pos ,
                            std::vector< InputClass* > &arguments
                          )
{
   VolumeClass* ptr = new VolumeClass ;
   bool colorCodeSet = false ;
   bool colorSet = false ;
   bool typeSet = false ;
   bool labelSet = false ;
   bool opacitySet = false ;
   int exit = 0 ;
   while( pos < argc - 1 )
   {
      pos++ ;
      if( !strcmp( argv[ pos ] , "-l" ) )
      {
        if( labelSet )
        {
          std::cerr << "Error: label flag given mutliple times for one object" << std::endl ;
          exit = 1 ;
          break ;
        }
        labelSet = true ;
        ptr->LabelMap( true ) ;
        continue ;
      }
      if( pos >= argc - 1 )
      {
        break ;
      }
      exit = ReadCommonSubArguments( argv[ pos ] , argv[ pos + 1 ] , ptr ) ;
      if( exit > 0 )
      {
         break ;
      }
      else if( exit < 0 )
      {
        pos++ ;
         continue ;
      }
      if( !strcmp( argv[ pos ] , "-cc" ) )
      {
         if( ReadColorCodeArgument( colorCodeSet , argv[ pos + 1 ] , ptr ) )
         {
            exit = 1 ;
            break ;
         }
         pos++ ;
      }
      else if( !strcmp( argv[ pos ] , "-op" ) )
      {
         if( ReadOpacityArgument( opacitySet , argv[ pos + 1 ] , ptr ) )
         {
            exit = 1 ;
            break ;
         }
         pos++ ;
      }
      else if( !strcmp( argv[ pos ] , "-dc" ) )
      {
         if( ReadColorArgument( colorSet , argv[ pos + 1 ] , ptr ) )
         {
            exit = 1 ;
            break ;
         }
         pos++ ;
      }
      else if( !strcmp( argv[ pos ] , "-y" ) )
      {
         if( typeSet )
         {
           std::cerr << "Error: Multiple types given for one object" << std::endl ;
            exit = 1 ;
            break ;
         }
         exit = ptr->SetVolumeType( argv[ pos + 1 ] ) ;
         if( exit )
         {
            ptr->PrintVolumeTypes() ;
            break ;
         }
         typeSet = true ;
         pos++ ;
      }
      else
      {
        std::cerr << "Error: No attributes found for one object" << std::endl ;
         exit = 1 ;
         break ;
      }
   }
   if( exit == 1 || !ptr->GetFileName().compare( "" ) )
   {
      delete ptr ;
      return 1 ;
   }
   if( exit == 2 )
   {
      pos-- ;
   }
   arguments.push_back( ptr ) ;
   return 0 ;
}

int ReadArguments( int argc , const char *argv[] , std::vector< InputClass* > &arguments )
{
   if( argc < 2 )
   {
      PrintHelp( argv[ 0 ] , 0 ) ;
      return 1 ;
   }
   int i = 1 ;
   if( strcmp( argv[ i ] , "-h" ) )
   {
      i = 2 ;
   }
   if( !strcmp( argv[ 1 ] , "-v" )
    || !strcmp( argv[ 1 ] , "-t" )
    || !strcmp( argv[ 1 ] , "-m" )
     )
   {
      std::cerr << "First argument must be the scene file name [or -h]" << std::endl ;
      PrintHelp( argv[ 0 ] , 0 ) ;
      return 1 ; 
   }
   for( ; i < argc ; i++ )
   {
      if( !strcmp( argv[ i ] , "-v" ) )
      {
         if( ReadVolumeSubArguments( argc , argv ,i , arguments ) )
         {
            PrintHelp( argv[ 0 ] , 0 ) ;
            return 1 ;
         }
      }
      else if( !strcmp( argv[ i ] , "-t" ) )
      {
         if( ReadTransformSubArguments( argc , argv ,i , arguments ) )
         {
            PrintHelp( argv[ 0 ] , 0 ) ;
            return 1 ;
         }
      }
      else if( !strcmp( argv[ i ] , "-m" ) )
      {
         if( ReadModelSubArguments( argc , argv ,i , arguments ) )
         {
            PrintHelp( argv[ 0 ] , 0 ) ;
            return 1 ;
         }
      }
      else if( !strcmp( argv[ i ] , "-h" ) )
      {
         PrintHelp( argv[ 0 ] , 1 ) ;
         return 0 ;
      }
      else
      {
         PrintHelp( argv[ 0 ] , 0 ) ;
         return 1 ;
      }
   }
   return 0 ;
}


int SetParentNode( vtkMRMLTransformableNode *child , const char* parentName , vtkMRMLScene* scene )
{
  if( !strcmp( parentName , "" ) )
  {
    return 0 ;
  }
  vtkMRMLNode* node = NULL ;
  //     vtkMRMLNode* node = scene->GetNodeByID( input->GetParentName() ) ;
  for( int i = 0 ; i < scene->GetNumberOfNodes() ; i++ )
  {
    vtkMRMLNode* tempnode = scene->GetNthNode( i) ;
    if( !strcmp( tempnode->GetName() , parentName ) )
    {
      node = tempnode ;
    }
  }
  if( !node )
  {
    std::cerr << "Parent node does not exist. Set parent before child" << std::endl ;
    return 1 ;
  }
  child->SetAndObserveTransformNodeID( node->GetID() ) ;
  return 0 ;
}

int AddTransform( InputClass *input , vtkMRMLScene* scene )
{
   int output = 0 ;
   vtkMRMLTransformStorageNode* snode = vtkMRMLTransformStorageNode::New() ;
   snode->SetFileName( input->GetFileName().c_str() ) ;
   scene->AddNode( snode ) ;

   vtkMRMLLinearTransformNode* lnode = vtkMRMLLinearTransformNode::New() ;
   lnode->SetName( input->GetNodeName().c_str() ) ;
   lnode->SetAndObserveStorageNodeID( snode->GetID() ) ;
   if( SetParentNode( lnode ,  input->GetParentName().c_str() , scene ) )
   {
     output = 1 ;
   }
   if( !output )
   {
     scene->AddNode( lnode ) ;
   }
   snode->Delete() ;
   lnode->Delete() ;
   return  output ;
}


int AddColorable( ColorableClass* colorable ,
                  vtkMRMLDisplayNode* dnode ,
                  vtkMRMLStorageNode* snode ,
                  vtkMRMLDisplayableNode* inode ,
                  vtkMRMLScene* scene
                )
{
   int output = EXIT_SUCCESS ;
   snode->SetFileName( colorable->GetFileName().c_str() ) ;
   scene->AddNode( snode ) ;
   dnode->SetOpacity( colorable->GetOpacity() ) ;
   double colorRGB[ 3 ] ;
   colorRGB[ 0 ] = colorable->GetR() ;
   colorRGB[ 1 ] = colorable->GetG() ;
   colorRGB[ 2 ] = colorable->GetB() ;
   dnode->SetColor( colorRGB ) ;
   if( !strcmp( colorable->GetColorString() , "" ) )
   {
     dnode->SetAndObserveColorNodeID( colorable->GetColor() ) ;
   }
   else
   {
      vtkMRMLColorTableStorageNode *ctsnode = vtkMRMLColorTableStorageNode::New() ;
      ctsnode->SetFileName( colorable->GetColorString() ) ;
      vtkMRMLColorTableNode *ctnode = vtkMRMLColorTableNode::New() ;
      scene->AddNode( ctsnode ) ;
      scene->AddNode( ctnode ) ;
      ctnode->SetAndObserveStorageNodeID( ctsnode->GetID() ) ;
      dnode->SetAndObserveColorNodeID( ctnode->GetID() ) ;
      ctnode->Delete() ;
      ctsnode->Delete() ;
   }
   scene->AddNode( dnode ) ;
   inode->SetName( colorable->GetNodeName().c_str() ) ;
   inode->SetAndObserveDisplayNodeID( dnode->GetID() ) ;
   inode->SetAndObserveStorageNodeID( snode->GetID() ) ;
   if( SetParentNode( inode , colorable->GetParentName().c_str() , scene ) )
   {
      output = EXIT_FAILURE ;
   }
   if( !output )
   {
      scene->AddNode( inode ) ;
   }
   inode->Delete() ;
   dnode->Delete() ;
   snode->Delete() ;
   return output ;
}

int AddModel( ModelClass *model , vtkMRMLScene* scene )
{
   vtkMRMLModelStorageNode *snode = vtkMRMLModelStorageNode::New() ;
   vtkMRMLModelDisplayNode *dnode = vtkMRMLModelDisplayNode::New() ;
   vtkMRMLModelNode *inode = vtkMRMLModelNode::New() ;
   return AddColorable( model , dnode , snode , inode , scene ) ;
}

int AddVolume( VolumeClass *volume , vtkMRMLScene* scene )
{
   vtkMRMLStorageNode *snode ;
   vtkMRMLScalarVolumeDisplayNode* dnode ;
   vtkMRMLVolumeNode *inode ;
   if( !volume->GetVolumeType().compare( "scalar" ) )
   {
     snode = vtkMRMLVolumeArchetypeStorageNode::New() ;
     dnode = vtkMRMLScalarVolumeDisplayNode::New() ;
     vtkMRMLScalarVolumeNode *vnode = vtkMRMLScalarVolumeNode::New() ;
     if( volume->IsLabelMap() )
     {
       vnode->LabelMapOn() ;
     }
     inode = vnode ;
   }
   else if( !volume->GetVolumeType().compare( "DWI" ) )
   {
     snode = vtkMRMLNRRDStorageNode::New() ;
     dnode = vtkMRMLDiffusionWeightedVolumeDisplayNode::New() ;
     inode = vtkMRMLDiffusionWeightedVolumeNode::New() ;
   }
   else if( !volume->GetVolumeType().compare( "DTI" ) )
   {
     snode = vtkMRMLNRRDStorageNode::New() ;
     dnode = vtkMRMLDiffusionTensorVolumeDisplayNode::New() ;
     inode = vtkMRMLDiffusionTensorVolumeNode::New() ;
   }
   else if( !volume->GetVolumeType().compare( "vector" ) )
   {
     snode = vtkMRMLVolumeArchetypeStorageNode::New() ;
     dnode = vtkMRMLVectorVolumeDisplayNode::New() ;
     inode = vtkMRMLVectorVolumeNode::New() ;
   }
   else
   {
     std::cerr << "Error: Volume type unknown" << std::endl ;
     return EXIT_FAILURE ;
   }
   return AddColorable( volume , dnode , snode , inode , scene ) ;
}

void CheckNodeName( std::vector< InputClass* > &arguments )
{
   for( unsigned int i = 0 ; i < arguments.size() ; i++ )
   {
      if( !arguments[ i ]->GetNodeName().compare( "" ) )
      {
         std::string name = arguments[ i ]->GetFileName() ;
         //remove folder
         std::size_t pos = name.find_last_of( "/\\" ) ;
         if( pos != std::string::npos )
         {
            name = name.substr( pos + 1 ) ;
         }
         //remove extension
         pos = name.find_last_of( "." ) ;
         if( pos != std::string::npos )
         {
            name = name.substr( 0 , pos ) ;
         }
         arguments[ i ]->SetNodeName( name ) ;
      }
   }
}

//Check that each node name is unique
int CheckDoublons( std::vector< InputClass* > arguments )
{
   for( unsigned int i = 0 ; i < arguments.size() - 1 ; i++ )
   {
     for( unsigned int j = i + 1 ; j < arguments.size() ; j++ )
      {
        if( !arguments[ i ]->GetNodeName().compare( arguments[ j ]->GetNodeName() ) )
         {
            return 1 ;
         }
      }
   }
   return 0 ;
}

int main( int argc , const char* argv[] )
{
  int output = EXIT_SUCCESS ;
   std::vector< InputClass* > arguments ;
   if( ReadArguments( argc , argv , arguments ) )
   {
      return EXIT_FAILURE ;
   }
   std::string sceneName = argv[ 1 ] ;
   CheckNodeName( arguments ) ;
   if( CheckDoublons( arguments ) )
   {
      std::cerr << "Error: Some node names appear multiple times. Please verify your input information" << std::endl ;
      for( unsigned int i = 0 ; i < arguments.size() ; i++ )
      {
         std::cerr << arguments[ i ]->GetFileName() << " : " << arguments[ i ]->GetNodeName() << std::endl ;
      }
      return EXIT_FAILURE ;
   }
   //Create scene
   vtkMRMLScene* scene = vtkMRMLScene::New() ;
   for( unsigned int i = 0 ; i < arguments.size() ; i++ )
   {
      if( !arguments[ i ]->GetType().compare( "Transform" ) )
      {
        if( AddTransform( arguments[ i ] , scene ) )
        {
          output = EXIT_FAILURE ;
         break ;
        }
      }
      if( !arguments[ i ]->GetType().compare( "Model" ) )
      {
         ModelClass *model= dynamic_cast< ModelClass*>( arguments[ i ] ) ;
         if( !model )
         {
            std::cerr << "Problem dynamic casting ModelClass: This should never happen!!!!" << std::endl ;
            return EXIT_FAILURE ;
         }
         if( AddModel( model , scene ) )
         {
           output = EXIT_FAILURE ;
           break ;
        }
      }
      if( !arguments[ i ]->GetType().compare( "Volume" ) )
      {
         VolumeClass *volume = dynamic_cast< VolumeClass*>( arguments[ i ] ) ;
         if( !volume )
         {
            std::cerr << "Problem dynamic casting VolumeClass: This should never happen!!!!" << std::endl ;
            return EXIT_FAILURE ;
         }
         if( AddVolume( volume , scene ) )
         {
           output = EXIT_FAILURE ;
           break ;
        }
      }
   }
if( output == EXIT_SUCCESS )
{
  scene->SetURL( sceneName.c_str() ) ;
  scene->Commit() ;
  scene->Delete() ;
}
 for( unsigned int i = 0 ; i < arguments.size() ; i++ )
 {
    delete arguments[ i ] ;
 }
 return output ;
}
