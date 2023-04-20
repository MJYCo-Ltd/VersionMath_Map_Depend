#version $GLSL_VERSION_STR

#pragma vp_name       YTY_Point_glsl_VS
#pragma vp_entryPoint point_vs
#pragma vp_location   vertex_model


uniform float pointSize;
void point_vs(inout vec4 vertex)
{
   gl_PointSize = pointSize;
}

[break]
#version $GLSL_VERSION_STR

#pragma vp_name YTY_Point_glsl_FS
#pragma vp_entryPoint point_fs
#pragma vp_location fragment_coloring

uniform bool bSlected=false;
uniform sampler2D baseTexture;
void point_fs(inout vec4 color)
{
   color = color * texture(baseTexture, gl_PointCoord);
   vec2 c = 2.0*gl_PointCoord-1.0;

   if(bSlected && (abs(c.x)>0.9 || abs(c.y)>0.9))
   {
      color.a = 1.0;
   }
   else
   {
      float r = dot(c, c);
      float d = fwidth(r);
      color.a *= 1.0 - smoothstep(1.0-d, 1.0+d, r);
   }
   
   
   if(color.a < 0.1)
      discard;
}