#version $GLSL_VERSION_STR

#pragma vp_name       YTY_Pulse_glsl_VS
#pragma vp_entryPoint pulse_vs
#pragma vp_location   vertex_model

out float fDistancZ;
void pulse_vs(inout vec4 vertex)
{
   fDistancZ = vertex.z;
}


[break]

#version $GLSL_VERSION_STR

#pragma vp_name YTY_Pulse_glsl_FS
#pragma vp_entryPoint pulse_fs
#pragma vp_location   fragment_coloring

in float fDistancZ;
uniform float  osg_FrameTime;
uniform float  fPulseStartTime;
uniform float  fPulseIntervalTime;
uniform bool   bIsOut;
uniform float  fPulseStep;
void pulse_fs(inout vec4 color)
{
   float fScale=1.0;
   if(bIsOut)
   {
      fScale *= -1.;
   }

   if(mod(fDistancZ+fScale*mod(osg_FrameTime-fPulseStartTime,fPulseIntervalTime)/fPulseIntervalTime*fPulseStep,fPulseStep)>fPulseStep/2.)
   {
      discard;
   }
}