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

#ifndef Logger_h
#define Logger_h

#include <string>
#include <memory>

namespace selx
{
enum class SeverityType {
  SELX_TRACE,
  SELX_DEBUG,
  SELX_INFO,
  SELX_WARNING,
  SELX_ERROR,
  SELX_FATAL
};

class LoggerInterface
{
public:

  typedef const std::string MessageType;

  virtual void Log( SeverityType severity, MessageType message ) const = 0;
};

class Logger : public LoggerInterface
{
public:

  typedef const std::string ChannelType;
  typedef const std::string FormatType;
  using LoggerInterface::MessageType;
  Logger();
  ~Logger();

  void AddConsole( FormatType format = "[%LineID% %TimeStamp%]: %Message%" );

  // void AddFile( FileNameType fileName = "SuperElastix_%Y-%m-%d_%H-%M-%S.%N.log",
  //               FormatType format = "[%LineID% %TimeStamp% %ComponentName% %Channel% %SeverityLevel%]: %Message%" );
  // void AddFile( FileNameType fileName = "SuperElastix_%Y-%m-%d_%H-%M-%S.%N.log",
  //               Logger::ChannelType channel = "SuperElastix",
  //               FormatType format = "[%LineID% %TimeStamp% %ComponentName% %Channel% %SeverityLevel%]: %Message%" );

  virtual void Log( SeverityType severity, MessageType message ) const;

  // void Log( ChannelType channel, SeverityType severity, MessageType message );

private:

  class LoggerImpl;
  std::unique_ptr< LoggerImpl > m_Pimple;
};
} // namespace

#endif // Logger_h
