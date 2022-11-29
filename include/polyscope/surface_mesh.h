// Copyright 2017-2019, Nicholas Sharp and the Polyscope contributors. http://polyscope.run.
#pragma once

#include <memory>
#include <vector>

#include "polyscope/affine_remapper.h"
#include "polyscope/color_management.h"
#include "polyscope/polyscope.h"
#include "polyscope/render/engine.h"
#include "polyscope/standardize_data_array.h"
#include "polyscope/structure.h"
#include "polyscope/surface_mesh_quantity.h"
#include "polyscope/types.h"

// Alllll the quantities
#include "polyscope/surface_color_quantity.h"
#include "polyscope/surface_count_quantity.h"
#include "polyscope/surface_distance_quantity.h"
#include "polyscope/surface_graph_quantity.h"
#include "polyscope/surface_parameterization_enums.h"
#include "polyscope/surface_parameterization_quantity.h"
#include "polyscope/surface_scalar_quantity.h"
#include "polyscope/surface_vector_quantity.h"
// #include "polyscope/surface_selection_quantity.h"
// #include "polyscope/surface_subset_quantity.h"


namespace polyscope {

// Forward declarations for quantities
class SurfaceVertexColorQuantity;
class SurfaceFaceColorQuantity;
class SurfaceVertexScalarQuantity;
class SurfaceFaceScalarQuantity;
class SurfaceEdgeScalarQuantity;
class SurfaceHalfedgeScalarQuantity;
class SurfaceVertexScalarQuantity;
class SurfaceCornerParameterizationQuantity;
class SurfaceVertexParameterizationQuantity;
class SurfaceVertexVectorQuantity;
class SurfaceFaceVectorQuantity;
class SurfaceVertexIntrinsicVectorQuantity;
class SurfaceFaceIntrinsicVectorQuantity;
class SurfaceOneFormIntrinsicVectorQuantity;
class SurfaceVertexCountQuantity;
class SurfaceVertexIsolatedScalarQuantity;
class SurfaceFaceCountQuantity;
class SurfaceGraphQuantity;


template <> // Specialize the quantity type
struct QuantityTypeHelper<SurfaceMesh> {
  typedef SurfaceMeshQuantity type;
};


// === The grand surface mesh class

class SurfaceMesh : public QuantityStructure<SurfaceMesh> {
public:
  typedef SurfaceMeshQuantity QuantityType;

  // === Member functions ===

  // Construct a new surface mesh structure
  SurfaceMesh(std::string name, const std::vector<glm::vec3>& vertexPositions,
              const std::vector<std::vector<size_t>>& faceIndices);

  // Build the imgui display
  virtual void buildCustomUI() override;
  virtual void buildCustomOptionsUI() override;
  virtual void buildPickUI(size_t localPickID) override;

  // Render the the structure on screen
  virtual void draw() override;

  // Render for picking
  virtual void drawPick() override;

  virtual void updateObjectSpaceBounds() override;
  virtual std::string typeName() override;

  virtual void refresh() override;

  // === Quantity-related
  // clang-format off

  // = Scalars (expect scalar array)
  template <class T> SurfaceVertexScalarQuantity* addVertexScalarQuantity(std::string name, const T& data, DataType type = DataType::STANDARD); 
  template <class T> SurfaceFaceScalarQuantity* addFaceScalarQuantity(std::string name, const T& data, DataType type = DataType::STANDARD); 
  template <class T> SurfaceEdgeScalarQuantity* addEdgeScalarQuantity(std::string name, const T& data, DataType type = DataType::STANDARD); 
  template <class T> SurfaceHalfedgeScalarQuantity* addHalfedgeScalarQuantity(std::string name, const T& data, DataType type = DataType::STANDARD);

  // = Distance (expect scalar array)
  template <class T> SurfaceVertexScalarQuantity* addVertexDistanceQuantity(std::string name, const T& data);
  template <class T> SurfaceVertexScalarQuantity* addVertexSignedDistanceQuantity(std::string name, const T& data);

