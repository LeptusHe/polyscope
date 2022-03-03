// Copyright 2017-2019, Nicholas Sharp and the Polyscope contributors. http://polyscope.run.
#pragma once

// Various types / enums / forward declarations which are broadly useful

namespace polyscope {

enum class NavigateStyle { Turntable = 0, Free, Planar, Arcball };
enum class UpDir { XUp = 0, YUp, ZUp, NegXUp, NegYUp, NegZUp};
enum class BackgroundView { None = 0 };
enum class ProjectionMode { Perspective = 0, Orthographic };
enum class TransparencyMode { None = 0, Simple, Pretty };
enum class GroundPlaneMode { None, Tile, TileReflection, ShadowOnly };
enum class BackFacePolicy { Identical, Different, Custom, Cull };
enum class ShadeStyle { FLAT = 0, SMOOTH };

enum class PointRenderMode { Sphere = 0, Quad};
enum class MeshElement { VERTEX = 0, FACE, EDGE, HALFEDGE, CORNER };
enum class VolumeMeshElement { VERTEX = 0, EDGE, FACE, CELL };
enum class VolumeCellType { TET = 0, HEX };

// What is the meaningful range of these values?
// Used to set meaningful colormaps
// STANDARD: [-inf, inf], zero does not mean anything special (ie, position)
// SYMMETRIC: [-inf, inf], zero is special (ie, net profit/loss)
// MAGNITUDE: [0, inf], zero is special (ie, length of a vector)
enum class DataType { STANDARD = 0, SYMMETRIC, MAGNITUDE };


}; // namespace polyscope
