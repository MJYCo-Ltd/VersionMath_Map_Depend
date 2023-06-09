char volume_multipass_cube_and_hull_frag[] = "#version 110\n"
                                             "\n"
                                             "uniform sampler2D colorTexture;\n"
                                             "uniform sampler2D depthTexture;\n"
                                             "uniform sampler2D backFaceDepthTexture;\n"
                                             "uniform sampler2D frontFaceDepthTexture;\n"
                                             "uniform vec4 viewportDimensions;\n"
                                             "uniform float ExteriorTransparencyFactorValue;\n"
                                             "\n"
                                             "\n"
                                             "// declare function defined in volume_compute_ray_color.frag\n"
                                             "vec4 computeRayColor(vec4 fragColor, float px, float py, float depth_start, float depth_end);\n"
                                             "\n"
                                             "vec4 computeSegment(vec4 fragColor, float px, float py, float depth_start, float depth_end, float transparencyFactor, vec4 scene_color, float scene_depth)\n"
                                             "{\n"
                                             "#if 1\n"
                                             "    //return mix(fragColor, computeRayColor(fragColor, px, py, depth_start, depth_end), transparencyFactor);\n"
                                             "    if ((depth_end<scene_depth) && (scene_depth<depth_start))\n"
                                             "    {\n"
                                             "        // scene fragment between segment end points\n"
                                             "        // compute front segment color and blend with scene fragment color\n"
                                             "        fragColor  = mix( fragColor, computeRayColor(fragColor, px, py, scene_depth, depth_end), transparencyFactor);\n"
                                             "        fragColor  = mix(scene_color, fragColor, fragColor.a);\n"
                                             "        if (fragColor.a>=1.0) return fragColor;\n"
                                             "\n"
                                             "        // compute rear segment color and blend with accumulated_color\n"
                                             "        return mix( fragColor, computeRayColor(fragColor, px, py, depth_start, scene_depth), transparencyFactor);\n"
                                             "    }\n"
                                             "    else\n"
                                             "    {\n"
                                             "        return mix( fragColor, computeRayColor(fragColor, px, py, depth_start, depth_end), transparencyFactor);\n"
                                             "    }\n"
                                             "#else\n"
                                             "    if ((depth_end<scene_depth) && (scene_depth<depth_start))\n"
                                             "    {\n"
                                             "        // scene fragment between segment end points\n"
                                             "        // compute front segment color and blend with scene fragment color\n"
                                             "        fragColor  = computeRayColor(fragColor, px, py, scene_depth, depth_end) * transparencyFactor;\n"
                                             "        fragColor  = mix(scene_color, fragColor, fragColor.a);\n"
                                             "        if (fragColor.a>=1.0) return fragColor;\n"
                                             "\n"
                                             "        // compute rear segment color and blend with accumulated_color\n"
                                             "        return computeRayColor(fragColor, px, py, depth_start, scene_depth) * transparencyFactor;\n"
                                             "    }\n"
                                             "    else\n"
                                             "    {\n"
                                             "        return computeRayColor(fragColor, px, py, depth_start, depth_end) * transparencyFactor;\n"
                                             "    }\n"
                                             "#endif\n"
                                             "}\n"
                                             "\n"
                                             "void main(void)\n"
                                             "{\n"
                                             "    vec2 texcoord = vec2((gl_FragCoord.x-viewportDimensions[0])/viewportDimensions[2], (gl_FragCoord.y-viewportDimensions[1])/viewportDimensions[3]);\n"
                                             "    vec4 color = texture2D( colorTexture, texcoord);\n"
                                             "    float texture_depth = texture2D( depthTexture, texcoord).s;\n"
                                             "    float back_depth = texture2D( backFaceDepthTexture, texcoord).s;\n"
                                             "    float front_depth = texture2D( frontFaceDepthTexture, texcoord).s;\n"
                                             "\n"
                                             "    //float ExteriorTransparencyFactorValue = 1.0;\n"
                                             "    vec4 fragColor = vec4(0.0, 0.0, 0.0, 0.0);\n"
                                             "\n"
                                             "    // make sure the front_depth and back_depth values represent usable depths for later tests\n"
                                             "    if (front_depth>back_depth)\n"
                                             "    {\n"
                                             "        if (front_depth==1.0)\n"
                                             "        {\n"
                                             "            // front face likely to have been clipped out by near plane\n"
                                             "            front_depth = 0.0;\n"
                                             "        }\n"
                                             "        else\n"
                                             "        {\n"
                                             "            // front and back faces of hull are reversed so treat them as empty.\n"
                                             "            front_depth = 1.0;\n"
                                             "            back_depth = 1.0;\n"
                                             "        }\n"
                                             "    }\n"
                                             "\n"
                                             "    if (back_depth<gl_FragCoord.z)\n"
                                             "    {\n"
                                             "        // segment front_depth to 0.0, exterior transparency\n"
                                             "        // segment back_depth to front_depth, interior transparency\n"
                                             "        // segment gl_FragCoord.z to back_depth, exterior transparency\n"
                                             "        fragColor = computeSegment(fragColor, gl_FragCoord.x, gl_FragCoord.y, front_depth, 0.0, ExteriorTransparencyFactorValue, color, texture_depth);\n"
                                             "        if (fragColor.a<1.0)\n"
                                             "        {\n"
                                             "            fragColor = computeSegment(fragColor, gl_FragCoord.x, gl_FragCoord.y, back_depth, front_depth, 1.0, color, texture_depth);\n"
                                             "            if (fragColor.a<1.0)\n"
                                             "            {\n"
                                             "                fragColor = computeSegment(fragColor, gl_FragCoord.x, gl_FragCoord.y, gl_FragCoord.z, back_depth, ExteriorTransparencyFactorValue, color, texture_depth);\n"
                                             "            }\n"
                                             "        }\n"
                                             "    }\n"
                                             "    else if (front_depth<gl_FragCoord.z)\n"
                                             "    {\n"
                                             "        // segment front_depth to 0, exterior transparency\n"
                                             "        // segment gl_FragCoord.z to front_depth, interior transparency\n"
                                             "        // back_depth behind gl_FragCoord.z so clipped out by cube and not required.\n"
                                             "        fragColor = computeSegment(fragColor, gl_FragCoord.x, gl_FragCoord.y, front_depth, 0.0, ExteriorTransparencyFactorValue, color, texture_depth);\n"
                                             "        if (fragColor.a<1.0)\n"
                                             "        {\n"
                                             "            fragColor = computeSegment(fragColor, gl_FragCoord.x, gl_FragCoord.y, gl_FragCoord.z, front_depth, 1.0, color, texture_depth);\n"
                                             "        }\n"
                                             "    }\n"
                                             "    else\n"
                                             "    {\n"
                                             "        // segment gl_FragCoord.z to 0.0\n"
                                             "        fragColor = computeSegment(fragColor, gl_FragCoord.x, gl_FragCoord.y, gl_FragCoord.z, 0.0, ExteriorTransparencyFactorValue, color, texture_depth);\n"
                                             "    }\n"
                                             "\n"
                                             "    if (texture_depth>gl_FragCoord.z)\n"
                                             "    {\n"
                                             "        fragColor = mix(color, fragColor, fragColor.a);\n"
                                             "    }\n"
                                             "\n"
                                             "    gl_FragColor = fragColor;\n"
                                             "    gl_FragDepth = 0.0;\n"
                                             "}\n"
                                             "\n";
