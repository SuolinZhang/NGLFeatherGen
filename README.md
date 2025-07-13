# NGLFeatherGen

A Qt-based application that demonstrates procedural feather generation using
the NGL graphics library. Feathers are created from Bezier curves for the rachis,
outlines and barbs then rendered in an OpenGL scene.

## Build

1. Install dependencies:
    - CMake >= 3.12
    - NGL library https://github.com/NCCA/NGL.git
    - Qt5 or Qt6 (Widgets and OpenGLWidgets)
    - GoogleTest for optional unit tests

2. Configure and build:

   ```bash
   mkdir build
   cd build
   cmake -G -Ninja ..
   ninja

## Usage
Run the generated Feather executable. The UI provides tabs to tweak rachis,
outline, barb, and full-feather parameters. Press Render to regenerate the
feather with the current settings.

Note: Render output depends on current tab i.e. Only show rachis when pressing render under Rachis tab. 
   
## Diagram
```mermaid
classDiagram
   class BezierCurve {
      +addPoint(ngl::Vec3 point) void
      +setLOD(unsigned int lod) void
      +getPointOnCurve(ngl::Real t) ngl::Vec3
      +getSamplePoints() std::vector<ngl::Vec3>
      +draw() void
      +drawControlPoints() void
      +createVAO() void
      -m_controlPoints std::vector<ngl::Vec3>
      -m_samplePoints std::vector<ngl::Vec3>
      -m_lod unsigned int
      -m_vao GLuint
}

    class Feather {
        +GenerateRachis() void
        +generateOutlines() void
        +generateTemplateBarbs() void
        +generateAllBarbs() void
        +setSampleNum(int num) void
        +setF0(ngl::Real f0) void
        +setFn(ngl::Real fn) void
        +setNumBarbs(unsigned int num) void
        +setBarbLOD(unsigned int lod) void
        +setOutlineMappingRange(ngl::Real start, ngl::Real end) void
        +drawRachis() void
        +drawOutlines() void
        +drawAllBarbs() void
        +drawFullFeather() void
        -m_rachis std::unique_ptr<BezierCurve>
        -m_leftOutline std::unique_ptr<BezierCurve>
        -m_rightOutline std::unique_ptr<BezierCurve>
        -m_leftBarb std::unique_ptr<BezierCurve>
        -m_rightBarb std::unique_ptr<BezierCurve>
        -m_leftBarbs std::vector<std::unique_ptr<BezierCurve>>
        -m_rightBarbs std::vector<std::unique_ptr<BezierCurve>>
        -m_sample unsigned int
        -m_F0 ngl::Real
        -m_Fn ngl::Real
        -m_numBarbs unsigned int
        -m_leftBarbOutlineFactor ngl::Real
        -m_rightBarbOutlineFactor ngl::Real
        -m_outlineMappingStart ngl::Real
        -m_outlineMappingEnd ngl::Real
        -ensureRachisExists() void
    }

    Feather "1" *-- "*" BezierCurve : Composition
    Feather "1" o-- "1" BezierCurve : Rachis
    Feather "1" o-- "1" BezierCurve : LeftOutline
    Feather "1" o-- "1" BezierCurve : RightOutline
    Feather "1" o-- "1" BezierCurve : LeftBarbTemplate
    Feather "1" o-- "1" BezierCurve : RightBarbTemplate
    Feather "1" *-- "*" BezierCurve : LeftBarbs
    Feather "1" *-- "*" BezierCurve : RightBarbs
```