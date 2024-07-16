
#ifndef COMMON_HOST_DEVICE
#define COMMON_HOST_DEVICE

#ifdef __cplusplus
#include <glm/glm.hpp>
// GLSL Type
using vec2 = glm::vec2;
using vec3 = glm::vec3;
using vec4 = glm::vec4;
using mat4 = glm::mat4;
using uint = unsigned int;
#endif

// clang-format off
#ifdef __cplusplus // Descriptor binding helper for C++ and GLSL
 #define START_ENUM(a) enum a {
 #define END_ENUM() }
#else
 #define START_ENUM(a)  const uint
 #define END_ENUM() 
#endif

START_ENUM(ScBindings)
  eMatrices  = 0,  // Global uniform containing camera matrices
  eObjDescs = 1,  // Access to the object descriptions
  eTextures = 2   // Access to textures
END_ENUM();

START_ENUM(RtBindings)
  eTlas     = 0,  // Top-level acceleration structure
  eOutImage = 1,   // Ray tracer output image
  eColorHistoryImage = 2
END_ENUM();
// clang-format on



// Information of a obj model when referenced in a shader
struct ObjDesc
{
  int      txtOffset;             // Texture index offset in the array of textures
  uint64_t vertexAddress;         // Address of the Vertex buffer
  uint64_t indexAddress;          // Address of the index buffer
  uint64_t materialAddress;       // Address of the material buffer
  uint64_t materialIndexAddress;  // Address of the triangle material index buffer
};


// Uniform buffer set at each frame
struct MatrixUniforms
{
  mat4 viewProj;     // Camera view * projection
  mat4 priorViewProj;     // Camera view * projection
  mat4 viewInverse;  // Camera inverse view matrix
  mat4 projInverse;  // Camera inverse projection matrix
};

// Push constant structure for the raster
struct PushConstantRaster
{
    mat4  modelMatrix;  // matrix of the instance
    vec3  lightPosition;
    float lightIntensity;
    uint  objIndex;     // index of instance
};


#ifdef __cplusplus
#define ALIGNAS(N) alignas(N)
#else
#define ALIGNAS(N)
#endif

// For structures used by both C++ and GLSL, byte alignment
// differs between the two languages.  Ints, uints, and floats align
// nicely, but bool's do not.
//  Use ALIGNAS(4) to align bools.
//  Use ALIGNAS(4) or not for ints, uints, and floats as they naturally align to 4 byte boundaries.
//  Use ALIGNAS(16) for vec3 and vec4, and probably mat4 (untested).
//
// @@ Do a sanity check by creating a sentinel value, say
// "alignmentTest" as the last variable in the PushConstantRay
// structure.  In the C++ program, set this to a know value.  In
// raytrace.gen, test for that value and signal failure if not found.
// For example:
// if (pcRay.alignmentTest != 1234) {
//    imageStore(colCurr,  ivec2(gl_LaunchIDEXT.xy), vec4(1,0,0,0)); return; }

// Also, If your alignment is incorrect, the VULKAN validation layer
// may now produce an error message.



// Push constant structure for the ray tracer
struct PushConstantRay
{
    // @@ Raycasting: Declare 3 temporary light values.  
    // ALIGNAS(16) vec4 tempLightPos;  // TEMPORARY: vec4(0.5f, 2.5f, 3.0f, 0.0);
    // ALIGNAS(16) vec4 tempLightInt;  // TEMPORARY: vec4(2.5, 2.5, 2.5, 0.0);
    // ALIGNAS(16) vec4 tempAmbient;   // TEMPORARY: vec4(0.2);
    // @@ Pathtracing: Remove these 3 values because path tracing finds light by tracing rays.
    ALIGNAS(4) int frameSeed;
    ALIGNAS(4) float rr;        // Russian-Roulette Threshold
    ALIGNAS(4) int depth;       // Maximum Depth based on rr value
    ALIGNAS(4) bool explicitLight;

    ALIGNAS(4) bool clear;  // Tell the ray generation shader to start accumulation from scratch
    ALIGNAS(4) float exposure;
    // @@ Set alignmentTest to a known value in C++;  Test for that value in the shader!
    ALIGNAS(4) int alignmentTest;
};

struct Vertex  // Created by readModel; used in shaders
{
  vec3 pos;
  vec3 nrm;
  vec2 texCoord;
};

struct Material  // Created by readModel; used in shaders
{
  vec3  diffuse;
  vec3  specular;
  vec3  emission;
  float shininess;
  int   textureId;
};

struct Emitter
{
    vec3 point;         // Will use in raytrace.rgen (SampleLight)

    vec3 v0, v1, v2;    // Vertices of light emitting triangle
    vec3 emission;      // Its emission
    vec3 normal;        // Its normal
    float area;         // Its traingle area
    uint index;         // The triangle index in the model's list of triangles
};


// Push constant structure for the ray tracer
struct PushConstantDenoise
{
    float normFactor, depthFactor;
    int  stepwidth;  
};

struct RayPayload
{
    uint seed;		// Used in Path Tracing step as random number seed
    bool hit;           // Does the ray intersect anything or not?
    float hitDist;      // Used in the denoising step
    vec3 hitPos;	// The world coordinates of the hit point.      
    int instanceIndex;  // Index of the object instance hit (we have only one, so =0)
    int primitiveIndex; // Index of the hit triangle primitive within object
    vec3 bc;            // Barycentric coordinates of the hit point within triangle
};

#endif
