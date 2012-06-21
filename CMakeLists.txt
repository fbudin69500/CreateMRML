cmake_minimum_required(VERSION 2.6)
project (CreateMRML)

set(CreateMRML_SOURCE
 CreateMRML.cxx
 MRMLColorableHelper.h
 MRMLColorableHelper.cxx
 MRMLFiducialHelper.cxx
 MRMLFiducialHelper.h
 MRMLNodeHelper.h
 MRMLModelHelper.h
 MRMLTransformHelper.h
 MRMLVolumeHelper.h
 CreateMRMLSceneHelper.h
 CreateMRMLSceneHelper.cxx
 MagicParser.h
 MagicParser.cxx
)

find_package(Slicer3 REQUIRED PATHS ${Slicer3_DIR})
include(${Slicer3_USE_FILE})

find_package(ITK REQUIRED)
if(ITK_FOUND)
  include(${ITK_USE_FILE})
endif()

find_package(VTK REQUIRED)
if(VTK_FOUND)
  include(${VTK_USE_FILE})
endif()

set( t_generateclp_DIR ${GenerateCLP_DIR} )
set( GenerateCLP_DIR ${t_generateclp_DIR} CACHE PATH "" FORCE )

set( t_tclap_DIR ${TCLAP_DIR} )
set( TCLAP_DIR ${t_tclap_DIR} CACHE PATH "" FORCE )


set( t_module_DIR ${ModuleDescriptionParser_DIR} )
set( ModuleDescriptionParser_DIR ${t_module_DIR} CACHE PATH "" FORCE )


add_executable(CreateMRML ${CreateMRML_SOURCE})
target_link_libraries(CreateMRML ${Slicer3_Libs_LIBRARIES} ${ITK_LIBRARIES} vtkCommon)


