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

#include "selxLogger.h"

#include "gtest/gtest.h"

using namespace selx;

TEST( LoggerTest, Initialization )
{
  Logger * logger = new Logger();
}

TEST( LoggerTest, Console )
{
  Logger * logger = new Logger();
  logger->AddConsole();
  logger->Log( SeverityType::SELX_TRACE, "Console TRACE message" );
  logger->Log( SeverityType::SELX_DEBUG, "Console DEBUG message" );
  logger->Log( SeverityType::SELX_INFO, "Console INFO message" );
  logger->Log( SeverityType::SELX_WARNING, "Console WARNING message" );
  logger->Log( SeverityType::SELX_ERROR, "Console ERROR message" );
  logger->Log( SeverityType::SELX_FATAL, "Console FATAL message" );
}
