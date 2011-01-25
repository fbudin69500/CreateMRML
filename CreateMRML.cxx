#include <vtkMRMLScene.h>
#include <vtkMRMLLinearTransformNode.h>
#include <vtkMRMLTransformStorageNode.h>
#include <vtkMRMLScalarVolumeNode.h>
#include <vtkMRMLVolumeArchetypeStorageNode.h>
#include <vtkMRMLScalarVolumeDisplayNode.h>
//#include <vtkMRMLVectorVolumeNode.h>
//#include <vtkMRMLDiffusionTensorVolumeNode.h>
//#include <vtkMRMLDiffusionWeightedVolumeNode.h>

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
-c color
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
   std::cout << arg0 << " SceneFileName [-t/-v/-m][-f/-p/-n][-c/-y][-h]" << std::endl ;
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
   std::cout << "If node is a volume:" << std::endl ;
   std::cout << std::endl ;
   std::cout << "-c ColorCode (default: 1 [grey])" << std::endl ;
   std::cout << "-y VolumeType (default: scalar)" << std::endl ;
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


int ReadModelSubArguments( int argc ,
                           const char *argv[] ,
                           int &pos ,
                           std::vector< InputClass* > &arguments
                         )
{
   ModelClass* ptr = new ModelClass ;
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
   bool color = false ;
   bool type = false ;
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
      if( !strcmp( argv[ pos ] , "-c" ) )
      {
         if( color )
         {
           std::cerr << "Error: Multiple colors given for one object" << std::endl ;
            exit = 1 ;
            break ;
         }
         std::istringstream stream ;
         stream.str( argv[ pos + 1 ] ) ;
         int nb ;
         try
         {
           stream >> nb ;
           if( nb < ptr->GetFirstColor() || nb > ptr->GetLastColor() )
           {
              throw "error: index ouside of range" ;
           }
         }
         catch(...)
         {
            std::cerr << "Color Table: " << std::endl ;
            ptr->PrintColors() ;
            exit = 1 ;
            break ;
         }
         ptr->SetColor( nb ) ;
         color = true ;
         pos++ ;
      }
      else if( !strcmp( argv[ pos ] , "-y" ) )
      {
         if( type )
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
         type = true ;
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
   if( !strcmp( argv[ 1 ] , "-v" ) || !strcmp( argv[ 1 ] , "-t" ) || !strcmp( argv[ 1 ] , "-m" ) )
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
   std::string storageName = input->GetNodeName() + "Storage" ;
   snode->SetName( storageName.c_str() ) ;
   scene->AddNodeNoNotify( snode ) ;

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


int AddModel( InputClass *input , vtkMRMLScene* scene )
{
   return 0 ;
}

int AddVolume( VolumeClass *volume , vtkMRMLScene* scene )
{
   int output = 0 ;
   vtkMRMLVolumeArchetypeStorageNode* snode = vtkMRMLVolumeArchetypeStorageNode::New() ;
   snode->SetFileName( volume->GetFileName().c_str() ) ;
   std::string storageName = volume->GetNodeName() + "Storage" ;
   snode->SetName( storageName.c_str() ) ;
   scene->AddNode( snode ) ;
   if( !volume->GetVolumeType().compare( "scalar" ) )
   {
      vtkMRMLScalarVolumeDisplayNode *dnode = vtkMRMLScalarVolumeDisplayNode::New() ;
      dnode->SetAndObserveColorNodeID( volume->GetColor() ) ;
      scene->AddNode( dnode ) ;
      vtkMRMLScalarVolumeNode* inode = vtkMRMLScalarVolumeNode::New() ;
      inode->SetName( volume->GetNodeName().c_str() ) ;
      inode->SetAndObserveStorageNodeID( snode->GetID() ) ;
      inode->SetAndObserveDisplayNodeID( dnode->GetID() ) ;
      if( SetParentNode( inode ,  volume->GetParentName().c_str() , scene ) )
      {
        output = 1 ;
      }
      if( !output )
      {
        scene->AddNode( inode ) ;
      }
      inode->Delete() ;
      dnode->Delete() ;
   }
   
   snode->Delete() ;

   return output ;
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
         AddTransform( arguments[ i ] , scene ) ;
      }
      if( !arguments[ i ]->GetType().compare( "Model" ) )
      {
         AddModel( arguments[ i ] , scene ) ;
      }
      if( !arguments[ i ]->GetType().compare( "Volume" ) )
      {
         VolumeClass *volume = dynamic_cast< VolumeClass*>( arguments[ i ] ) ;
         if( !volume )
         {
            std::cerr << "Problem dynamic casting VolumeClass: This should never happen!!!!" << std::endl ;
            return EXIT_FAILURE ;
         }
         AddVolume( volume , scene ) ;
      }
   }

scene->SetURL( sceneName.c_str() ) ;
scene->Commit() ;
scene->Delete() ;
 for( unsigned int i = 0 ; i < arguments.size() ; i++ )
 {
    delete arguments[ i ] ;
 }
 return EXIT_SUCCESS ;
}
