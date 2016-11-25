// Generate mesh
#include <itkImage.h>
#include <itkQuadEdgeMesh.h>
#include <dvGenerateHalfSphere.h>
#include <itkAdditiveGaussianNoiseQuadEdgeMeshFilter.h>
#include <itkMeshFileWriter.h>

#include <itkQuadEdgeMeshDecimationCriteria.h>
#include <itkQuadricDecimationQuadEdgeMeshFilter.h>

#include <map>

#include <fstream>

typedef double     TPixel;
const unsigned int Dimension = 3;

typedef itk::QuadEdgeMesh< TPixel, Dimension > TMesh;
typedef itk::AdditiveGaussianNoiseQuadEdgeMeshFilter< TMesh, TMesh > TNoise;
typedef itk::MeshFileWriter< TMesh > TMeshWriter;

typedef itk::NumberOfFacesCriterion< TMesh > TCriterion;
typedef itk::QuadricDecimationQuadEdgeMeshFilter< TMesh,
                                                  TMesh,
                                                  TCriterion > TDecimation;

namespace dv
{
void
WriteMesh(TMesh::Pointer mesh, const std::string &fileName)
{
  const auto writer = TMeshWriter::New();
  writer->SetInput( mesh );
  writer->SetFileName( fileName );
  writer->Update();
}

template<typename T>
void
WriteContainer(const T &container, const std::string fileName)
{

  std::ofstream file(fileName);
  if (!file.good())
    {
    std::cerr << "Problem opening the file." << std::endl;
    return;
    }
  for (const auto c : container)
    file << c << '\n';
  file.close();
}

std::vector<unsigned int>
CalculateValencies(const TMesh::PointsContainerPointer points)
{
    std::vector<unsigned int> valency;

    for (auto it = points->Begin();
         it != points->End();
         ++it)
      {
      valency.emplace_back(it->Value().GetValence());
      }

    return valency;
}

std::vector<double>
CalculateAreas(TMesh::Pointer mesh)
{
  std::vector<double> areas;
  std::array<TMesh::PointType, 3> points;
  using TTriangle = itk::TriangleHelper< TMesh::PointType >;
  for (auto cell = mesh->GetCells()->Begin();
       cell != mesh->GetCells()->End();
       ++cell)
    {
    points[0] = mesh->GetPoints()->ElementAt(*(cell->Value()->PointIdsBegin()  ));
    points[1] = mesh->GetPoints()->ElementAt(*(cell->Value()->PointIdsBegin()+1));
    points[2] = mesh->GetPoints()->ElementAt(*(cell->Value()->PointIdsBegin()+2));
    areas.emplace_back(TTriangle::ComputeArea( points[0], points[1], points[2] ));
    }

  return areas;
}

std::vector<double>
CalculateAspectRatios(TMesh::Pointer mesh)
{
  std::vector<double> ratios;
  std::array<TMesh::PointType, 3> points;
  for (auto cell = mesh->GetCells()->Begin();
       cell != mesh->GetCells()->End();
       ++cell)
    {
    points[0] = mesh->GetPoints()->ElementAt(*(cell->Value()->PointIdsBegin()  ));
    points[1] = mesh->GetPoints()->ElementAt(*(cell->Value()->PointIdsBegin()+1));
    points[2] = mesh->GetPoints()->ElementAt(*(cell->Value()->PointIdsBegin()+2));
    const auto L0 = points[0].EuclideanDistanceTo(points[1]);
    const auto L1 = points[1].EuclideanDistanceTo(points[2]);
    const auto L2 = points[2].EuclideanDistanceTo(points[0]);
    const auto maximum = std::max(std::max(L0, L1), L2);
    const auto minimum = std::min(std::min(L0, L1), L2);
    ratios.emplace_back( maximum / minimum );
    }

  return ratios;
}
}

int
main()
{

  ///////////
  // Clean //
  ///////////

  const auto half_sphere = TMesh::New();
  half_sphere->Graft( dv::GenerateHalfSphere<TMesh>() );

    {
    dv::WriteMesh(half_sphere, "../data/half_sphere.vtk");

    const auto target = half_sphere->GetNumberOfCells() / 100;
    const auto criterion = TCriterion::New();
    criterion->SetTopologicalChange( false );
    criterion->SetNumberOfElements( target );

    const auto decimate = TDecimation::New();
    decimate->SetCriterion( criterion );
    decimate->SetInput( half_sphere );

    dv::WriteMesh(decimate->GetOutput(), "../data/half_sphere_decimated.vtk" );

    dv::WriteContainer( dv::CalculateValencies(decimate->GetOutput()->GetPoints()),
                        "../data/clean_valencies.txt" );
    dv::WriteContainer( dv::CalculateAreas(decimate->GetOutput()),
                        "../data/clean_areas.txt" );
    dv::WriteContainer( dv::CalculateAspectRatios(decimate->GetOutput()),
                        "../data/clean_ratios.txt" );

    }

  ///////////
  // Noise //
  ///////////

  const auto noise = TNoise::New();
  noise->SetInput( half_sphere );
  noise->SetSigma( 0.01 );
  noise->Update();

  const auto half_sphere_noise = TMesh::New();
  half_sphere_noise->Graft( noise->GetOutput() );

    {
    dv::WriteMesh(half_sphere_noise, "../data/half_sphere_noise.vtk");

    const auto target = half_sphere_noise->GetNumberOfCells() / 100;
    const auto criterion = TCriterion::New();
    criterion->SetTopologicalChange( false );
    criterion->SetNumberOfElements( target );

    const auto decimate = TDecimation::New();
    decimate->SetCriterion( criterion );
    decimate->SetInput( half_sphere_noise );

    dv::WriteMesh(decimate->GetOutput(), "../data/half_sphere_noise_decimated.vtk" );

    dv::WriteContainer( dv::CalculateValencies(decimate->GetOutput()->GetPoints()),
                        "../data/noise_valencies.txt" );
    dv::WriteContainer( dv::CalculateAreas(decimate->GetOutput()),
                        "../data/noise_areas.txt" );
    dv::WriteContainer( dv::CalculateAspectRatios(decimate->GetOutput()),
                        "../data/noise_ratios.txt" );

    }

  return EXIT_SUCCESS;

}