  // = Colors (expect vec3 array)
  template <class T> SurfaceVertexColorQuantity* addVertexColorQuantity(std::string name, const T& data);
  template <class T> SurfaceFaceColorQuantity* addFaceColorQuantity(std::string name, const T& data);
  
	// = Parameterizations (expect vec2 array)
  template <class T> SurfaceCornerParameterizationQuantity* addParameterizationQuantity(std::string name, const T& coords, ParamCoordsType type = ParamCoordsType::UNIT); 
	template <class T> SurfaceVertexParameterizationQuantity* addVertexParameterizationQuantity(std::string name, const T& coords, ParamCoordsType type = ParamCoordsType::UNIT);
  template <class T> SurfaceVertexParameterizationQuantity* addLocalParameterizationQuantity(std::string name, const T& coords, ParamCoordsType type = ParamCoordsType::WORLD);
  
	// = Vectors (expect vector array, inner type must be indexable with correct dimension (3 for extrinsic, 2 for intrinsic) 
	template <class T> SurfaceVertexVectorQuantity* addVertexVectorQuantity(std::string name, const T& vectors, VectorType vectorType = VectorType::STANDARD); 
	template <class T> SurfaceVertexVectorQuantity* addVertexVectorQuantity2D(std::string name, const T& vectors, VectorType vectorType = VectorType::STANDARD); 
	template <class T> SurfaceFaceVectorQuantity* addFaceVectorQuantity(std::string name, const T& vectors, VectorType vectorType = VectorType::STANDARD); 
	template <class T> SurfaceFaceVectorQuantity* addFaceVectorQuantity2D(std::string name, const T& vectors, VectorType vectorType = VectorType::STANDARD); 
	template <class T> SurfaceFaceIntrinsicVectorQuantity* addFaceIntrinsicVectorQuantity(std::string name, const T& vectors, int nSym = 1, VectorType vectorType = VectorType::STANDARD); 
	template <class T> SurfaceVertexIntrinsicVectorQuantity* addVertexIntrinsicVectorQuantity(std::string name, const T& vectors, int nSym = 1, VectorType vectorType = VectorType::STANDARD); 
	template <class T, class O> SurfaceOneFormIntrinsicVectorQuantity* addOneFormIntrinsicVectorQuantity(std::string name, const T& data, const O& orientations);


  // = Counts/Values on isolated vertexPositions (expect index/value pairs)
  SurfaceVertexCountQuantity* addVertexCountQuantity(std::string name, const std::vector<std::pair<size_t, int>>&
  values); 
	SurfaceFaceCountQuantity* addFaceCountQuantity(std::string name, const std::vector<std::pair<size_t, int>>&
  values); 
	SurfaceVertexIsolatedScalarQuantity* addVertexIsolatedScalarQuantity(std::string name, const std::vector<std::pair<size_t, double>>& values);


  // = Misc quantities
  template <class P, class E>
  SurfaceGraphQuantity* addSurfaceGraphQuantity(std::string name, const P& nodes, const E& edges);
  template <class P, class E>
  SurfaceGraphQuantity* addSurfaceGraphQuantity2D(std::string name, const P& nodes, const E& edges);
  template <class P>
  SurfaceGraphQuantity* addSurfaceGraphQuantity(std::string name, const std::vector<P>& paths);
  template <class P>
  SurfaceGraphQuantity* addSurfaceGraphQuantity2D(std::string name, const std::vector<P>& paths);

  // = I/O Selections
  template <class T>
  void addVertexSelectionQuantity(std::string name, const T& initialMembership);
  // void addInputCurveQuantity(std::string name);

  // clang-format on


  // === Make a one-time selection
  long long int selectVertex();
  // size_t selectFace();

  // === Mutate
  template <class V>
  void updateVertexPositions(const V& newPositions);
  template <class V>
  void updateVertexPositions2D(const V& newPositions2D);


  // === Indexing conventions

  // Permutation arrays. Empty == default ordering
  std::vector<size_t> vertexPerm;
  std::vector<size_t> facePerm;
  std::vector<size_t> edgePerm;
  std::vector<size_t> halfedgePerm;
  std::vector<size_t> cornerPerm;

