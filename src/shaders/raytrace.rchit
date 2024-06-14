#version 460
#extension GL_EXT_ray_tracing : require
#extension GL_GOOGLE_include_directive : enable
#extension GL_EXT_shader_explicit_arithmetic_types_int64 : require

#include "shared_structs.h"

layout(location=0) rayPayloadInEXT RayPayload payload;

hitAttributeEXT vec2 bc;  // Hit point's barycentric coordinates (two of them)

void main()
{
    // @@ Raycasting: Set payload.hit = true, and fill in the
    // remaining payload values with information (provided by Vulkan)
    // about the hit point.
}
