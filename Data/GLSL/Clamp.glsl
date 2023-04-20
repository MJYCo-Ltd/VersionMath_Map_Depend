#version $GLSL_VERSION_STR

#pragma vp_name       YTY_Clamp_glsl_VS
#pragma vp_entryPoint clamp_vs
#pragma vp_location   vertex_view

#pragma import_defines(YTY_CLAMP)

#ifdef YTY_CLAMP

in float reletiveHight;
vec3 oe_UpVectorView;

// SDK functions:
// void oe_rex_applyElevation(inout vec4 vertex);

void clamp_vs(inout vec4 vv) 
{
    // oe_rex_applyElevation(vv);
    vv.xyz += oe_UpVectorView*reletiveHight;
}
#else

void clamp_vs(inout vec4 vv){}

#endif