#version $GLSL_VERSION_STR

#pragma vp_name       YTY_Global_glsl_VS
#pragma vp_entryPoint setTextCoord_vs
#pragma vp_location   vertex_model

out vec2 texCoord;
void setTextCoord_vs(inout vec4 vertex)
{
    texCoord = gl_MultiTexCoord0.xy;
}


[break]
#version $GLSL_VERSION_STR

#pragma vp_name       YTY_Global_glsl_FS
#pragma vp_entryPoint mixTexture_fs
#pragma vp_location   fragment_coloring

in vec2 texCoord;
uniform sampler2D baseTexture;
void mixTexture_fs(inout vec4 color)
{
   color *= texture(baseTexture, texCoord);
}