  // Set permutations
  template <class T>
  void setEdgePermutation(const T& perm, size_t expectedSize = 0);
  template <class T>
  void setHalfedgePermutation(const T& perm, size_t expectedSize = 0);
  template <class T>
  void setCornerPermutation(const T& perm, size_t expectedSize = 0);

  // Get the expected data length, either using the default convention or a permutation as above
  size_t vertexDataSize;
  size_t faceDataSize;
  size_t edgeDataSize;
  size_t halfedgeDataSize;
  size_t cornerDataSize;


  // === Helpers

  // === Manage the mesh itself

  // Core data
  std::vector<std::vector<size_t>> faces; // TODO delete

  // Face indices
  std::vector<uint32_t> faceInds_start;
  std::vector<uint32_t> faceInds_entries;
  std::vector<glm::uvec3> triangleInds; // always triangulated

  // Derived indices
  std::vector<glm::uvec3> faceVertexInds; // TODO delete
  std::vector<std::vector<size_t>> edgeIndices;
  std::vector<std::vector<size_t>> halfedgeIndices;

  // Counts
  size_t nVertices() const;
  size_t nFaces() const { return faces.size(); }

  size_t nFacesTriangulationCount = 0;
  size_t nFacesTriangulation() const { return faces.size(); } // TODO

  size_t nEdgesCount = 0;
  size_t nEdges() const { return nEdgesCount; }

  size_t nCornersCount = 0; // = nHalfedges = sum face degree
  size_t nCorners() const { return nCornersCount; }
  size_t nHalfedges() const { return nCornersCount; }

  // =================================================
  // ========    Geometric Quantities      ==========
  // =================================================
  //
  // Neeeded for various visualization purposes. These all
  // follow the same pattern of:
  //   - a CPU buffer where the value is originally calculated
  //   - a GPU attribute where the value is stored to render
  //   - ensureHave___(), which makes sure the CPU buffer is filled,
  //     call it before any use. It will NOT update it if already full;
  //     empty the buffer if it needs to be updated.
  //   - get___RenderBuffer(), which returns a ref to the GPU buffer,
  //     populating it if needed
  //
  //  NOTE: if a buffer is being manually updated on-device via the renderBuffer
  //  pointer, then the corresponding CPU buffer will be empty.
  //
  //  NOTE: geometric quantities are derived from positions. If the positions are manually updated, the other quantities
  //  must also be manually updated, or they will become stale.

  // Vertex positions
  std::vector<glm::vec3> vertexPositions;
  glm::vec3 getVertexPosition(size_t ind);
  std::shared_ptr<render::AttributeBuffer> getVertexPositionsRenderBuffer();
  bool vertexPositionsStoredInMemory() const;
  void checkIfVertexPositionsAreInMemory() const;
  void vertexPositionRenderBufferDataExternallyUpdated(); // After updating any data via the buffer above, call this
                                                          // function to let Polyscope know.

  // Face normals
  std::vector<glm::vec3> faceNormals; // TODO make all of these private?
  void ensureHaveFaceNormals(bool updateRenderBuffer = true);
  std::shared_ptr<render::AttributeBuffer> getFaceNormalsRenderBuffer();

  // Face centers
  std::vector<glm::vec3> faceCenters;
  void ensureHaveFaceCenters(bool updateRenderBuffer = true);
  std::shared_ptr<render::AttributeBuffer> getFaceCentersRenderBuffer();

  // Face areas
  std::vector<double> faceAreas;
  void ensureHaveFaceAreas(bool updateRenderBuffer = true);
  std::shared_ptr<render::AttributeBuffer> getFaceAreasRenderBuffer();

  // Vertex normals
  std::vector<glm::vec3> vertexNormals;
  void ensureHaveVertexNormals(bool updateRenderBuffer = true);
  std::shared_ptr<render::AttributeBuffer> getVertexNormalsRenderBuffer();

  // Vertex areas
  std::vector<double> vertexAreas;
  void ensureHaveVertexAreas(bool updateRenderBuffer = true);
  std::shared_ptr<render::AttributeBuffer> getVertexAreasRenderBuffer();

