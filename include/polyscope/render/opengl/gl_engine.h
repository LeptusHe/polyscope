// Copyright 2017-2019, Nicholas Sharp and the Polyscope contributors. http://polyscope.run.
#pragma once

#include "polyscope/options.h"
#include "polyscope/render/engine.h"
#include "polyscope/utilities.h"

#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include "GLFW/glfw3.h"
#else
#include "glad/glad.h"
// glad must come first
#include "GLFW/glfw3.h"
#endif

#ifdef _WIN32
#undef APIENTRY
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <GLFW/glfw3native.h>
#endif

#include "imgui.h"
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <unordered_map>

// Note: DO NOT include this header throughout polyscope, and do not directly make openGL calls. This header should only
// be used to construct an instance of Engine. engine.h gives the render API, all render calls should pass through that.


namespace polyscope {
namespace render {
namespace backend_openGL3_glfw {

// Some very nice typdefs
typedef GLuint TextureHandle;
typedef GLuint RenderBufferHandle;
typedef GLuint FrameBufferHandle;
typedef GLuint ShaderHandle;
typedef GLuint ProgramHandle;
typedef GLuint AttributeHandle;
typedef GLuint VertexBufferHandle;

typedef GLint UniformLocation;
typedef GLint AttributeLocation;
typedef GLint TextureLocation;

class GLAttributeBuffer : public AttributeBuffer {
public:
  GLAttributeBuffer(RenderDataType dataType_, int arrayCount_ = 1);
  virtual ~GLAttributeBuffer();

  void bind();
  VertexBufferHandle getHandle() const { return VBOLoc; }

  void setData(const std::vector<glm::vec2>& data) override;
  void setData(const std::vector<glm::vec3>& data) override;
  void setData(const std::vector<glm::vec4>& data) override;
  void setData(const std::vector<float>& data) override;
  void setData(const std::vector<double>& data) override;
  void setData(const std::vector<int32_t>& data) override;
  void setData(const std::vector<uint32_t>& data) override;

  float getData_float(size_t ind) override;
  double getData_double(size_t ind) override;
  glm::vec2 getData_vec2(size_t ind) override;
  glm::vec3 getData_vec3(size_t ind) override;
  glm::vec4 getData_vec4(size_t ind) override;
  int getData_int(size_t ind) override;
  uint32_t getData_uint32(size_t ind) override;

  uint32_t getNativeBufferID() override;

protected:
  VertexBufferHandle VBOLoc;

private:
  void checkType(RenderDataType targetType);
};

class GLTexture : public Texture {
public:
  // create a 1D texture from data
  GLTexture(TextureFormat format, unsigned int size1D, unsigned char* data = nullptr);
  GLTexture(TextureFormat format, unsigned int size1D, float* data);

  // create a 2D texture from data
  GLTexture(TextureFormat format, unsigned int sizeX_, unsigned int sizeY_, unsigned char* data = nullptr);
  GLTexture(TextureFormat format, unsigned int sizeX_, unsigned int sizeY_, float* data);

  ~GLTexture() override;


  // Resize the underlying buffer (contents are lost)
  void resize(unsigned int newLen) override;
  void resize(unsigned int newX, unsigned int newY) override;

  void setFilterMode(FilterMode newMode) override;
  void* getNativeHandle() override;

  std::vector<float> getDataScalar() override;
  std::vector<glm::vec2> getDataVector2() override;
  std::vector<glm::vec3> getDataVector3() override;

  void bind();
  GLenum textureType();
  TextureHandle getHandle() const { return handle; }


protected:
  TextureHandle handle;
};

class GLRenderBuffer : public RenderBuffer {
public:
  GLRenderBuffer(RenderBufferType type, unsigned int sizeX_, unsigned int sizeY_);
  ~GLRenderBuffer() override;

  void resize(unsigned int newX, unsigned int newY) override;

  void bind();
  RenderBufferHandle getHandle() const { return handle; }

  RenderBufferHandle handle;
};


class GLFrameBuffer : public FrameBuffer {

public:
  GLFrameBuffer(unsigned int sizeX_, unsigned int sizeY_, bool isDefault = false);
  ~GLFrameBuffer() override;

  void bind() override;

  // Bind to this framebuffer so subsequent draw calls will go to it
  // If return is false, binding failed and the framebuffer should not be used.
  bool bindForRendering() override;

  // Clear to redraw
  void clear() override;

  // Bind to textures/renderbuffers for output
  void addColorBuffer(std::shared_ptr<RenderBuffer> renderBuffer) override;
  void addColorBuffer(std::shared_ptr<Texture> textureBuffer) override;
  void addDepthBuffer(std::shared_ptr<RenderBuffer> renderBuffer) override;
  void addDepthBuffer(std::shared_ptr<Texture> textureBuffer) override;

