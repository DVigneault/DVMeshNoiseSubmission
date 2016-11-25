#ifndef dvGenerateHalfSphere_h
#define dvGenerateHalfSphere_h

#include <itkImage.h>
#include <itkMesh.h>
#include <itkRegularSphereMeshSource.h>
#include <itkTriangleMeshToBinaryImageFilter.h>
#include <itkConstantPadImageFilter.h>
#include <itkBinaryMask3DMeshSource.h>

namespace dv
{
template< typename TMesh >
typename TMesh::Pointer
GenerateHalfSphere(const int RES = 6,
                   const int SCALE = 5,
                   const int DENSITY = 5)
{

  typedef itk::Image< typename TMesh::PixelType, TMesh::MeshTraits::PointDimension > TVolume;
  typedef itk::RegularSphereMeshSource< TMesh > TSphere;
  typedef itk::TriangleMeshToBinaryImageFilter< TMesh, TVolume > TBinary;
  typedef itk::ConstantPadImageFilter< TVolume, TVolume > TPad;
  typedef itk::BinaryMask3DMeshSource< TVolume, TMesh > TMarch;

  const auto sphere = TSphere::New();

  sphere->SetResolution( RES );
  sphere->SetScale( SCALE );
  sphere->Update();

  const auto mesh2binary = TBinary::New();

  mesh2binary->SetInput( sphere->GetOutput() );
  const float origin[3] = {-1.0f*(SCALE+1.0f), -1.0f*(SCALE+1.0f), -1.0f};
  mesh2binary->SetOrigin( origin );
  const float spacing[3] = {1.0f / DENSITY, 1.0f / DENSITY, 1.0f / DENSITY};
  mesh2binary->SetSpacing( spacing );
  const typename TBinary::SizeType size = {{2ul*DENSITY*(SCALE+1ul),
                                            2ul*DENSITY*(SCALE+1ul),
                                            DENSITY*(SCALE+2ul)}};
  mesh2binary->SetSize( size );
  mesh2binary->Update();

  const auto pad = TPad::New();
  typename TPad::SizeType bounds = {{1, 1, 1}};
  pad->SetPadUpperBound( bounds );
  pad->SetPadLowerBound( bounds );
  pad->SetInput( mesh2binary->GetOutput() );

  const auto binary2mesh = TMarch::New();
  binary2mesh->SetInput( pad->GetOutput() );
  binary2mesh->Update();

  return binary2mesh->GetOutput();

}

}

#endif