  // Edge lengths
  std::vector<double> edgeLengths;
  void ensureHaveEdgeLengths(bool updateRenderBuffer = true);
  std::shared_ptr<render::AttributeBuffer> getEdgeLengthsRenderBuffer();

  // Face tangent spaces
  // The user
  // own, set them with setFaceTangentBasisX()
  std::vector<std::array<glm::vec3, 2>> faceTangentSpaces;
  void ensureHaveFaceTangentSpaces(bool updateRenderBuffer = true);
  std::shared_ptr<render::AttributeBuffer> getFaceTangentSpacesRenderBuffer();

  // Face tangent spaces
  // Polyscope automatically generates default tangent spaces. If you to use your
  // own, set them with setFaceTangentBasisX()
  std::vector<std::array<glm::vec3, 2>> vertexTangentSpaces;
  void ensureHaveVertexTangentSpaces(bool updateRenderBuffer = true);
  std::shared_ptr<render::AttributeBuffer> getVertexTangentSpacesRenderBuffer();

  // Derived connectivity quantities
  // Not necessarily populated by default. Call ensureHaveManifoldConnectivity() to be sure they are populated.
  std::vector<size_t> faceForHalfedge; // for halfedge i, the index of the face it is in
  // Note that these are only really well-defined on a manifold mesh. On a non-manifold mesh, mesh, they will just
  // point to _some_ sane entry
  std::vector<size_t> twinHalfedge; // for halfedge i, the index of a twin halfedge

  // = Mesh helpers
  void computeCounts();       // call to populate counts and indices
  void computeGeometryData(); // call to populate normals/areas/lengths
  void ensureHaveManifoldConnectivity();

  // Set tangent space coordinates for vertices
  template <class T>
  void setVertexTangentBasisX(const T& vectors);
  template <class T>
  void setVertexTangentBasisX2D(const T& vectors);

  // Set tangent space coordinates for faces
  template <class T>
  void setFaceTangentBasisX(const T& vectors);
  template <class T>
  void setFaceTangentBasisX2D(const T& vectors);

  static const std::string structureTypeName;

  // Picking helpers
  // One of these will be non-null on return
  // void getPickedElement(size_t localPickID, size_t& vOut, size_t& fOut, size_t& eOut, size_t& heOut);

  // === Getters and setters for visualization settings

  // Color of the mesh
  SurfaceMesh* setSurfaceColor(glm::vec3 val);
  glm::vec3 getSurfaceColor();

  // Color of edges
  SurfaceMesh* setEdgeColor(glm::vec3 val);
  glm::vec3 getEdgeColor();

  // Material
  SurfaceMesh* setMaterial(std::string name);
  std::string getMaterial();

  // Backface color
  SurfaceMesh* setBackFaceColor(glm::vec3 val);
  glm::vec3 getBackFaceColor();

  // Width of the edges. Scaled such that 1 is a reasonable weight for visible edges, but values  1 can be used for
  // bigger edges. Use 0. to disable.
  SurfaceMesh* setEdgeWidth(double newVal);
  double getEdgeWidth();

  // Backface policy
  SurfaceMesh* setBackFacePolicy(BackFacePolicy newPolicy);
  BackFacePolicy getBackFacePolicy();

  // Face normal type
  SurfaceMesh* setShadeStyle(MeshShadeStyle newStyle);
  MeshShadeStyle getShadeStyle();

  // == Rendering helpers used by quantities

  void setSurfaceMeshUniforms(render::ShaderProgram& p);
  void fillGeometryBuffers(render::ShaderProgram& p);
  std::vector<std::string> addSurfaceMeshRules(std::vector<std::string> initRules, bool withMesh = true,
                                               bool withSurfaceShade = true);

  void setMeshIndexAttribues(render::ShaderProgram& p);
  void setMeshGeometryAttributes(render::ShaderProgram& p);

  // === ~DANGER~ experimental/unsupported functions

  // === DEPRECATED

