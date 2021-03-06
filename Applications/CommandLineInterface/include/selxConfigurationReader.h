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

#ifndef ConfigurationReader_h
#define ConfigurationReader_h

#include "selxBlueprint.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>

#include <string>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <map>

namespace selx
{
class ConfigurationReader
{
public:

  typedef Blueprint::ParameterKeyType   ParameterKeyType;
  typedef Blueprint::ParameterValueType ParameterValueType;
  typedef Blueprint::ParameterMapType   ParameterMapType;
  typedef std::unique_ptr< Blueprint >  BlueprintPointerType;

  using PathType = boost::filesystem::path;
  using PathsType = std::list<PathType>;
  static BlueprintPointerType FromFile(const PathType & filename);
  static void MergeFromFile(BlueprintPointerType & blueprint, const PathType & filename);
  
private:

  typedef boost::property_tree::ptree         PropertyTreeType;
  typedef const boost::property_tree::ptree & ComponentOrConnectionTreeType;

  static PropertyTreeType ReadPropertyTree(const PathType & filename);

  static PathsType FindIncludes(const PropertyTreeType &);
  static ParameterValueType VectorizeValues( ComponentOrConnectionTreeType componentOrConnectionTree );

  static BlueprintPointerType FromPropertyTree( const PropertyTreeType & );
  static void MergeProperties(BlueprintPointerType&, const PropertyTreeType &);


  
};
}

#endif // #define ConfigurationReader_h
