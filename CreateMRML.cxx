//#include "vtkSmartPointer.h"

#include <vtkMRMLScene.h>
#include <vtkMRMLLinearTransformNode.h>
#include <vtkMRMLTransformStorageNode.h>
//#include <vtkMRMLNRRDStorageNode.h>
#include <vtkMRMLScalarVolumeNode.h>
#include <vtkMRMLVolumeArchetypeStorageNode.h>
#include <vtkMRMLScalarVolumeDisplayNode.h>
#include <vtkMRMLColorTableNode.h>
//#include <vtkMRMLStorageNode.h>
//#include <vtkMRMLVectorVolumeNode.h>
//#include <vtkMRMLTensorVolumeNode.h>
//#include <vtkMRMLDiffusionTensorVolumeNode.h>
//#include <vtkMRMLDiffusionWeightedVolumeNode.h>


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
If -h (only, otherwise print error), print help
//visualization
****
*/

int main( int argc , char* argv[] )
{

//  vtkSmartPointer< vtkMRMLScene > scene1 = vtkSmartPointer< vtkMRMLScene >::New();
vtkMRMLScene* scene = vtkMRMLScene::New() ;

vtkMRMLTransformStorageNode* snode = vtkMRMLTransformStorageNode::New() ;
snode->SetFileName( "LinearTransform.tfm" ) ;
snode->SetName( "transformStorage" ) ;
scene->AddNodeNoNotify( snode ) ;

vtkMRMLLinearTransformNode* lnode = vtkMRMLLinearTransformNode::New() ;
lnode->SetName( "transform" ) ;
lnode->SetAndObserveStorageNodeID( snode->GetID() ) ;
scene->AddNode( lnode ) ;

//vtkMRMLStorageNode* s2node = vtkMRMLStorageNode::New() ;
vtkMRMLVolumeArchetypeStorageNode* s2node = vtkMRMLVolumeArchetypeStorageNode::New() ;
s2node->SetFileName( "idwi.nrrd" ) ;
s2node->SetName( "imageStorage" ) ;
scene->AddNode( s2node ) ;


vtkMRMLColorTableNode *colorNode = vtkMRMLColorTableNode::New() ;
colorNode->SetTypeToGrey() ;
vtkMRMLScalarVolumeDisplayNode *dnode = vtkMRMLScalarVolumeDisplayNode::New() ;
dnode->SetAndObserveColorNodeID( colorNode->GetTypeAsIDString() ) ;
colorNode->Delete() ;
scene->AddNode( dnode ) ;

vtkMRMLScalarVolumeNode* inode = vtkMRMLScalarVolumeNode::New() ;
inode->SetName( "scalarImage" ) ;
inode->SetAndObserveStorageNodeID( s2node->GetID() ) ;
inode->SetAndObserveDisplayNodeID( dnode->GetID() ) ;
inode->SetAndObserveTransformNodeID( lnode->GetID() ) ;
scene->AddNode( inode ) ;


inode->Delete() ;
s2node->Delete() ;
dnode->Delete() ;
snode->Delete() ;
lnode->Delete() ;




scene->SetURL( "toto.mrml" ) ;
scene->Commit() ;
scene->Delete() ;
 return EXIT_SUCCESS ;
}
