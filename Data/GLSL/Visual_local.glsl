#version $GLSL_VERSION_STR

#pragma vp_name       YTY_visual_glsl_VS
#pragma vp_entryPoint visual_vs
#pragma vp_location   vertex_model

uniform mat4 textureProjMatrix;
uniform mat4 textureViewMatrix;

out vec4 fragCoord;
out vec4 viewPos;
void visual_vs(inout vec4 vertex)
{
   viewPos = textureViewMatrix * vertex;
   fragCoord = textureProjMatrix * viewPos;
}


[break]

#version $GLSL_VERSION_STR

#pragma vp_name YTY_visual_glsl_FS
#pragma vp_entryPoint visual_fs
#pragma vp_location   fragment_output
#pragma import_defines(VISUAL_CAMERA)

uniform vec4 visibleColor;
uniform vec4 hiddenColor;
uniform sampler2D depthTexture;
uniform float fFar;
uniform float fNear;

in vec4 fragCoord;
in vec4 viewPos;

out vec4 fragColor;
void visual_fs(inout vec4 color)
{
#ifdef VISUAL_CAMERA
   float fDiff = fFar - fNear; 
   gl_FragDepth = (-viewPos.z/viewPos.w - fNear)/fDiff; 
   fragColor = vec4(vec3(
      gl_FragDepth
   ),1);
   return;
#endif
   fragColor = color;
   float fDistance = -viewPos.z/viewPos.w;
   if(fDistance<0 || fDistance<fNear || fDistance>fFar)
   {
      return;
   }

   if (fragCoord.x<-fragCoord.w|| fragCoord.x>fragCoord.w
	  || fragCoord.y<-fragCoord.w || fragCoord.y>fragCoord.w
	  || fragCoord.z<-fragCoord.w || fragCoord.z>fragCoord.w)
   {
         return;
   }

   float fDepth = (fDistance - fNear)/(fFar - fNear);
   

   // qu zhi fan wei tong yi dao [-1~1]
   fragCoord /= fragCoord.w;
   // zhuan dao [0~1] li
   fragCoord.xy = fragCoord.xy * 0.5 + 0.5;

   float depth = texture(depthTexture, fragCoord.xy).r + 1e-4;
   if(depth > fDepth)
   {
      fragColor.rgb  = color.rgb * visibleColor.rgb;
   }
   else
   {
      fragColor.rgb  = color.rgb  * hiddenColor.rgb;
   }
}