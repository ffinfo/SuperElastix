#
# This CMakeLists file will generate the file selxDefaultComponents.h
# in the bin directory. This file defines the components to be compiled.
#

# User specified variables:
mark_as_advanced( SUPERELASTIX_IMAGE_DIMENSIONS )
set( ELASTIX_IMAGE_DIMENSIONS 2 3 4 CACHE STRING "Specify image dimensions" )

mark_as_advanced( SUPERELASTIX_IMAGE_2D_PIXELTYPES )
set( ELASTIX_IMAGE_2D_PIXELTYPES "float" CACHE STRING "Specify 2D pixel types" )

mark_as_advanced( SUPERELASTIX_IMAGE_3D_PIXELTYPES )
set( ELASTIX_IMAGE_3D_PIXELTYPES "short" "float" CACHE STRING "Specify 3D pixel types" )

mark_as_advanced( SUPERELASTIX_IMAGE_4D_PIXELTYPES )
set( ELASTIX_IMAGE_4D_PIXELTYPES "short" CACHE STRING "Specify 4D pixel types" )

# Define supported dimensions and types for sanity checks.
# Gives protection against typo's.
set( SUPPORTED_DIMENSIONS 2 3 4 )
set( SUPPORTED_TYPES "char" "unsigned char"
  "short" "unsigned short" "int" "unsigned int"
  "long" "unsigned long" "float" "double" )

# Sanity check if > 0 number of dimensions are requested
list( LENGTH SUPERELASTIX_IMAGE_DIMENSIONS ndims )
if( ${ndims} EQUAL 0 )
  message( FATAL_ERROR "ERROR: you should set one or more values "
    " to SUPERELASTIX_IMAGE_DIMENSIONS!\n"
    "Choose a subset of {${SUPPORTED_DIMENSIONS}}." )
endif()

# Start a string containing the supported image types
# and initialize some variables.
set( supportString "" )
set( supportOpenCLImages "" )
set( index 1 )
set( numSupported 0 )

# Add supported image types to the string
foreach( dim ${ELASTIX_IMAGE_DIMENSIONS} )

  # Check dimension
  if( ${dim} EQUAL 2 )
    set( PIXEL_TYPE_LIST ${ELASTIX_IMAGE_2D_PIXELTYPES} )
    set( whichList "ELASTIX_IMAGE_2D_PIXELTYPES" )
  elseif( ${dim} EQUAL 3 )
    set( PIXEL_TYPE_LIST ${ELASTIX_IMAGE_3D_PIXELTYPES} )
    set( whichList "ELASTIX_IMAGE_3D_PIXELTYPES" )
  elseif( ${dim} EQUAL 4 )
    set( PIXEL_TYPE_LIST ${ELASTIX_IMAGE_4D_PIXELTYPES} )
    set( whichList "ELASTIX_IMAGE_4D_PIXELTYPES" )
  else()
    message( FATAL_ERROR "ERROR: you selected SUPERELASTIX_IMAGE_DIMENSIONS"
      " to include ${dim}, which is not supported!\n"
      "Choose a subset of {${SUPPORTED_DIMENSIONS}}." )
  endif()

  # Sanity check if > 0 number of pixel types are requested
  list( LENGTH PIXEL_TYPE_LIST PIXEL_TYPE_LIST_LENGTH )
  if( ${PIXEL_TYPE_LIST_LENGTH} EQUAL 0 )
    message( FATAL_ERROR "ERROR: you should set one or more values"
      " to ${whichList}!\n"
      "Choose a subset of {${SUPPORTED_TYPES}}." )
  endif()

  # Add types
  foreach( type ${PIXEL_TYPE_LIST} )
    # Sanity check
    list( FIND SUPPORTED_TYPES ${type} foundIndex )
    if( ${foundIndex} EQUAL -1 )
      message( FATAL_ERROR "ERROR: you selected ${whichList}"
        " to include ${type}, which is not supported!\n"
        "Choose a subset of {${SUPPORTED_TYPES}}." )
    endif()

    set( MODULE_COMPONENT_INCLUDE_FILES
      "{MODULE_COMPONENT_INCLUDE_FILES}" ${${MODULE}_COMPONENT_INCLUDE_FILES}\n" # <-- TODO: Generate automatically or by user?
    )
    
    # TODO: Should this be filled with dims types here or in module's cmake?
    set( MODULE_COMPONENT_TYPELISTS 
      "${MODULE_COMPONENT_TYPELISTS}" ${${MODULE}_COMPONENT_TYPELIST}\n" <-- TODO: Generate automatically or by user?
    )
  endforeach()
endforeach()

configure_file(
  ${${MODULE}_SOURCE_DIR}/include/selxDefaultComponents.h.in
  ${${MODULE}_BINARY_DIR}/include/selxDefaultComponents.h
  @ONLY
)
