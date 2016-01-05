# Module that exposes old elastix as an ITK filter
set( MODULE ModuleElastix )

# If OpenMP is supported, elastix will have beeen compiled with
# OpenMP flags, and we need to add them here as well
find_package( OpenMP )
if( OPENMP_FOUND )
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}" )
  set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}" )
endif()

if( NOT EXISTS ${ELASTIX_USE_FILE} )
  option( ELASTIX_USE_FILE "NOT_FOUND" )
  message( FATAL_ERROR "${MODULE} could not find ELASTIX_USE_FILE. Use the SuperBuild or manually point the ELASTIX_USE_FILE CMake variable to the UseElastix.cmake file in the root of your local elastix build tree." )
endif()

# Export include files
include( ${ELASTIX_USE_FILE} )

# Export libraries
set( ${MODULE}_LIBRARIES 
  elastix
  transformix
)

# Export tests
set( ${MODULE}_TESTS 
  elxElastixFilterTest.cxx
  elxTransformixFilterTest.cxx
)
