/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
/*=========================================================================
 *
 *  Portions of this file are subject to the VTK Toolkit Version 3 copyright.
 *
 *  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
 *
 *  For complete copyright, license and disclaimer of warranty information
 *  please refer to the NOTICE file at the top of the ITK source tree.
 *
 *=========================================================================*/
#ifndef selxSharedPointerDataObjectDecorator_h
#define selxSharedPointerDataObjectDecorator_h

#include <memory>
#include "itkDataObject.h"
#include "itkObjectFactory.h"

namespace itk
{
/** \class SharedPointerDataObjectDecorator
 * \brief Decorates any pointer to a simple object with a DataObject API using
 * SharedPointer semantics.
 *
 * SharedPointerDataObjectDecorator decorates a pointer to an object
 * with a DataObject API. This allows a pointer to an object to be
 * encapsulated in a DataObject and passed through the pipeline. This
 * object differs from SimpleDataObjectDecorator in that the decorator
 * takes control of deleting the pointer upon destruction.
 *
 * The decorator provides two methods Set() and Get() to access the
 * decorated object (referred internally as the component).
 *
 * Note that when an instance of SimpleDataObjectDecorator is created,
 * the component is initialized with its default constructor (i.e. a
 * null pointer).
 *
 * \sa AutoPointerDataObjectDecorator
 * \sa SimpleDataObjectDecorator
 * \sa DataObjectDecorator
 * \ingroup ITKSystemObjects
 *
 * \ingroup ITKCommon
 */
template< typename T >
class SharedPointerDataObjectDecorator : public DataObject
{
public:

  /** Standard typedefs. */
  typedef SharedPointerDataObjectDecorator Self;
  typedef DataObject                       Superclass;
  typedef SmartPointer< Self >             Pointer;
  typedef SmartPointer< const Self >       ConstPointer;

  /** Typedef for the component type (object being decorated) */
  typedef T                    ComponentType;
  typedef std::shared_ptr< T > ComponentPointer;

  /** Method for creation through the object factory. */
  itkNewMacro( Self );

  /** Run-time type information (and related methods). */
  itkTypeMacro( SharedPointerDataObjectDecorator, DataObject );

  /** Set the contained object */
  virtual void Set( std::shared_ptr< T > val );

  /** Get the contained object */
  virtual std::shared_ptr< T > Get() { return m_Component; }
  virtual const std::shared_ptr< T > Get() const { return m_Component; }

protected:

  SharedPointerDataObjectDecorator();
  ~SharedPointerDataObjectDecorator();
  virtual void PrintSelf( std::ostream & os, Indent indent ) const ITK_OVERRIDE;

protected:

private:

  // ITK_DISALLOW_COPY_AND_ASSIGN(SharedPointerDataObjectDecorator);

  ComponentPointer m_Component;
};
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkSharedPointerDataObjectDecorator.hxx"
#endif

#endif
