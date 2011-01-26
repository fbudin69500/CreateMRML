#ifndef _CreateMRMLScene_h
#define _CreateMRMLScene_h

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
#include <vtkMRMLFiducialListStorageNode.h>
#include <vtkMRMLFiducialListNode.h>

#include "CreateMRMLScene.h"
#include "ModelClass.h"
#include "TransformClass.h"
#include "VolumeClass.h"
#include "FiducialClass.h"
#include <vector>
#include <string>

class CreateMRMLScene
{
   public:
      CreateMRMLScene() ;
      ~CreateMRMLScene() ;
      void SetSceneName( std::string name ) ;
      int Write() ;
      void SetInputs( std::vector< InputClass* > arg ) ;
//      void AddInput( InputClass* arg ) ;
   private:
      std::string m_SceneName ;
      std::vector< InputClass* > m_Arguments ;
      vtkMRMLScene* m_Scene ;
      int CheckDoublons( ) ;
      int AddVolume( VolumeClass *volume ) ;
      int SetParentNode( vtkMRMLTransformableNode *child , const char* parentName ) ;
      int AddTransform( InputClass *input ) ;
      int AddColorable( ColorableClass* colorable ,
                        vtkMRMLDisplayNode* dnode ,
                        vtkMRMLStorageNode* snode ,
                        vtkMRMLDisplayableNode* inode
                      ) ;
      int AddModel( ModelClass *model ) ;
      int AddFiducial( FiducialClass *fiducial ) ;
};

#endif