  // Deprecated: use shadeType instead
  SurfaceMesh* setSmoothShade(bool isSmooth);
  bool isSmoothShade();


private:
  // Visualization settings
  PersistentValue<glm::vec3> surfaceColor;
  PersistentValue<glm::vec3> edgeColor;
  PersistentValue<std::string> material;
  PersistentValue<float> edgeWidth;
  PersistentValue<BackFacePolicy> backFacePolicy;
  PersistentValue<glm::vec3> backFaceColor;
  PersistentValue<MeshShadeStyle> shadeStyle;

  // Do setup work related to drawing, including allocating openGL data
  void prepare();
  void preparePick();
  void geometryChanged(); // call whenever geometry changed

  // Picking-related
  // Order of indexing: vertexPositions, faces, edges, halfedges
  // Within each set, uses the implicit ordering from the mesh data structure
  // These starts are LOCAL indices, indexing elements only with the mesh
  size_t facePickIndStart, edgePickIndStart, halfedgePickIndStart;
  void buildVertexInfoGui(size_t vInd);
  void buildFaceInfoGui(size_t fInd);
  void buildEdgeInfoGui(size_t eInd);
  void buildHalfedgeInfoGui(size_t heInd);

  // ==== Gui implementation details

  // == Buffers for geometry quantities

  void ensureVertexIndexRenderBufferFilled();
  void updateVertexIndexRenderBufferIfAllocated();
  std::shared_ptr<render::AttributeBuffer> vertexIndicesRenderBuffer;

  void ensureVertexPositionsRenderBufferFilled();
  void updateVertexPositionsRenderBufferIfAllocated();
  std::shared_ptr<render::AttributeBuffer> vertexPositionsRenderBuffer;

  void ensureFaceNormalsRenderBufferFilled();
  void updateFaceNormalsRenderBufferIfAllocated();
  std::shared_ptr<render::AttributeBuffer> faceNormalsRenderBuffer;

  void ensureFaceCentersRenderBufferFilled();
  void updateFaceCentersRenderBufferIfAllocated();
  std::shared_ptr<render::AttributeBuffer> faceCentersRenderBuffer;

  void ensureFaceAreasRenderBufferFilled();
  void updateFaceAreasRenderBufferIfAllocated();
  std::shared_ptr<render::AttributeBuffer> faceAreasRenderBuffer;

  void ensureVertexNormalsRenderBufferFilled();
  void updateVertexNormalsRenderBufferIfAllocated();
  std::shared_ptr<render::AttributeBuffer> vertexNormalsRenderBuffer;

  void ensureVertexAreasRenderBufferFilled();
  void updateVertexAreasRenderBufferIfAllocated();
  std::shared_ptr<render::AttributeBuffer> vertexAreasRenderBuffer;

  void ensureEdgeLengthsRenderBufferFilled();
  void updateEdgeLengthsRenderBufferIfAllocated();
  std::shared_ptr<render::AttributeBuffer> edgeLengthsRenderBuffer;

  void ensureFaceTangentSpacesRenderBufferFilled();
  void updateFaceTangentSpacesRenderBufferIfAllocated();
  std::shared_ptr<render::AttributeBuffer> faceTangentSpacesRenderBuffer;

  void ensureVertexTangentSpacesRenderBufferFilled();
  void updateVertexTangentSpacesRenderBufferIfAllocated();
  std::shared_ptr<render::AttributeBuffer> vertexTangentSpacesRenderBuffer;

  std::shared_ptr<render::ShaderProgram> program;
  std::shared_ptr<render::ShaderProgram> pickProgram;


  // === Helper functions

  // Initialization work
  void initializeMeshTriangulation();

  void fillGeometryBuffersSmooth(render::ShaderProgram& p);
  void fillGeometryBuffersFlat(render::ShaderProgram& p);
  glm::vec2 projectToScreenSpace(glm::vec3 coord);
  // bool screenSpaceTriangleTest(size_t fInd, glm::vec2 testCoords, glm::vec3& bCoordOut);


  // clang-format off

  // === Quantity adders

