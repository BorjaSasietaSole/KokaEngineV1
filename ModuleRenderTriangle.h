#ifndef __ModuleRenderTriangle_h__
#define __ModuleRenderTriangle_h__

#include "Module.h"

#include "MathGeoLib.h"
#include "Math/float4x4.h"
#include "Math/float3.h"


class ModuleRenderTriangle : public Module
{
public:
    ModuleRenderTriangle();
    ~ModuleRenderTriangle();

	bool Init();
	update_status Update();
	bool CleanUp();

private:
	unsigned vbo = 0;
	unsigned texture0 = 0;

	math::float4x4 tri_model = math::float4x4::identity;
	math::float4x4 proj = math::float4x4::identity;
	math::float4x4 view = math::float4x4::identity;

	void ModuleRenderTriangle::LookAt(math::float4x4&  matrix, const math::float3& eye, const math::float3& target, const math::float3& up);
};

#endif /* __ModuleRenderTriangle_h__ */
