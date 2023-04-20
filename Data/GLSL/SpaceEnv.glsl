#version $GLSL_VERSION_STR

#pragma vp_name       YTY_SpaceEnv_glsl_VS
#pragma vp_entryPoint space_vs
#pragma vp_location   vertex_view
#pragma vp_order      first

// Atmospheric Scattering and Sun Shaders
// Adapted from code that is Copyright (c) 2004 Sean ONeil

uniform mat4 osg_ViewMatrixInverse;   // camera position in [3].xyz
uniform vec3 atmos_v3LightDir;        // The direction vector to the light source 
uniform float atmos_fOuterRadius;     // Outer atmosphere radius 		

out vec3 atmos_v3Direction; 
out float atmos_renderFromSpace;

vec3 vVec; 
float atmos_fCameraHeight;    // The camera's current height 		

void space_vs(inout vec4 VertexVIEW) 
{
    // Get camera position and height 
    vVec = osg_ViewMatrixInverse[3].xyz; 
    atmos_fCameraHeight = length(vVec); 
    atmos_v3Direction = vVec  - gl_Vertex.xyz; 

    // Transition from space to atmosphere
    if(atmos_fCameraHeight > atmos_fOuterRadius)
    {
        atmos_renderFromSpace=1.0;
    }
    else
    {
        atmos_renderFromSpace = 1.0 - clamp(
            (atmos_fOuterRadius-atmos_fCameraHeight)/50000.0,
            0.0, 1.0 );
    }
}

[break]
#version $GLSL_VERSION_STR

#pragma vp_name       YTY_SpaceEnv_glsl_FS
#pragma vp_entryPoint space_fs
#pragma vp_location   fragment_coloring
#pragma vp_order      last

uniform vec3 atmos_v3LightDir; 

in vec3 atmos_v3Direction; 	 
in float atmos_renderFromSpace;

void space_fs(inout vec4 color) 
{
    float fCos = dot(atmos_v3LightDir, atmos_v3Direction); 
    if(fCos>0)
    {
        if(atmos_renderFromSpace<0.01)
        {
            color=vec4(vec3(0.),1.);
        }
    }
}