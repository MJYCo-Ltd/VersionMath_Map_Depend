#version $GLSL_VERSION_STR

#pragma vp_name    YTY_Line_glsl_GS
#pragma vp_entryPoint Line_GS
#pragma vp_location geometry

void VP_EmitModelVertex();

uniform float LineWidth;
uniform vec2  aaRadius;
uniform vec2  cameraSize;

out float g_line_width;
out float g_line_length;
out float g_u;
out float g_v;

layout(lines)in;
layout(triangle_strip, max_vertices=4) out;

void Line_GS(void)
{
    float u_width        = cameraSize[0];
    float u_height       = cameraSize[1];
    float u_aspect_ratio = u_height / u_width;

    vec4 prevClip = gl_ModelViewProjectionMatrix * gl_in[0].gl_Position;
    vec4 currClip = gl_ModelViewProjectionMatrix * gl_in[1].gl_Position;

    vec2 ndc_a = prevClip.xy / prevClip.w;
    vec2 ndc_b = currClip.xy / currClip.w;

    vec2 line_vector = ndc_b - ndc_a;
    vec2 viewport_line_vector = line_vector * cameraSize;
    vec2 dir = normalize(vec2( line_vector.x, line_vector.y * u_aspect_ratio ));

    float line_width_a     = max( 1.0, LineWidth ) + aaRadius[0];
    float line_width_b     = max( 1.0, LineWidth ) + aaRadius[0];
    float extension_length = aaRadius[1];
    float line_length      = length( viewport_line_vector ) + 2.0 * extension_length;
        
    vec2 normal    = vec2( -dir.y, dir.x );
    vec2 normal_a  = vec2( line_width_a/u_width, line_width_a/u_height ) * normal;
    vec2 normal_b  = vec2( line_width_b/u_width, line_width_b/u_height ) * normal;
    vec2 extension = vec2( extension_length / u_width, extension_length / u_height ) * dir;

    //g_col = vec4( v_col[0].rgb, v_col[0].a * min( v_line_width[0], 1.0f ) );
    g_u = line_width_a;
    g_v = line_length * 0.5;
    g_line_width = line_width_a;
    g_line_length = line_length * 0.5;
    gl_Position = vec4( (ndc_a + normal_a - extension) * prevClip.w, prevClip.zw );
    //VP_EmitModelVertex();
    EmitVertex();
        
    g_u = -line_width_a;
    g_v = line_length * 0.5;
    g_line_width = line_width_a;
    g_line_length = line_length * 0.5;
    gl_Position = vec4( (ndc_a - normal_a - extension) * prevClip.w, prevClip.zw );
    //VP_EmitModelVertex();
    EmitVertex();
        
    //g_col = vec4( v_col[0].rgb, v_col[0].a * min( v_line_width[0], 1.0f ) );
    g_u = line_width_b;
    g_v = -line_length * 0.5;
    g_line_width = line_width_b;
    g_line_length = line_length * 0.5;
    gl_Position = vec4( (ndc_b + normal_b + extension) * currClip.w, currClip.zw );
    //VP_EmitModelVertex();
    EmitVertex();
        
    g_u = -line_width_b;
    g_v = -line_length * 0.5;
    g_line_width = line_width_b;
    g_line_length = line_length * 0.5;
    gl_Position = vec4( (ndc_b - normal_b + extension) * currClip.w, currClip.zw );
    //VP_EmitModelVertex();
    EmitVertex();
        
    EndPrimitive();
}

[break]
#version $GLSL_VERSION_STR

#pragma vp_name    YTY_Line_glsl_FS
#pragma vp_entryPoint Line_FS
#pragma vp_location fragment_coloring

uniform vec2  aaRadius;
in float g_u;
in float g_v;
in float g_line_width;
in float g_line_length;

void Line_FS(inout vec4 color)
{
    float au = 1.0 - smoothstep( 1.0 - ((2.0*aaRadius[0]) / g_line_width),  1.0, abs(g_u / g_line_width) );
    float av = 1.0 - smoothstep( 1.0 - ((2.0*aaRadius[1]) / g_line_length), 1.0, abs(g_v / g_line_length) );
    color.a *= min(av, au);
}
