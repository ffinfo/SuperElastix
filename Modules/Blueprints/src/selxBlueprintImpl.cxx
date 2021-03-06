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

#include "selxBlueprintImpl.h"

#include <stdexcept>

namespace selx
{
// Declared outside of the class body, so it is a free function
std::ostream &
operator<<( std::ostream & out, const Blueprint::ParameterMapType & val )
{
  for( auto const & mapPair : val )
  {
    out << mapPair.first << " : [ ";
    for( auto const & value : mapPair.second )
    {
      out << value << " ";
    }
    out << "]\\n";
  }
  return out;
}


template< class NameType, class ParameterMapType >
class vertex_label_writer
{
public:

  vertex_label_writer( NameType _name, ParameterMapType _parameterMap ) : name( _name ), parameterMap( _parameterMap ) {}
  template< class VertexOrEdge >
  void operator()( std::ostream & out, const VertexOrEdge & v ) const
  {
    out << "[label=\"" << name[ v ] << "\n" << parameterMap[ v ] << "\"]";
  }


private:

  NameType         name;
  ParameterMapType parameterMap;
};

template< class NameType, class ParameterMapType >
inline vertex_label_writer< NameType, ParameterMapType >
make_vertex_label_writer( NameType n, ParameterMapType p )
{
  return vertex_label_writer< NameType, ParameterMapType >( n, p );
}


template< class ParameterMapType >
class edge_label_writer
{
public:

  edge_label_writer( ParameterMapType _parameterMap ) : parameterMap( _parameterMap ) {}
  template< class VertexOrEdge >
  void operator()( std::ostream & out, const VertexOrEdge & v ) const
  {
    out << "[label=\"" << parameterMap[ v ] << "\"]";
  }


private:

