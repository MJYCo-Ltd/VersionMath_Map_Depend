#version $GLSL_VERSION_STR

#pragma vp_name       YTY_Line_glsl_VS
#pragma vp_entryPoint Line_VS
#pragma vp_location vertex_model

in vec3 Line_prev_point;
in vec3 Line_next_point;

out vec4 prevModel;
out vec4 nextModel;

void Line_VS(inout vec4 vetex)
{
    prevModel = vec4(Line_prev_point,1);
    nextModel = vec4(Line_next_point,1);
}

[break]
#version $GLSL_VERSION_STR

#pragma vp_name    YTY_Line_glsl_GS
#pragma vp_entryPoint Line_GS
#pragma vp_location geometry
#pragma import_defines(LINE_GLOW)

void VP_LoadVertex(const in int);
void VP_EmitModelVertex();

uniform float LineWidth;
uniform int   LineStipplePattern;
uniform vec2  cameraSize;

vec4 prevModel; 
vec4 nextModel;  
vec4 vp_Vertex; 

#ifdef LINE_GLOW
out float fDistance;
#endif 

layout(lines)in;
layout(triangle_strip, max_vertices=4) out;

flat out vec2 LineDrawable_rv;

void Line_GS(void)
{
#ifndef LINE_GLOW
    float fDistance=LineWidth;
#endif
    for(int i=0; i<2; ++i )
    {
        VP_LoadVertex(i);
        vec4 prevClip = gl_ModelViewProjectionMatrix * prevModel;
        vec4 currClip = gl_ModelViewProjectionMatrix * vp_Vertex;
        vec4 nextClip = gl_ModelViewProjectionMatrix * nextModel;

        /// convert 2 screen pos
        vec2 prevPixel = ((prevClip.xy/prevClip.w)+1.0) * 0.5*cameraSize;
        vec2 currPixel = ((currClip.xy/currClip.w)+1.0) * 0.5*cameraSize;
        vec2 nextPixel = ((nextClip.xy/nextClip.w)+1.0) * 0.5*cameraSize;

        fDistance = max(0.5, floor(LineWidth));
        float len = fDistance;

        vec2 dir = vec2(0.0);
        bool isStart = 0 == i;
        vec2 stippleDir;
        /// if is start point kao lv yong hou mian yi ge dian de xie lv 
        if (vp_Vertex.xyz == prevModel.xyz)
        {
            dir = normalize(nextPixel - currPixel);
            isStart = true;
            stippleDir = dir;
        }/// if equal the end point 
        else if (vp_Vertex.xyz == nextModel.xyz)
        {
            dir = normalize(currPixel - prevPixel);
            stippleDir = dir;
        }
        else
        {
          vec2 dirIn  = normalize(currPixel - prevPixel);
          vec2 dirOut = normalize(nextPixel - currPixel);

          /// ru guo liang tiao xian cheng 180 du 
          if (dot(dirIn,dirOut) < -0.999999)
          {
              dir = isStart? dirOut : dirIn;
          }
          else
          {
              vec2 tangent = normalize(dirIn+dirOut);
              vec2 perp = vec2(-dirIn.y, dirIn.x);
              vec2 miter = vec2(-tangent.y, tangent.x);
              dir = tangent;
              len = fDistance / dot(miter, perp);

              /// fang zhi jiao du tai xiao zao cheng tai jian rui
              const float limit = 2.0;
              if (len > fDistance*limit)
              {
                  len = fDistance;
                  dir = isStart? dirOut : dirIn;
              }
          }
          stippleDir = dirOut;
        }
        
        // prepare for stippling:
        if (LineStipplePattern != 0xffff)
        {
            float way = sign(cross(vec3(1, 0, 0), vec3(-stippleDir, 0)).z);
            float angle = acos(dot(vec2(1, 0), -stippleDir)) * way;
    
            const float pi = 3.14159265359;
            const float q = pi/8.0;
            angle = floor(angle/q) * q;
    
            LineDrawable_rv = vec2(cos(angle), sin(angle));
        }
        /// ni shi zhen xuan zhuan 90 du
        vec2 extrudePixel = vec2(-dir.y, dir.x) * len;

        /// zhuan huan dao [-1,1] equ space
        vec2 extrudeUnit = extrudePixel / cameraSize;
        
        /// zhuan huan dao clip space
        vec2 offset = extrudeUnit * currClip.w;

        vec4 bakVertex = vp_Vertex;
        
        gl_Position = vp_Vertex;
        VP_EmitModelVertex();
        vp_Vertex.xy += offset;
        gl_Position = vp_Vertex;
        EmitVertex();
#ifdef LINE_GLOW
        fDistance *= -1;
#endif        
        gl_Position=bakVertex;
        VP_EmitModelVertex();
        vp_Vertex.xy -= offset;
        gl_Position = vp_Vertex;
        EmitVertex();
    }
    EndPrimitive();
}

[break]
#version $GLSL_VERSION_STR

#pragma vp_name    YTY_Line_glsl_FS
#pragma vp_entryPoint Line_FS
#pragma vp_location fragment_coloring
#pragma import_defines(LINE_GLOW)

uniform int LineStippleFactor;
uniform int LineStipplePattern;
flat in vec2 LineDrawable_rv;

#ifdef LINE_GLOW
uniform float LineWidth;
in float fDistance;

void Line_FS(inout vec4 color)
{
    if (LineStipplePattern != 0xffff)
    {
        vec2 coord = (gl_FragCoord.xy - 0.5);

        vec2 coordProj =
            mat2(LineDrawable_rv.x, -LineDrawable_rv.y,
                 LineDrawable_rv.y,  LineDrawable_rv.x)
            * coord;

        // sample the stippling pattern (16-bits repeating)
        int ci = int(mod(coordProj.x, 16.0 * float(LineStippleFactor))) / LineStippleFactor;
        int pattern16 = 0xffff & (LineStipplePattern & (1 << ci));
        if (pattern16 == 0)
        {
            discard; 
        }
    }

    color.a = pow((LineWidth - abs(fDistance))/LineWidth,2);
}
#else
void Line_FS(inout vec4 color)
{
    if (LineStipplePattern != 0xffff)
    {
        vec2 coord = (gl_FragCoord.xy - 0.5);

        vec2 coordProj =
            mat2(LineDrawable_rv.x, -LineDrawable_rv.y,
                 LineDrawable_rv.y,  LineDrawable_rv.x)
            * coord;

        // sample the stippling pattern (16-bits repeating)
        int ci = int(mod(coordProj.x, 16.0 * float(LineStippleFactor))) / LineStippleFactor;
        int pattern16 = 0xffff & (LineStipplePattern & (1 << ci));
        if (pattern16 == 0)
        {
            discard; 
        }
    }
}
#endif