  void setDrawBuffers() override;

  // Query pixels
  std::vector<unsigned char> readBuffer() override;
  std::array<float, 4> readFloat4(int xPos, int yPos) override;
  void blitTo(FrameBuffer* other) override;

  // Getters
  FrameBufferHandle getHandle() const { return handle; }

  FrameBufferHandle handle;
};


class GLShaderProgram : public ShaderProgram {

public:
  GLShaderProgram(const std::vector<ShaderStageSpecification>& stages, DrawMode dm);
  ~GLShaderProgram() override;

  // === Store data
  // If update is set to "true", data is updated rather than allocated (must be allocated first)

  // Uniforms
  bool hasUniform(std::string name) override;
  void setUniform(std::string name, int val) override;
  void setUniform(std::string name, unsigned int val) override;
  void setUniform(std::string name, float val) override;
  void setUniform(std::string name, double val) override; // WARNING casts down to float
  void setUniform(std::string name, float* val) override;
  void setUniform(std::string name, glm::vec2 val) override;
  void setUniform(std::string name, glm::vec3 val) override;
  void setUniform(std::string name, glm::vec4 val) override;
  void setUniform(std::string name, std::array<float, 3> val) override;
  void setUniform(std::string name, float x, float y, float z, float w) override;

  // = Attributes
  // clang-format off
  bool hasAttribute(std::string name) override;
  void setExternalBuffer(std::string name, std::shared_ptr<AttributeBuffer> externalBuffer) override; 
  bool attributeIsSet(std::string name) override;
  std::shared_ptr<AttributeBuffer> getAttributeBuffer(std::string name) override;
  void setAttribute(std::string name, const std::vector<glm::vec2>& data) override;
  void setAttribute(std::string name, const std::vector<glm::vec3>& data) override;
  void setAttribute(std::string name, const std::vector<glm::vec4>& data) override;
  void setAttribute(std::string name, const std::vector<float>& data) override;
  void setAttribute(std::string name, const std::vector<double>& data) override;
  void setAttribute(std::string name, const std::vector<int32_t>& data) override; 
  void setAttribute(std::string name, const std::vector<uint32_t>& data) override;
  // clang-format on

  // Convenience method to set an array-valued attrbute, such as 'in vec3 vertexVal[3]'. Applies interleaving then
  // forwards to the usual setAttribute
  template <typename T, unsigned int C>
  void setAttribute(std::string name, const std::vector<std::array<T, C>>& data, bool update = false, int offset = 0,
                    int size = -1);


  // Indices
  void setIndex(std::vector<std::array<unsigned int, 3>>& indices) override;
  void setIndex(std::vector<unsigned int>& indices) override;
  void setPrimitiveRestartIndex(unsigned int restartIndex) override;

  // Textures
  bool hasTexture(std::string name) override;
  bool textureIsSet(std::string name) override;
  void setTexture1D(std::string name, unsigned char* texData, unsigned int length) override;
  void setTexture2D(std::string name, unsigned char* texData, unsigned int width, unsigned int height,
                    bool withAlpha = true, bool useMipMap = false, bool repeat = false) override;
  void setTextureFromColormap(std::string name, const std::string& colorMap, bool allowUpdate = false) override;
  void setTextureFromBuffer(std::string name, Texture* textureBuffer) override;

  // Draw!
  void draw() override;
  void validateData() override;

protected:
  // Classes to keep track of attributes and uniforms
  struct GLShaderUniform {
    std::string name;
    RenderDataType type;
    bool isSet;               // has a value been assigned to this uniform?
    UniformLocation location; // -1 means "no location", usually because it was optimized out
  };

  struct GLShaderAttribute {
    std::string name;
    RenderDataType type;
    int arrayCount;
    AttributeLocation location;              // -1 means "no location", usually because it was optimized out
    std::shared_ptr<GLAttributeBuffer> buff; // the buffer that we will actually use
  };

  struct GLShaderTexture {
    std::string name;
    int dim;
    unsigned int index;
    bool isSet;
    GLTexture* textureBuffer;
    std::shared_ptr<GLTexture> textureBufferOwned; // might be empty, if texture isn't owned
    TextureLocation location;                            // -1 means "no location", usually because it was optimized out
  };

  // Lists of attributes and uniforms that need to be set
  std::vector<GLShaderUniform> uniforms;
  std::vector<GLShaderAttribute> attributes;
  std::vector<GLShaderTexture> textures;

