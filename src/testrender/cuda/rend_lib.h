// Copyright Contributors to the Open Shading Language project.
// SPDX-License-Identifier: BSD-3-Clause
// https://github.com/imageworks/OpenShadingLanguage

#pragma once

#if (OPTIX_VERSION < 70000)
#include <optix_math.h>
#endif
#include <OSL/device_string.h>

OSL_NAMESPACE_ENTER
// Create an OptiX variable for each of the 'standard' strings declared in
// <OSL/strdecls.h>.
namespace DeviceStrings {
#if (OPTIX_VERSION < 70000)
#define STRDECL(str,var_name)                           \
    rtDeclareVariable(OSL_NAMESPACE::DeviceString, var_name, , );
#else
#define STRDECL(str,var_name)                           \
    extern __device__ OSL_NAMESPACE::DeviceString var_name;
#endif

#include <OSL/strdecls.h>
#undef STRDECL
}
OSL_NAMESPACE_EXIT

namespace {  // anonymous namespace

#if (OPTIX_VERSION < 70000)
#ifdef __cplusplus
    typedef optix::float3 float3;
#endif
#endif

// These are CUDA variants of various OSL options structs. Their layouts and
// default values are identical to the host versions, but they might differ in
// how they are constructed. They are duplicated here as a convenience and to
// avoid including additional host headers.

struct NoiseOptCUDA {
    int    anisotropic;
    int    do_filter;
    float3 direction;
    float  bandwidth;
    float  impulses;

    __device__
    NoiseOptCUDA ()
        : anisotropic (0),
          do_filter   (true),
          direction   (make_float3(1.0f,0.0f,0.0f)),
          bandwidth   (1.0f),
          impulses    (16.0f)
    {
    }
};


struct TextureOptCUDA {
    // TO BE IMPLEMENTED
};


struct TraceOptCUDA {
    // TO BE IMPLEMENTED
};


// This isn't really a CUDA version of the host-side ShadingContext class;
// instead, it is used as a container for a handful of pointers accessed during
// shader executions that are accessed via the ShadingContext.
struct ShadingContextCUDA {
    NoiseOptCUDA*   m_noiseopt;
    TextureOptCUDA* m_textureopt;
    TraceOptCUDA*   m_traceopt;

    __device__ void* noise_options_ptr ()   { return m_noiseopt; }
    __device__ void* texture_options_ptr () { return m_textureopt; }
    __device__ void* trace_options_ptr ()   { return m_traceopt; }
};


struct ShaderGlobals {
    float3 P, dPdx, dPdy;
    float3 dPdz;
    float3 I, dIdx, dIdy;
    float3 N;
    float3 Ng;
    float  u, dudx, dudy;
    float  v, dvdx, dvdy;
    float3 dPdu, dPdv;
    float  time;
    float  dtime;
    float3 dPdtime;
    float3 Ps, dPsdx, dPsdy;
    void*  renderstate;
    void*  tracedata;
    void*  objdata;
    void*  context;
    void*  renderer;
    void*  object2common;
    void*  shader2common;
    void*  Ci;
    float  surfacearea;
    int    raytype;
    int    flipHandedness;
    int    backfacing;
    int    shaderID;
};


enum RayType {
    CAMERA       = 1,
    SHADOW       = 2,
    REFLECTION   = 4,
    REFRACTION   = 8,
    DIFFUSE      = 16,
    GLOSSY       = 32,
    SUBSURFACE   = 64,
    DISPLACEMENT = 128
};


// Closures supported by the OSL sample renderer.  This list is mosly aspirational.
enum ClosureIDs {
    EMISSION_ID = 1,
    BACKGROUND_ID,
    DIFFUSE_ID,
    OREN_NAYAR_ID,
    TRANSLUCENT_ID,
    PHONG_ID,
    WARD_ID,
    MICROFACET_ID,
    REFLECTION_ID,
    FRESNEL_REFLECTION_ID,
    REFRACTION_ID,
    TRANSPARENT_ID,
    DEBUG_ID,
    HOLDOUT_ID,
};

#if (OPTIX_VERSION >= 70000)
// ========================================
//
// Some helper vector functions
//
static __forceinline__ __device__ float3 operator*(const float a, const float3& b)
{
      return make_float3(a * b.x, a * b.y, a * b.z);
}

static __forceinline__ __device__ float3 operator*(const float3 & a, const float b)
{
      return make_float3(a.x * b, a.y * b, a.z * b);
}

static __forceinline__ __device__ float3 operator+(const float3& a, const float3& b)
{
      return make_float3(a.x + b.x, a.y + b.y, a.z + b.z);
}

static __forceinline__ __device__ float3 operator-(const float3& a, const float3& b)
{
      return make_float3(a.x - b.x, a.y - b.y, a.z - b.z);
}

static __forceinline__ __device__ float3 operator-(const float3& a)
{
      return make_float3(-a.x, -a.y, -a.z);
}

static __forceinline__ __device__ float dot(const float3& a, const float3& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

static __forceinline__ __device__ float3 normalize(const float3& v)
{
    float invLen = 1.0f / sqrtf(dot(v, v));
    return invLen * v;
}
//
// ========================================
#endif //#if (OPTIX_VERSION >= 70000)

}  // anonymous namespace
