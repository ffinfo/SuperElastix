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

#include "selxLoggerObject.h"
#include "selxLogger.h"

namespace selx
{

void
selxLoggerObject
::SetLogger( const Logger & logger )
{
  this->Modified();
  this->m_Logger = LoggerPointer( logger );
}


const Logger &
selxLoggerObject
::GetLogger( void)
{
  if( this->m_Logger )
  {
    return *this->m_Logger;
  }
  else
  {
    itkExceptionMacro( "Logger not set." )
  }
}

void
selxLoggerObject
::AddConsole( FormatType format )
{
  this->m_Logger->AddConsole( format );
}


// void
// Logger
// :: AddFile( FileNameType fileName, FormatType format )
// {
//   this->m_Pimple->AddFile( fileName, format );
// }

// void
// Logger
// :: AddFile( FileNameType fileName, Logger::ChannelType channel, FormatType format )
// {
//   this->m_Pimple->AddFile( fileName, channel, format );
// }

void
selxLoggerObject
::Log( SeverityType severity, MessageType message ) const
{
  this->m_Logger->Log( severity, message );
}


// void
// Logger
// ::Log( ChannelType channel, SeverityType severity, MessageType message )
// {
//   this->m_Pimple->Log( channel, severity, message );
// }
}
