#version $GLSL_VERSION_STR

#pragma vp_name       YTY_Start_glsl_VS
#pragma vp_entryPoint start_vs
#pragma vp_location   vertex_model

uniform float lnfovFactor;
uniform float lnInputScale;
uniform float lnOneOverMaxdL;
uniform float fLinearScale;
uniform float alphaWaOverAlphaDa;
uniform float lnTerm2;

out float magnitude;
out float vertSize;
out float vertAlpha;

float pointSourceMagToLnLuminance(float mag)
{
  return -0.92103 *(mag + 12.12331) + lnfovFactor;
}

float adaptLuminanceScaledLn(float lnWorldLuminance, float pFact)
{
  return exp(((lnInputScale + lnWorldLuminance - 8.0656104861) * alphaWaOverAlphaDa + lnTerm2 + lnOneOverMaxdL) * pFact);
}

float remap( float val, float vmin, float vmax, float r0, float r1 )
{
   float vr = (clamp(val, vmin, vmax)-vmin)/(vmax-vmin);
	return r0 + vr * (r1-r0);
}

void start_vs(inout vec4 vertex)
{
   magnitude = gl_Color.a * 1000.0;
   float lnlum = pointSourceMagToLnLuminance(magnitude);
   vertSize = adaptLuminanceScaledLn(lnlum, 0.35) * fLinearScale * 0.24;
   if (vertSize < 1.0)
	{
       vertAlpha = vertSize * vertSize;
	    vertSize = 1.0;
	}
	else
   {
      vertAlpha = 1.0;
		if(vertSize > 8.0)
		{
         vertSize = 8.0 + sqrt(1.0 + vertSize - 8.0) - 1.0;
		}
	}

   if (vertSize < 6.0)
   {
      gl_PointSize = vertSize * 3.5; 
   }
	else
   {
      gl_PointSize = vertSize * 6.0;
   }
}

[break]
#version $GLSL_VERSION_STR

#pragma vp_name YTY_Start_glsl_FS
#pragma vp_entryPoint start_fs
#pragma vp_location fragment_coloring

in float magnitude;
in float vertSize;
in float vertAlpha;

uniform float maxMag;
uniform sampler2D baseTexture;
uniform sampler2D starTexture;

void start_fs(inout vec4 color)
{
   if ((maxMag > 0.0) && (magnitude > maxMag))
      discard;
   color.a = vertAlpha;

	if (vertSize < 6.0)
   {
      color = color * texture(baseTexture, gl_PointCoord);
   }
	else
   {
	   color = color * texture(starTexture, gl_PointCoord);
   }
}