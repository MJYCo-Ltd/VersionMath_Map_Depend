#version $GLSL_VERSION_STR

#pragma vp_name       YTY_Flash_glsl_FS
#pragma vp_entryPoint flash_fs
#pragma vp_location   fragment_coloring
#pragma vp_order last

uniform float  osg_FrameTime;
uniform float  flashStartTime;
uniform float  flashDurTime;
uniform float  flashIntervalTime;
uniform vec4   flashColor;

void flash_fs(inout vec4 color)
{
   if(flashIntervalTime<0.)
   {
      color.rgb *= flashColor.rgb;
   }
   else if(mod(osg_FrameTime-flashStartTime,flashIntervalTime) > flashDurTime) 
   {
      color.rgb *= flashColor.rgb;
   }
}