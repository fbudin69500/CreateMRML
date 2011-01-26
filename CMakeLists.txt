cmake_minimum_required(VERSION 2.6)
project (CreateMRML)

set(CreateMRML_SOURCE
 CreateMRML.cxx
 ColorableClass.h
 ColorableClass.cxx
 FiducialClass.cxx
 FiducialClass.h
 InputClass.h
 ModelClass.h
 TransformClass.h
 VolumeClass.h
)

  find_package(Slicer3 REQUIRED)
  include(${Slicer3_USE_FILE})

add_executable(CreateMRML ${CreateMRML_SOURCE})
target_link_libraries(CreateMRML ${Slicer3_Libs_LIBRARIES} )


