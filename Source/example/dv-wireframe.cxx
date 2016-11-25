// VTK
#include <vtkSphereSource.h>
#include <vtkPolyDataReader.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkBoundingBox.h>

// Custom
#include <dvWireFrame.h>

int
main(int argc, char** argv)
{

  vtkSmartPointer<vtkPolyData> mesh = nullptr;

  if (1 == argc)
    {
    const auto source = vtkSmartPointer<vtkSphereSource>::New();
    source->Update();
    mesh = source->GetOutput();
    }
  else
    {
    const auto source = vtkSmartPointer<vtkPolyDataReader>::New();
    source->SetFileName( argv[1] );
    source->Update();
    mesh = source->GetOutput();
    }

  const double length = mesh->GetLength();

  const auto renderer = vtkSmartPointer<vtkRenderer>::New();
  renderer->SetBackground(1.0, 1.0, 1.0);

  auto wireframe = dv::WireFrame(renderer, mesh);
  wireframe.radius = length * 0.005;
  wireframe.Setup();
  wireframe.SetVisible(true);

  const auto window = vtkSmartPointer<vtkRenderWindow>::New();
  window->SetSize(500, 500);
  window->AddRenderer(renderer);
 
  const auto renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(window);
 
  renderWindowInteractor->Start();
 
  return EXIT_SUCCESS;
}
