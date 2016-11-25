#include <dvWireFrame.h>

namespace dv
{

void
WireFrame
::Setup()
{

  // Edges
  this->edges = vtkSmartPointer<vtkExtractEdges>::New();
  this->edges->SetInputData( this->mesh );

  this->tubes = vtkSmartPointer<vtkTubeFilter>::New();
  this->tubes->SetInputConnection( this->edges->GetOutputPort() );
  this->tubes->SetRadius( this->radius );
  this->tubes->SetNumberOfSides( this->tubesSides );

  this->tubesMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->tubesMapper->SetInputConnection(this->tubes->GetOutputPort());
 
  this->tubesActor = vtkSmartPointer<vtkActor>::New();
  this->tubesActor->SetMapper(this->tubesMapper);
  this->tubesActor->GetProperty()->SetColor( this->wireframeColor );

  // Vertices
  this->vertices = vtkSmartPointer<vtkGlyph3D>::New();
  this->sphere = vtkSmartPointer<vtkSphereSource>::New();
  this->sphere->SetRadius( this->radius * 2.0 );

  this->vertices->SetSourceConnection(this->sphere->GetOutputPort());

  this->verticesMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->verticesActor = vtkSmartPointer<vtkActor>::New();

  this->vertices->SetInputData(                 this->mesh );
  this->verticesMapper->SetInputConnection(     this->vertices->GetOutputPort() );
  this->verticesActor->SetMapper(               this->verticesMapper );
  this->verticesActor->GetProperty()->SetColor( this->wireframeColor );

  // Surface
  this->surfaceTriangulate = vtkSmartPointer<vtkTriangleFilter>::New();
  this->surfaceTriangulate->SetInputData( this->mesh );
  this->surfaceFilter = vtkSmartPointer<vtkLoopSubdivisionFilter>::New();
  this->surfaceFilter->SetInputConnection( this->surfaceTriangulate->GetOutputPort() );
  this->surfaceFilter->SetNumberOfSubdivisions( 5 );
  this->surfaceFilter->Update();

  this->surfaceMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  this->surfaceMapper->SetInputData( this->surfaceFilter->GetOutput() );

  this->surfaceActor = vtkSmartPointer<vtkActor>::New();
  this->surfaceActor->SetMapper( this->surfaceMapper );
  this->surfaceActor->GetProperty()->SetColor( this->surfaceColor );

  this->hasBeenSetup = true; 
}

void
WireFrame
::SetVisible(bool visible)
{
  if (!this->hasBeenSetup) return;

  if (visible)
    {
    this->renderer->AddActor( this->tubesActor );
    this->renderer->AddActor( this->verticesActor );
    this->renderer->AddActor( this->surfaceActor );
    }
  else
    {
    this->renderer->RemoveActor( this->tubesActor );
    this->renderer->RemoveActor( this->verticesActor );
    this->renderer->RemoveActor( this->surfaceActor );
    }
}

}
