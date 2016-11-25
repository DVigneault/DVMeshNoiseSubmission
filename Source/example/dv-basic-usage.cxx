// Generate mesh
#include <itkMesh.h>
#include <itkRegularSphereMeshSource.h>
#include <itkAdditiveGaussianNoiseMeshFilter.h>
#include <itkMeshFileWriter.h>

typedef double     TPixel;
const unsigned int Dimension = 3;

typedef itk::Mesh< TPixel, Dimension > TMesh;
typedef itk::RegularSphereMeshSource< TMesh > TSphere;
typedef itk::AdditiveGaussianNoiseMeshFilter< TMesh, TMesh > TNoise;
typedef itk::MeshFileWriter< TMesh > TMeshWriter;

int
main()
{

  const auto sphere = TSphere::New();
  sphere->SetResolution( 2 );

    {
    const auto writer = TMeshWriter::New();
    writer->SetInput( sphere->GetOutput() );
    writer->SetFileName( "../data/sphere.vtk" );
    writer->Update();
    }

  const auto noise = TNoise::New();
  noise->SetInput( sphere->GetOutput() );
  noise->SetSeed( 0 );
  noise->SetMean( 0.0 );
  noise->SetSigma( 0.05 );

    {
    const auto writer = TMeshWriter::New();
    writer->SetInput( noise->GetOutput() );
    writer->SetFileName( "../data/sphere_noise.vtk" );
    writer->Update();
    }

  return EXIT_SUCCESS;

}
