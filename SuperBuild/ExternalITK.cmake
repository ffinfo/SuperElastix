#=========================================================================
#
#  Copyright Leiden University Medical Center, Erasmus University Medical 
#  Center and contributors
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#        http://www.apache.org/licenses/LICENSE-2.0.txt
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
#
#=========================================================================

set( proj ITK )
set( ITK_REPOSITORY https://github.com/InsightSoftwareConsortium/ITK.git )
set( ITK_TAG "v${ITK_VERSION_STRING}")

UPDATE_SELX_SUPERBUILD_COMMAND(${proj})

ExternalProject_Add( ${proj}
  GIT_REPOSITORY ${ITK_REPOSITORY}
  GIT_TAG ${ITK_TAG}
  UPDATE_COMMAND ""
  PATCH_COMMAND "${GIT_EXECUTABLE}" reset --hard && "${GIT_EXECUTABLE}" clean --force && "${GIT_EXECUTABLE}" apply --whitespace=fix "${CMAKE_SOURCE_DIR}/Patches/itk_png_mangle_get_uint_31.patch" && "${GIT_EXECUTABLE}" apply "${CMAKE_SOURCE_DIR}/Patches/itk_nifti_mangle.patch"
  SOURCE_DIR ${proj}
  BINARY_DIR ${proj}-build
  CMAKE_ARGS
    --no-warn-unused-cli
    -DBUILD_EXAMPLES:BOOL=OFF
    -DBUILD_TESTING:BOOL=OFF
    -DITK_BUILD_DEFAULT_MODULES:BOOL=ON
    -DCMAKE_CONFIGURATION_TYPES:STRING=${CMAKE_CONFIGURATION_TYPES}
    -DCMAKE_SKIP_RPATH:BOOL=ON
    -DITK_LEGACY_REMOVE:BOOL=ON
    -DBUILD_SHARED_LIBS:BOOL=${SUPERELASTIX_BUILD_SHARED_LIBS}
    -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
  BUILD_COMMAND ${SELX_SUPERBUILD_COMMAND}
)

ExternalProject_Get_Property( ${proj} install_dir )
set( ITK_DIR "${install_dir}/lib/cmake/ITK-${ITK_VERSION_MAJOR}.${ITK_VERSION_MINOR}" )

list( APPEND SUPERELASTIX_DEPENDENCIES ${proj} )