  SurfaceVertexColorQuantity* addVertexColorQuantityImpl(std::string name, const std::vector<glm::vec3>& colors);
  SurfaceFaceColorQuantity* addFaceColorQuantityImpl(std::string name, const std::vector<glm::vec3>& colors);
  SurfaceVertexScalarQuantity* addVertexScalarQuantityImpl(std::string name, const std::vector<double>& data, DataType type);
  SurfaceFaceScalarQuantity* addFaceScalarQuantityImpl(std::string name, const std::vector<double>& data, DataType type);
  SurfaceEdgeScalarQuantity* addEdgeScalarQuantityImpl(std::string name, const std::vector<double>& data, DataType type);
  SurfaceHalfedgeScalarQuantity* addHalfedgeScalarQuantityImpl(std::string name, const std::vector<double>& data, DataType type);
  SurfaceVertexScalarQuantity* addVertexDistanceQuantityImpl(std::string name, const std::vector<double>& data);
  SurfaceVertexScalarQuantity* addVertexSignedDistanceQuantityImpl(std::string name, const std::vector<double>& data);
  SurfaceCornerParameterizationQuantity* addParameterizationQuantityImpl(std::string name, const std::vector<glm::vec2>& coords, ParamCoordsType type);
  SurfaceVertexParameterizationQuantity* addVertexParameterizationQuantityImpl(std::string name, const std::vector<glm::vec2>& coords, ParamCoordsType type);
  SurfaceVertexParameterizationQuantity* addLocalParameterizationQuantityImpl(std::string name, const std::vector<glm::vec2>& coords, ParamCoordsType type);
  SurfaceVertexVectorQuantity* addVertexVectorQuantityImpl(std::string name, const std::vector<glm::vec3>& vectors, VectorType vectorType);
  SurfaceFaceVectorQuantity* addFaceVectorQuantityImpl(std::string name, const std::vector<glm::vec3>& vectors, VectorType vectorType);
  SurfaceFaceIntrinsicVectorQuantity* addFaceIntrinsicVectorQuantityImpl(std::string name, const std::vector<glm::vec2>& vectors, int nSym, VectorType vectorType);
  SurfaceVertexIntrinsicVectorQuantity* addVertexIntrinsicVectorQuantityImpl(std::string name, const std::vector<glm::vec2>& vectors, int nSym, VectorType vectorType);
  SurfaceOneFormIntrinsicVectorQuantity* addOneFormIntrinsicVectorQuantityImpl(std::string name, const std::vector<double>& data, const std::vector<char>& orientations);
  SurfaceVertexCountQuantity* addVertexCountQuantityImpl(std::string name, const std::vector<std::pair<size_t, int>>& values);
  SurfaceVertexIsolatedScalarQuantity* addVertexIsolatedScalarQuantityImpl(std::string name, const std::vector<std::pair<size_t, double>>& values);
  SurfaceFaceCountQuantity* addFaceCountQuantityImpl(std::string name, const std::vector<std::pair<size_t, int>>& values);
	SurfaceGraphQuantity* addSurfaceGraphQuantityImpl(std::string name, const std::vector<glm::vec3>& nodes, const std::vector<std::array<size_t, 2>>& edges);

  // === Helper implementations

  void setVertexTangentBasisXImpl(const std::vector<glm::vec3>& vectors);
  void setFaceTangentBasisXImpl(const std::vector<glm::vec3>& vectors);
  // clang-format on
};

// Register functions
template <class V, class F>
SurfaceMesh* registerSurfaceMesh(std::string name, const V& vertexPositions, const F& faceIndices);
template <class V, class F>
SurfaceMesh* registerSurfaceMesh2D(std::string name, const V& vertexPositions, const F& faceIndices);
template <class V, class F, class P>
SurfaceMesh* registerSurfaceMesh(std::string name, const V& vertexPositions, const F& faceIndices,
                                 const std::array<std::pair<P, size_t>, 5>& perms);


// Shorthand to get a mesh from polyscope
inline SurfaceMesh* getSurfaceMesh(std::string name = "");
inline bool hasSurfaceMesh(std::string name = "");
inline void removeSurfaceMesh(std::string name = "", bool errorIfAbsent = true);


} // namespace polyscope

#include "polyscope/surface_mesh.ipp"
