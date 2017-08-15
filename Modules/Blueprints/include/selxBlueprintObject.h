/*=========================================================================
 *
 *  Copyright Leiden University Medical Center, Erasmus University Medical
 *  Center and contributors
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

#ifndef selxBlueprintObject_h
#define selxBlueprintObject_h

#include "itkDataObject.h"

#include <string>
#include <vector>
#include <map>
#include <memory>

namespace selx
{
class selxBlueprintObject : public itk::DataObject
{
public:

  /** Standard ITK typedefs. */
  typedef selxBlueprintObject             Self;
  typedef itk::ProcessObject              Superclass;
  typedef itk::SmartPointer< Self >       Pointer;
  typedef itk::SmartPointer< const Self > ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro( Self );

  /** Run-time type information (and related methods). */
  itkTypeMacro( Self, itk::ProcessObject );

  typedef std::string                                      ParameterKeyType;
  typedef std::vector< std::string >                       ParameterValueType;
  typedef std::map< ParameterKeyType, ParameterValueType > ParameterMapType;
  typedef std::string                                      ComponentNameType;
  typedef std::vector< ComponentNameType >                 ComponentNamesType;

  /** The actual blueprint is a pimpled member variable */
  class Blueprint;
  typedef std::unique_ptr< Blueprint > BlueprintPointer;

  void SetBlueprint( Blueprint, BlueprintPointer );
  const Blueprint & GetBlueprint( void );

  bool SetComponent( ComponentNameType, ParameterMapType parameterMap );

  ParameterMapType GetComponent( ComponentNameType componentName ) const;

  bool DeleteComponent( ComponentNameType componentName );

  bool ComponentExists( ComponentNameType componentName ) const;

  // Returns a vector of the all Component names in the graph.
  ComponentNamesType GetComponentNames( void ) const;

  bool SetConnection( ComponentNameType upstream, ComponentNameType downstream, ParameterMapType parameterMap );

  ParameterMapType GetConnection( ComponentNameType upstream, ComponentNameType downstream ) const;

  bool DeleteConnection( ComponentNameType upstream, ComponentNameType downstream );

  bool ConnectionExists( ComponentNameType upstream, ComponentNameType downstream ) const;

  //std::unique_ptr<Blueprint> Clone(Blueprint const &other );

  // "functional" composition of blueprints is done by adding settings of other to this blueprint. Redefining/overwriting properties is not allowed and returns false.
  bool ComposeWith(const selxBlueprintObject &other);

  // Returns a vector of the Component names at the incoming direction
  ComponentNamesType GetInputNames( const ComponentNameType name ) const;

  // Returns a vector of the Component names at the outgoing direction
  ComponentNamesType GetOutputNames( const ComponentNameType name ) const;

  void Write( const std::string filename );

private:

  BlueprintPointer m_Blueprint;
};
}

#endif // #define selxBlueprintObject_h
