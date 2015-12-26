#ifndef TransformixFilter_hxx
#define TransformixFilter_hxx

namespace selx {

template< typename TInputImage  >
TransformixFilter< TInputImage >
::TransformixFilter( void )
{
  this->AddRequiredInputName( "TransformParameterObject");
  
  this->SetPrimaryInputName( "InputImage" );
  this->SetPrimaryOutputName( "ResultImage" );

  this->ComputeSpatialJacobianOff();
  this->ComputeDeterminantOfSpatialJacobianOff();
  this->ComputeDeformationFieldOff();
  this->m_PointSetFileName = std::string();

  this->m_OutputDirectory = std::string();
  this->m_LogFileName = std::string();
  
  this->LogToConsoleOff();
  this->LogToFileOff();
}

template< typename TInputImage >
void
TransformixFilter< TInputImage >
::GenerateData( void )
{
  // Assert that at least one output has been requested
  if( !this->HasInput( "InputImage" ) &&
      !this->GetComputeSpatialJacobian() &&
      !this->GetComputeDeterminantOfSpatialJacobian() &&
      !this->GetComputeDeformationField() &&
      this->GetPointSetFileName().empty() )
  {
    itkExceptionMacro( "Expected at least one of SetInputImage(), ComputeSpatialJacobianOn(), "
                    << "ComputeDeterminantOfSpatialJacobianOn() or SetPointSetFileName() or " 
                    << "to be set.");
  }

  // Check if an output directory is needed
  // TODO: Change behaviour upstream to have transformix save all outputs as data objects
  if( ( this->GetComputeSpatialJacobian() ||
        this->GetComputeDeterminantOfSpatialJacobian() ||
        this->GetComputeDeformationField() ||
        !this->GetPointSetFileName().empty() ||
        this->GetLogToFile() ) &&
      ( this->GetOutputDirectory().empty() ) )
  {
    itkExceptionMacro( "The requested outputs require an output directory to be specified."
                    << "Use SetOutputDirectory()." )
  }

  if( ( this->GetComputeSpatialJacobian() ||
        this->GetComputeDeterminantOfSpatialJacobian() ||
        this->GetComputeDeformationField() ||
        !this->GetPointSetFileName().empty() ||
        this->GetLogToFile() ) &&
      !itksys::SystemTools::FileExists( this->GetOutputDirectory() ) )
  {
    itkExceptionMacro( "Output directory \"" << this->GetOutputDirectory() << "\" does not exist." )
  }

  // Transformix uses "-def" for path to point sets AND as flag for writing deformatin field
  // TODO: Change behaviour upstream: Split into seperate arguments
  if( this->GetComputeDeformationField() && !this->GetPointSetFileName().empty() )
  {
    itkExceptionMacro( << "For backwards compatibility, only one of ComputeDeformationFieldOn() or SetPointSetFileName() can be "
                       << "active at any one time." )
  }

  // Setup argument map which transformix uses internally ito figure out what needs to be done
  ArgumentMapType argumentMap;
  if( this->GetOutputDirectory().empty() ) {
    if( this->GetLogToFile() )
    {
      itkExceptionMacro( "LogToFileOn() requires an output directory to be specified. Use SetOutputDirectory().")
    }

    // There must be an "-out", this is checked later in the code
    argumentMap.insert( ArgumentMapEntryType( "-out", "output_path_not_set" ) );
  }
  else
  {
    if( !itksys::SystemTools::FileExists( this->GetOutputDirectory() ) )
    {
      itkExceptionMacro( "Output directory \"" << this->GetOutputDirectory() << "\" does not exist." )
    }

    if( this->GetOutputDirectory().back() != '/' || this->GetOutputDirectory().back() != '\\' )
    {
      this->SetOutputDirectory( this->GetOutputDirectory() + "/" );
    }

    argumentMap.insert( ArgumentMapEntryType( "-out", this->GetOutputDirectory() ) );
  }

  if( this->GetComputeSpatialJacobian() )
  {
    argumentMap.insert( ArgumentMapEntryType( "-jacmat", "all" ) );
  }

  if( this->GetComputeDeterminantOfSpatialJacobian() )
  {
    argumentMap.insert( ArgumentMapEntryType( "-jac", "all" ) );
  }

  if( this->GetComputeDeformationField() ) 
  {
    argumentMap.insert( ArgumentMapEntryType( "-def" , "all" ) );
  }

  if( !this->GetPointSetFileName().empty() )
  {
    argumentMap.insert( ArgumentMapEntryType( "-def", this->GetPointSetFileName() ) );
  }

  // Setup xout
  std::string logFileName;
  if( this->GetLogToFile() )
  { 
    if( this->GetLogFileName().empty() )
    {
      logFileName = this->GetOutputDirectory() + "transformix.log";
    }
    else
    {
      logFileName = this->GetOutputDirectory() + this->GetLogFileName();
    }
  }

  if( elx::xoutSetup( logFileName.c_str(), this->GetLogToFile(), this->GetLogToConsole() ) )
  {
    itkExceptionMacro( "ERROR while setting up xout" );
  }

  // Instantiate transformix
  TransformixMainPointer transformix = TransformixMainType::New();

  // Setup image containers
  DataObjectContainerPointer inputImageContainer = 0;
  DataObjectContainerPointer resultImageContainer = 0;

  if( this->HasInput( "InputImage" ) ) {
    DataObjectContainerPointer inputImageContainer = DataObjectContainerType::New();
    inputImageContainer->CreateElementAt( 0 ) = this->GetInput("InputImage");
    transformix->SetInputImageContainer( inputImageContainer );
    transformix->SetResultImageContainer( resultImageContainer );
  }

  // Get ParameterMap
  ParameterObjectConstPointer transformParameterObject = static_cast< const ParameterObject* >( this->GetInput( "TransformParameterObject" ) );
  ParameterMapVectorType transformParameterMapVector = transformParameterObject->GetParameterMap();

  for( unsigned int i = 0; i < transformParameterMapVector.size(); ++i )
  {
    // Transformix reads type information from parameter files. We set this information automatically and overwrite
    // user settings in case they are incorrect (elastix will segfault or throw exception when casting) 
    transformParameterMapVector[ i ][ "MovingInternalImagePixelType" ] = ParameterValueVectorType( 1, TypeName< typename TInputImage::PixelType >::ToString() );    
    transformParameterMapVector[ i ][ "MovingImageDimension" ] = ParameterValueVectorType( 1, std::to_string( InputImageDimension ) );
    transformParameterMapVector[ i ][ "ResultImagePixelType" ] = ParameterValueVectorType( 1, TypeName< typename TInputImage::PixelType >::ToString() );
  }

  // Run transformix
  unsigned int isError = 0;
  try
  {
    isError = transformix->Run( argumentMap, transformParameterMapVector );
  }
  catch( itk::ExceptionObject &e )
  {
    itkExceptionMacro( << "Errors occured during registration: " << e.what() );
  }

  if( isError != 0 )
  {
    itkExceptionMacro( << "Uncought errors occured during registration." );
  }

  // Save result image
  resultImageContainer = transformix->GetResultImageContainer();
  if( resultImageContainer.IsNotNull() && resultImageContainer->Size() > 0 )
  {
    this->GraftOutput( "ResultImage", resultImageContainer->ElementAt( 0 ) );
  }

  // Clean up
  TransformixMainType::UnloadComponents();
}

template< typename TInputImage >
void
TransformixFilter< TInputImage >
::SetInputImage( InputImagePointer inputImage )
{
  this->SetInput( "InputImage", static_cast< itk::DataObject* >( inputImage ) );
}

template< typename TInputImage >
void
TransformixFilter< TInputImage >
::SetTransformParameterObject( ParameterObjectPointer parameterObject )
{
  this->SetInput( "TransformParameterObject", static_cast< itk::DataObject* >( parameterObject ) );
}

template< typename TInputImage >
typename selx::TransformixFilter< TInputImage >::ParameterObjectPointer
TransformixFilter< TInputImage >
::GetTransformParameters( void )
{
  return static_cast< ParameterObject* >( this->GetInput( "TransformParameterObject" ) );
}

} // namespace selx

#endif // TransformixFilter_hxx