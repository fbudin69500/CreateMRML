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

find_package(Slicer3 REQUIRED)

include_directories(BEFORE 
  ${Slicer3_Libs_INCLUDE_DIRS} 
  ${Slicer3_Base_INCLUDE_DIRS}
  )

# Add link directories needed to use Slicer3.
link_directories(
  ${Slicer3_Libs_LIBRARY_DIRS}
  ${Slicer3_Base_LIBRARY_DIRS}
  )

find_package(ITK REQUIRED)
if(ITK_FOUND)
  include(${ITK_USE_FILE})
endif()

find_package(VTK REQUIRED)
if(VTK_FOUND)
  include(${VTK_USE_FILE})
endif()

link_directories( ${Slicer3_HOME}/lib/MRMLCLI )
mark_as_advanced( Slicer3_HOME )

add_executable(CreateMRML ${CreateMRML_SOURCE})
target_link_libraries(CreateMRML ${Slicer3_Libs_LIBRARIES} ${ITK_LIBRARIES} vtkCommon)


