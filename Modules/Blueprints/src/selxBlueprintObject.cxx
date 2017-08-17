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

#include "selxBlueprintObject.h"
#include "selxBlueprint.h"

namespace selx
{

void
selxBlueprintObject
::SetBlueprint( Blueprint & blueprint )
{
  this->Modified();
  this->m_Blueprint = BlueprintPointer( &blueprint );
}


Blueprint &
selxBlueprintObject
::GetBlueprint( void)
{
  if( this->m_Blueprint )
  {
    return *this->m_Blueprint;
  }
  else
  {
    itkExceptionMacro( "Blueprint not set." )
  }
}


bool
selxBlueprintObject
::SetComponent( ComponentNameType name, ParameterMapType parameterMap )
{
  this->Modified();
  return this->m_Blueprint->SetComponent( name, parameterMap );
}


selxBlueprintObject::ParameterMapType
selxBlueprintObject
::GetComponent( ComponentNameType componentName ) const
{
  return this->m_Blueprint->GetComponent( componentName );
}


bool
selxBlueprintObject
::DeleteComponent( ComponentNameType componentName )
{
  this->Modified();
  return this->m_Blueprint->DeleteComponent( componentName );
}


selxBlueprintObject::ComponentNamesType
selxBlueprintObject::GetComponentNames( void ) const
{
  return this->m_Blueprint->GetComponentNames();
}


bool
selxBlueprintObject
::SetConnection( ComponentNameType upstream, ComponentNameType downstream, ParameterMapType parameterMap )
{
  this->Modified();
  return this->m_Blueprint->SetConnection( upstream, downstream, parameterMap );
}


selxBlueprintObject::ParameterMapType
selxBlueprintObject
::GetConnection( ComponentNameType upstream, ComponentNameType downstream ) const
{
  return this->m_Blueprint->GetConnection( upstream, downstream );
}


bool
selxBlueprintObject
::DeleteConnection( ComponentNameType upstream, ComponentNameType downstream )
{
  this->Modified();
  return this->m_Blueprint->DeleteConnection( upstream, downstream );
}


bool
selxBlueprintObject
::ComponentExists( ComponentNameType componentName ) const
{
  return this->m_Blueprint->ComponentExists( componentName );
}


bool
selxBlueprintObject
::ConnectionExists( ComponentNameType upstream, ComponentNameType downstream ) const
{
  return this->m_Blueprint->ConnectionExists( upstream, downstream );
}


bool
selxBlueprintObject
::ComposeWith( selxBlueprintObject& other)
{
  this->Modified();
  return this->m_Blueprint->ComposeWith( other.GetBlueprint() );
}


selxBlueprintObject::ComponentNamesType
selxBlueprintObject
::GetOutputNames( const ComponentNameType name ) const
{
  return this->m_Blueprint->GetOutputNames( name );
}


selxBlueprintObject::ComponentNamesType
selxBlueprintObject
::GetInputNames( const ComponentNameType name ) const
{
  return this->m_Blueprint->GetInputNames( name );
}


void
selxBlueprintObject
::Write( const std::string filename )
{
  this->m_Blueprint->Write( filename );
}
} // namespace selx