  ParameterMapType parameterMap;
};

template< class ParameterMapType >
inline edge_label_writer< ParameterMapType >
make_edge_label_writer( ParameterMapType p )
{
  return edge_label_writer< ParameterMapType >( p );
}


bool
Blueprint::BlueprintImpl
::SetComponent( ComponentNameType name, ParameterMapType parameterMap )
{
  if( this->ComponentExists( name ) )
  {
    this->m_Graph[ name ].parameterMap = parameterMap;
    return true;
  }
  else
  {
    return this->m_Graph.insert_vertex( name, { name, parameterMap } ).second;
  }
}


Blueprint::ParameterMapType
Blueprint::BlueprintImpl
::GetComponent( ComponentNameType name ) const
{
  if( !this->ComponentExists( name ) )
  {
    std::stringstream msg;
    msg << "Blueprint does not contain component " << name << std::endl;
    throw std::runtime_error( msg.str() );
  }

  return this->m_Graph[ name ].parameterMap;
}


bool
Blueprint::BlueprintImpl
::DeleteComponent( ComponentNameType name )
{
  if( !this->ComponentExists( name ) )
  {
    this->m_Graph.remove_vertex( name );
    return true;
  }

  return false;
}


Blueprint::ComponentNamesType
Blueprint::BlueprintImpl
::GetComponentNames( void ) const
{
  ComponentNamesType container;
  for( auto it = boost::vertices( this->m_Graph.graph() ).first; it != boost::vertices( this->m_Graph.graph() ).second; ++it )
  {
    container.push_back( this->m_Graph.graph()[ *it ].name );
  }
  return container;
}


bool
Blueprint::BlueprintImpl
::SetConnection( ComponentNameType upstream, ComponentNameType downstream, ParameterMapType parameterMap )
{
  if( !this->ComponentExists( upstream ) || !this->ComponentExists( downstream ) )
  {
    return false;
  }

  if( !this->ConnectionExists( upstream, downstream ) )
  {
    boost::add_edge_by_label( upstream, downstream,  { parameterMap }, this->m_Graph );
  }
  else
  {
    this->m_Graph[ this->GetConnectionIndex( upstream, downstream ) ].parameterMap = parameterMap;
  }

  return true;
}


Blueprint::ParameterMapType
Blueprint::BlueprintImpl
::GetConnection( ComponentNameType upstream, ComponentNameType downstream ) const
{
  return this->m_Graph[ this->GetConnectionIndex( upstream, downstream ) ].parameterMap;
}


bool
Blueprint::BlueprintImpl
::DeleteConnection( Blueprint::ComponentNameType upstream, Blueprint::ComponentNameType downstream )
{
  if( this->ConnectionExists( upstream, downstream ) )
  {
    boost::remove_edge_by_label( upstream, downstream, this->m_Graph );
  }

  return !this->ConnectionExists( upstream, downstream );
}


bool
Blueprint::BlueprintImpl
::ComponentExists( ComponentNameType componentName ) const
{
  const GraphType::vertex_descriptor descriptor = this->m_Graph.vertex( componentName );
  return descriptor != boost::graph_traits< GraphType >::null_vertex();
}


bool
Blueprint::BlueprintImpl
::ConnectionExists( ComponentNameType upstream, ComponentNameType downstream ) const
{
  if( !this->ComponentExists( upstream ) )
  {
    return false;
  }
  if( !this->ComponentExists( downstream ) )
  {
    return false;
  }

  return boost::edge_by_label( upstream, downstream, this->m_Graph ).second;
}


bool
Blueprint::BlueprintImpl
::ComposeWith( std::unique_ptr< Blueprint > const & other )
{
  // Make a backup of the current blueprint status in case composition fails
  GraphType graph_backup = GraphType( this->m_Graph );

  // Copy-in all components (Nodes)
  for( auto const & componentName : other->GetComponentNames() )
  {
    // Does other blueprint use component with a name that already exists?
    if( this->ComponentExists( componentName ) )
    {
      // Component exists, check if properties can be merged
      auto ownProperties    = this->GetComponent( componentName );
      auto othersProperties = other->GetComponent( componentName );

      for( auto const & othersEntry : othersProperties )
      {
        // Does other use a property key that already exists in this component?
        if( ownProperties.count( othersEntry.first ) )
        {
          auto && ownValues   = ownProperties[ othersEntry.first ];
          auto && otherValues = othersEntry.second;
          // Are the property values equal?
          if( ownValues.size() != otherValues.size() )
          {
            // No, based on the number of values we see that it is different. Blueprints cannot be Composed
            this->m_Graph = graph_backup;
            return false;
          }
          else
          {
            ParameterValueType::const_iterator ownValue;
            ParameterValueType::const_iterator otherValue;
            for( ownValue = ownValues.begin(), otherValue = otherValues.begin(); ownValue != ownValues.end(); ++ownValue, ++otherValue )
            {
              if( *otherValue != *ownValue )
              {
                // No, at least one value is different. Blueprints cannot be Composed
                this->m_Graph = graph_backup;
                return false;
              }
            }
          }
        }
        else
        {
          // Property key doesn't exist yet, add entry to this component
          auto ownProperties = this->GetComponent( componentName );
          ownProperties[ othersEntry.first ] = othersEntry.second;
          this->SetComponent( componentName, ownProperties );
        }
      }
    }
    else
    {
      // Create Component copying properties of other
      this->SetComponent( componentName, other->GetComponent( componentName ) );
    }
  }
  // Copy-in all connections (Edges)
  for( auto const & componentName : other->GetComponentNames() )
  {
    for( auto incomingName : other->GetInputNames( componentName ) )
    {
      // Does other blueprint have a connection that already exists?
      if( this->ConnectionExists( incomingName, componentName ) )
      {
        // Connection exists, check if properties can be merged
        auto ownProperties    = this->GetConnection( incomingName, componentName );
        auto othersProperties = other->GetConnection( incomingName, componentName );

        for( auto const & othersEntry : othersProperties )
        {
          // Does other use a property key that already exists in this component?
          if( ownProperties.count( othersEntry.first ) )
          {
            auto && ownValues   = ownProperties[ othersEntry.first ];
            auto && otherValues = othersEntry.second;
            // Are the property values equal?
            if( ownValues.size() != otherValues.size() )
            {
              // No, based on the number of values we see that it is different. Blueprints cannot be Composed
              this->m_Graph = graph_backup;
              return false;
            }
            else
            {
              ParameterValueType::const_iterator ownValue;
              ParameterValueType::const_iterator otherValue;
              for( ownValue = ownValues.begin(), otherValue = otherValues.begin(); ownValue != ownValues.end(); ++ownValue, ++otherValue )
              {
                if( *otherValue != *ownValue )
                {
                  // No, at least one value is different. Blueprints cannot be Composed
                  this->m_Graph = graph_backup;
                  return false;
                }
              }
            }
          }
          else
          {
            // Property key doesn't exist yet, add entry to this component
            auto ownProperties = this->GetConnection( incomingName, componentName );
            ownProperties[ othersEntry.first ] = othersEntry.second;
            this->SetConnection( incomingName, componentName, ownProperties );
          }
        }
      }
      else
      {
        // Create Component copying properties of other
        this->SetConnection( incomingName, componentName, other->GetConnection( incomingName, componentName ) );
      }
    }
  }

  return true;
}


Blueprint::ComponentNamesType
Blueprint::BlueprintImpl
::GetInputNames( const ComponentNameType name ) const
{
  ComponentNamesType container;
  //auto vertex = this->m_Graph.vertex(name);
  //boost::in_edges(vertex, this->m_Graph);
  InputIteratorPairType inputIteratorPair = boost::in_edges( this->m_Graph.vertex( name ), this->m_Graph );
  for( auto it = inputIteratorPair.first; it != inputIteratorPair.second; ++it )
  {
    container.push_back( this->m_Graph.graph()[ it->m_source ].name );
  }

  return container;
}


Blueprint::ComponentNamesType
Blueprint::BlueprintImpl
::GetOutputNames( const ComponentNameType name ) const
{
  ComponentNamesType     container;
  OutputIteratorPairType outputIteratorPair = boost::out_edges( this->m_Graph.vertex( name ), this->m_Graph );
  for( auto it = outputIteratorPair.first; it != outputIteratorPair.second; ++it )
  {
    container.push_back( this->m_Graph.graph()[ it->m_target ].name );
  }

  return container;
}


Blueprint::BlueprintImpl::ConnectionIndexType
Blueprint::BlueprintImpl
::GetConnectionIndex( ComponentNameType upstream, ComponentNameType downstream ) const
{
  // This function is part of the internal API and should fail hard if we use it incorrectly
  if( !this->ConnectionExists( upstream, downstream ) )
  {
    throw std::runtime_error( "Blueprint does not contain connection from component " + upstream + " to " + downstream );
  }

  return boost::edge_by_label( upstream, downstream, this->m_Graph ).first;
}


void
Blueprint::BlueprintImpl
::Write( const std::string filename )
{
  std::ofstream dotfile( filename.c_str() );
  boost::write_graphviz( dotfile, this->m_Graph,
    make_vertex_label_writer( boost::get( &ComponentPropertyType::name, this->m_Graph ),
    boost::get( &ComponentPropertyType::parameterMap, this->m_Graph ) ),
    make_edge_label_writer( boost::get( &ConnectionPropertyType::parameterMap, this->m_Graph ) ) );
}
} // namespace selx
