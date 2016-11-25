#ifndef dvWireFrame_h
#define dvWireFrame_h

#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkExtractEdges.h>
#include <vtkTubeFilter.h>
#include <vtkGlyph3D.h>
#include <vtkSphereSource.h>
#include <vtkRenderer.h>
#include <vtkProperty.h>
#include <vtkLoopSubdivisionFilter.h>
#include <vtkTriangleFilter.h>

namespace dv
{
class WireFrame
{

public:

  WireFrame(vtkRenderer* _renderer,
            vtkPolyData* _mesh)
  : renderer(_renderer), mesh(_mesh){}

  double radius = 0.015;
  unsigned int tubesSides = 10;
  double wireframeColor[3] = {1.0, 0.4, 0.4};
  double surfaceColor[3] = {1.0, 0.8, 0.8};

  void
  Setup();

  void
  SetVisible(bool visible);

private:

  // Edges
  vtkSmartPointer<vtkExtractEdges> edges = nullptr;
  vtkSmartPointer<vtkTubeFilter> tubes = nullptr;
  vtkSmartPointer<vtkPolyDataMapper> tubesMapper = nullptr;
  vtkSmartPointer<vtkActor> tubesActor = nullptr;

  // Vertices
  vtkSmartPointer<vtkGlyph3D> vertices = nullptr;
  vtkSmartPointer<vtkSphereSource> sphere = nullptr;

  vtkSmartPointer<vtkPolyDataMapper> verticesMapper = nullptr;
  vtkSmartPointer<vtkActor> verticesActor = nullptr;

  // Surface
  vtkSmartPointer<vtkTriangleFilter> surfaceTriangulate = nullptr;
  vtkSmartPointer<vtkLoopSubdivisionFilter> surfaceFilter = nullptr;

  vtkSmartPointer<vtkPolyDataMapper> surfaceMapper = nullptr;
  vtkSmartPointer<vtkActor> surfaceActor = nullptr;

  // Other
  vtkRenderer* renderer = nullptr;
  vtkPolyData* mesh = nullptr;

  bool hasBeenSetup = false;

};

}

#endif