  void addUniqueAttribute(ShaderSpecAttribute attribute);
  void addUniqueUniform(ShaderSpecUniform uniform);
  void addUniqueTexture(ShaderSpecTexture texture);

private:
  // Setup routines
  void compileGLProgram(const std::vector<ShaderStageSpecification>& stages);
  void setDataLocations();
  void bindVAO();
  void createBuffers();
  void ensureBufferExists(GLShaderAttribute& a);
  void createBuffer(GLShaderAttribute& a);
  void assignBufferToVAO(GLShaderAttribute& a);

  void deleteAttributeBuffer(GLShaderAttribute& attribute);

  // Drawing related
  void activateTextures();

  // GL pointers for various useful things
  ProgramHandle programHandle = 0;
  AttributeHandle vaoHandle;
  AttributeHandle indexVBO;
};


class GLEngine : public Engine {
public:
  GLEngine();

  // High-level control
  void initialize();
  void checkError(bool fatal = false) override;

  void swapDisplayBuffers() override;
  std::vector<unsigned char> readDisplayBuffer() override;
  // void blitFinalSceneToScreen() override;

  // Manage render state
  void setDepthMode(DepthMode newMode = DepthMode::Less) override;
  void setBlendMode(BlendMode newMode = BlendMode::Over) override;
  void setColorMask(std::array<bool, 4> mask = {true, true, true, true}) override;
  void setBackfaceCull(bool newVal) override;

  // === Windowing and framework things
  void makeContextCurrent() override;
  void focusWindow() override;
  void showWindow() override;
  void hideWindow() override;
  void updateWindowSize(bool force = false) override;
  std::tuple<int, int> getWindowPos() override;
  bool windowRequestsClose() override;
  void pollEvents() override;
  bool isKeyPressed(char c) override; // for lowercase a-z and 0-9 only
  std::string getClipboardText() override;
  void setClipboardText(std::string text) override;

  // ImGui
  void initializeImGui() override;
  void shutdownImGui() override;
  void ImGuiNewFrame() override;
  void ImGuiRender() override;

  // === Factory methods

  // create attribute buffers
  std::shared_ptr<AttributeBuffer> generateAttributeBuffer(RenderDataType dataType_, int arrayCount_ = 1) override;

  // create textures
  std::shared_ptr<Texture> generateTexture(TextureFormat format, unsigned int size1D,
                                                       unsigned char* data = nullptr) override; // 1d
  std::shared_ptr<Texture> generateTexture(TextureFormat format, unsigned int size1D,
                                                       float* data) override; // 1d
  std::shared_ptr<Texture> generateTexture(TextureFormat format, unsigned int sizeX_, unsigned int sizeY_,
                                                       unsigned char* data = nullptr) override; // 2d
  std::shared_ptr<Texture> generateTexture(TextureFormat format, unsigned int sizeX_, unsigned int sizeY_,
                                                       float* data) override; // 2d

  // create render buffers
  std::shared_ptr<RenderBuffer> generateRenderBuffer(RenderBufferType type, unsigned int sizeX_,
                                                     unsigned int sizeY_) override;
  // create frame buffers
  std::shared_ptr<FrameBuffer> generateFrameBuffer(unsigned int sizeX_, unsigned int sizeY_) override;

  // general flexible interface
  std::shared_ptr<ShaderProgram>
  requestShader(const std::string& programName, const std::vector<std::string>& customRules,
                ShaderReplacementDefaults defaults = ShaderReplacementDefaults::SceneObject) override;

  // === Implementation details

  // Add a shader programs/rules so that they can be requested above
  void registerShaderProgram(const std::string& name, const std::vector<ShaderStageSpecification>& stages);
  void registerShaderRule(const std::string& name, const ShaderReplacementRule& rule);

  // Transparency
  virtual void applyTransparencySettings() override;

  virtual void setFrontFaceCCW(bool newVal) override;

protected:
  // Helpers
  virtual void createSlicePlaneFliterRule(std::string name) override;

  // Internal windowing and engine details
  GLFWwindow* mainWindow = nullptr;

  // Shader program & rule caches
  // TODO oneday make these caches of precompiled programs which are shared, rather than caches of the sources
  std::unordered_map<std::string, std::pair<std::vector<ShaderStageSpecification>, DrawMode>> registeredShaderPrograms;
  std::unordered_map<std::string, ShaderReplacementRule> registeredShaderRules;
  void populateDefaultShadersAndRules();

  std::shared_ptr<ShaderProgram> generateShaderProgram(const std::vector<ShaderStageSpecification>& stages,
                                                       DrawMode dm) override;
};

} // namespace backend_openGL3_glfw
} // namespace render
} // namespace polyscope
