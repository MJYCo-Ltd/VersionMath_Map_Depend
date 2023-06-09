/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2014 Robert Osfield
 *  Copyright (C) 2014 Pawel Ksiezopolski
 *
 * This library is open source and may be redistributed and/or modified under  
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or 
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * OpenSceneGraph Public License for more details.
 *
*/
#ifndef GPU_CULL_SHADERS
#define GPU_CULL_SHADERS 1

char SHADER_STATIC_CULL_VERTEX[] = 
    "#version 420 compatibility\n"
    "\n"
    "uniform mat4 osg_ViewMatrixInverse;\n"
    "\n"
    "layout(R32I) coherent uniform iimageBuffer indirectCommand0;\n"
    "layout(R32I) coherent uniform iimageBuffer indirectCommand1;\n"
    "uniform int indirectCommandSize; // = sizeof(DrawArraysIndirectCommand) / sizeof(unsigned int) = 4\n"
    "\n"
    "layout(R32I) coherent uniform iimageBuffer getIndirectCommand( int index )\n"
    "{\n"
    "    if(index==0) return indirectCommand0;\n"
    "    if(index==1) return indirectCommand1;\n"
    "    return indirectCommand0;\n"
    "}\n"
    "\n"
    "layout(RGBA32F) coherent uniform imageBuffer indirectTarget0;\n"
    "layout(RGBA32F) coherent uniform imageBuffer indirectTarget1;\n"
    "\n"
    "layout(RGBA32F) coherent uniform imageBuffer getIndirectTarget( int index )\n"
    "{\n"
    "    if(index==0) return indirectTarget0;\n"
    "    if(index==1) return indirectTarget1;\n"
    "    return indirectTarget0;\n"
    "}\n"
    "\n"
    "struct InstanceLOD\n"
    "{\n"
    "    vec4 bbMin;\n"
    "    vec4 bbMax;\n"
    "    ivec4 indirectTargetParams;    // x=targetID, y=indexInTarget, z=offsetInTarget, w=lodMaxQuantity\n"
    "    vec4 distances;               // x=minDistance, y=minFadeDistance, z=maxFadeDistance, w=maxDistance\n"
    "};\n"
    "\n"
    "const int OSGGPUCULL_MAXIMUM_LOD_NUMBER = 8;\n"
    "\n"
    "struct InstanceType\n"
    "{\n"
    "    vec4 bbMin;\n"
    "    vec4 bbMax;\n"
    "    ivec4 params;\n"
    "    InstanceLOD lods[OSGGPUCULL_MAXIMUM_LOD_NUMBER];\n"
    "};\n"
    "\n"
    "layout(std140) uniform instanceTypesData\n"
    "{\n"
    "    InstanceType instanceTypes[32];\n"
    "};\n"
    "\n"
    "// StaticInstance params are stored in vertex attributes\n"
    "layout(location = 0) in vec3 VertexPosition;\n"
    "layout(location = 10) in vec4 M0;\n"
    "layout(location = 11) in vec4 M1;\n"
    "layout(location = 12) in vec4 M2;\n"
    "layout(location = 13) in vec4 M3;\n"
    "layout(location = 14) in vec4 ExtraParams;\n"
    "layout(location = 15) in vec4 IdParams;\n"
    "\n"
    "out vec4 fColor;\n"
    "\n"
    "bool boundingBoxInViewFrustum( in mat4 matrix, in vec3 bbMin, in vec3 bbMax )\n"
    "{\n"
    "    vec4 BoundingBox[8];\n"
    "    BoundingBox[0] = matrix * vec4( bbMax.x, bbMax.y, bbMax.z, 1.0);\n"
    "    BoundingBox[1] = matrix * vec4( bbMin.x, bbMax.y, bbMax.z, 1.0);\n"
    "    BoundingBox[2] = matrix * vec4( bbMax.x, bbMin.y, bbMax.z, 1.0);\n"
    "    BoundingBox[3] = matrix * vec4( bbMin.x, bbMin.y, bbMax.z, 1.0);\n"
    "    BoundingBox[4] = matrix * vec4( bbMax.x, bbMax.y, bbMin.z, 1.0);\n"
    "    BoundingBox[5] = matrix * vec4( bbMin.x, bbMax.y, bbMin.z, 1.0);\n"
    "    BoundingBox[6] = matrix * vec4( bbMax.x, bbMin.y, bbMin.z, 1.0);\n"
    "    BoundingBox[7] = matrix * vec4( bbMin.x, bbMin.y, bbMin.z, 1.0);\n"
    "\n"
    "    int outOfBound[6] = int[6]( 0, 0, 0, 0, 0, 0 );\n"
    "    for (int i=0; i<8; i++)\n"
    "    {\n"
    "        outOfBound[0] += int( BoundingBox[i].x >  BoundingBox[i].w );\n"
    "        outOfBound[1] += int( BoundingBox[i].x < -BoundingBox[i].w );\n"
    "        outOfBound[2] += int( BoundingBox[i].y >  BoundingBox[i].w );\n"
    "        outOfBound[3] += int( BoundingBox[i].y < -BoundingBox[i].w );\n"
    "        outOfBound[4] += int( BoundingBox[i].z >  BoundingBox[i].w );\n"
    "        outOfBound[5] += int( BoundingBox[i].z < -BoundingBox[i].w );\n"
    "    }\n"
    "    return (outOfBound[0] < 8 ) && ( outOfBound[1] < 8 ) && ( outOfBound[2] < 8 ) && ( outOfBound[3] < 8 ) && ( outOfBound[4] < 8 ) && ( outOfBound[5] < 8 );\n"
    "}\n"
    "\n"
    "void main(void) \n"
    "{\n"
    "    mat4 instanceMatrix = mat4(M0,M1,M2,M3);\n"
    "    mat4 mvpoMatrix = gl_ModelViewProjectionMatrix * instanceMatrix;\n"
    "\n"
    "    // gl_Position is created only for debugging purposes\n"
    "    gl_Position = mvpoMatrix * vec4(0.0,0.0,0.0,1.0);\n"
    "\n"
    "    int instanceTypeIndex = int(IdParams.x);\n"
    "\n"
    "    fColor = vec4(0.0,0.0,0.0,1.0);\n"
    "    if( boundingBoxInViewFrustum( mvpoMatrix, instanceTypes[instanceTypeIndex].bbMin.xyz, instanceTypes[instanceTypeIndex].bbMax.xyz ) )\n"
    "    {\n"
    "        fColor = vec4(1.0,0.0,0.0,1.0);\n"
    "        float distanceToObject = distance(osg_ViewMatrixInverse[3].xyz / osg_ViewMatrixInverse[3].w, instanceMatrix[3].xyz / instanceMatrix[3].w );\n"
    "        for(int i=0;i<instanceTypes[instanceTypeIndex].params.x;++i)\n"
    "        {\n"
    "            if( boundingBoxInViewFrustum( mvpoMatrix, instanceTypes[instanceTypeIndex].lods[i].bbMin.xyz, instanceTypes[instanceTypeIndex].lods[i].bbMax.xyz ) &&\n"
    "                ( distanceToObject >= instanceTypes[instanceTypeIndex].lods[i].distances.x ) && ( distanceToObject < instanceTypes[instanceTypeIndex].lods[i].distances.w ) )\n"
    "            {\n"
    "                fColor = vec4(1.0,1.0,0.0,1.0);\n"
    "                float fadeAlpha  = 4.0 * ( clamp( (distanceToObject-instanceTypes[instanceTypeIndex].lods[i].distances.x)/( instanceTypes[instanceTypeIndex].lods[i].distances.y - instanceTypes[instanceTypeIndex].lods[i].distances.x), 0.0, 1.0 ) \n"
    "                    +  clamp( (distanceToObject- instanceTypes[instanceTypeIndex].lods[i].distances.z)/( instanceTypes[instanceTypeIndex].lods[i].distances.w - instanceTypes[instanceTypeIndex].lods[i].distances.z), 0.0, 1.0 ) );\n"
    "                int sampleMask = ( 0xF0 >> int(fadeAlpha) ) & 0xF;\n"
    "                int indirectCommandIndex   = instanceTypes[instanceTypeIndex].lods[i].indirectTargetParams.x;\n"
    "                int indirectCommandAddress = instanceTypes[instanceTypeIndex].lods[i].indirectTargetParams.y;\n"
    "                int objectIndex            = imageAtomicAdd( getIndirectCommand( indirectCommandIndex ), indirectCommandAddress*indirectCommandSize+1, 1 );\n"
    "                int indirectTargetAddress  = 6*(instanceTypes[instanceTypeIndex].lods[i].indirectTargetParams.z + objectIndex);\n"
    "                imageStore( getIndirectTarget(indirectCommandIndex), indirectTargetAddress+0, M0 );\n"
    "                imageStore( getIndirectTarget(indirectCommandIndex), indirectTargetAddress+1, M1 );\n"
    "                imageStore( getIndirectTarget(indirectCommandIndex), indirectTargetAddress+2, M2 );\n"
    "                imageStore( getIndirectTarget(indirectCommandIndex), indirectTargetAddress+3, M3 );\n"
    "                imageStore( getIndirectTarget(indirectCommandIndex), indirectTargetAddress+4, ExtraParams );\n"
    "                imageStore( getIndirectTarget(indirectCommandIndex), indirectTargetAddress+5, vec4(IdParams.x,IdParams.y,float(sampleMask),0.0) );\n"
    "            }\n"
    "        }\n"
    "    }\n"
    "}\n";


char SHADER_STATIC_CULL_FRAGMENT[] = 
    "#version 420 compatibility\n"
    "\n"
    "layout(location = 0, index = 0) out vec4 FragmentColor;\n"
    "\n"
    "in vec4 fColor;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    FragmentColor = fColor;\n"
    "}\n";


char SHADER_STATIC_DRAW_0_VERTEX[] = 
    "#version 420 compatibility\n"
    "\n"
    "layout(location = 0) in vec4 VertexPosition;\n"
    "layout(location = 2) in vec3 VertexNormal;\n"
    "layout(location = 3) in vec4 VertexColor;\n"
    "layout(location = 8) in vec2 VertexTexCoord0;\n"
    "layout(location = 9) in vec3 VertexTexCoord1;\n"
    "\n"
    "struct InstanceLOD\n"
    "{\n"
    "    vec4 bbMin;\n"
    "    vec4 bbMax;\n"
    "    ivec4 indirectTargetParams;    // x=targetID, y=indexInTarget, z=offsetInTarget, w=lodMaxQuantity\n"
    "    vec4 distances;               // x=minDistance, y=minFadeDistance, z=maxFadeDistance, w=maxDistance\n"
    "};\n"
    "\n"
    "const int OSGGPUCULL_MAXIMUM_LOD_NUMBER = 8;\n"
    "\n"
    "struct InstanceType\n"
    "{\n"
    "    vec4 bbMin;\n"
    "    vec4 bbMax;\n"
    "    ivec4 params;\n"
    "    InstanceLOD lods[OSGGPUCULL_MAXIMUM_LOD_NUMBER];\n"
    "};\n"
    "\n"
    "layout(std140) uniform instanceTypesData\n"
    "{\n"
    "    InstanceType instanceTypes[32];\n"
    "};\n"
    "\n"
    "layout(RGBA32F) coherent uniform imageBuffer indirectTarget;\n"
    "\n"
    "out vec3 ecPosition3;\n"
    "out vec3 ecNormal;\n"
    "out vec2 TexCoord;\n"
    "out vec4 color;\n"
    "flat out int sMask;\n"
    "  \n"
    "void main()\n"
    "{\n"
    "   // every vertex has its type coded on VertexTexCoord1.x,\n"
    "   // and its lodNumber coded in VertexTexCoord1.y\n"
    "   int instanceTypeIndex = int(VertexTexCoord1.x);\n"
    "   int instanceLodNumber = int(VertexTexCoord1.y);\n"
    "   int indirectTargetAddress  = 6*(instanceTypes[instanceTypeIndex].lods[instanceLodNumber].indirectTargetParams.z + gl_InstanceID);\n"
    "   mat4 instanceMatrix = mat4(\n"
    "       imageLoad(indirectTarget, indirectTargetAddress + 0),\n"
    "       imageLoad(indirectTarget, indirectTargetAddress + 1),\n"
    "       imageLoad(indirectTarget, indirectTargetAddress + 2),\n"
    "       imageLoad(indirectTarget, indirectTargetAddress + 3) );\n"
    "   vec4 extraParams = imageLoad(indirectTarget, indirectTargetAddress + 4);\n"
    "   vec4 idParams = imageLoad(indirectTarget, indirectTargetAddress + 5);\n"
    "\n"
    "   sMask = int(idParams.z);\n"
    "\n"
    "   mat4 mvMatrix = gl_ModelViewMatrix * instanceMatrix;\n"
    "   \n"
    "   // Do fixed functionality vertex transform\n"
    "   vec4  ecPos  = mvMatrix * vec4(VertexPosition.xyz,1.0);\n"
    "   gl_Position = gl_ProjectionMatrix * ecPos;\n"
    "\n"
    "   ecPosition3 = ecPos.xyz / ecPos.w;\n"
    "   ecNormal = normalize( mat3(mvMatrix) * VertexNormal.xyz );\n"
    "   \n"
    "   TexCoord = VertexTexCoord0.xy;\n"
    "   color.xyz = VertexColor.xyz * extraParams.x;\n"
    "   color.a = VertexColor.a;\n"
    "}\n";


char SHADER_STATIC_DRAW_0_FRAGMENT[] = 
    "#version 420 compatibility\n"
    "\n"
    "in vec3 ecPosition3;\n"
    "in vec3 ecNormal;\n"
    "in vec2 TexCoord;\n"
    "in vec4 color;\n"
    "\n"
    "flat in int sMask;\n"
    "\n"
    "layout(location = 0) out vec4 FragColor;\n"
    "\n"
    "void main()\n"
    "{\n"
    "// simple fragment shader that calculates ambient + diffuse lighting with osg::LightSource\n"
    "    vec3 lightDir = normalize(vec3(gl_LightSource[0].position));\n"
    "    vec3 normal   = normalize(ecNormal);\n"
    "    float NdotL   = max(dot(normal, lightDir), 0.0);\n"
    "    FragColor =  NdotL * color * gl_LightSource[0].diffuse +  color * gl_LightSource[0].ambient;\n"
    "    gl_SampleMask[0] = sMask;\n"
    "}\n";


char SHADER_STATIC_DRAW_1_VERTEX[] = 
    "#version 420 compatibility\n"
    "\n"
    "layout(location = 0) in vec4 VertexPosition;\n"
    "layout(location = 2) in vec3 VertexNormal;\n"
    "layout(location = 3) in vec4 VertexColor;\n"
    "layout(location = 8) in vec2 VertexTexCoord0;\n"
    "layout(location = 9) in vec3 VertexTexCoord1;\n"
    "\n"
    "struct InstanceLOD\n"
    "{\n"
    "    vec4 bbMin;\n"
    "    vec4 bbMax;\n"
    "    ivec4 indirectTargetParams;    // x=targetID, y=indexInTarget, z=offsetInTarget, w=lodMaxQuantity\n"
    "    vec4 distances;               // x=minDistance, y=minFadeDistance, z=maxFadeDistance, w=maxDistance\n"
    "};\n"
    "\n"
    "const int OSGGPUCULL_MAXIMUM_LOD_NUMBER = 8;\n"
    "\n"
    "struct InstanceType\n"
    "{\n"
    "    vec4 bbMin;\n"
    "    vec4 bbMax;\n"
    "    ivec4 params;\n"
    "    InstanceLOD lods[OSGGPUCULL_MAXIMUM_LOD_NUMBER];\n"
    "};\n"
    "\n"
    "layout(std140) uniform instanceTypesData\n"
    "{\n"
    "    InstanceType instanceTypes[32];\n"
    "};\n"
    "\n"
    "layout(RGBA32F) coherent uniform imageBuffer indirectTarget;\n"
    "\n"
    "uniform vec2 windDirection;\n"
    "uniform float osg_FrameTime;\n"
    "\n"
    "out vec3 ecPosition3;\n"
    "out vec3 ecNormal;\n"
    "out vec2 TexCoord;\n"
    "out vec4 color;\n"
    "flat out int sMask;\n"
    "  \n"
    "void main()\n"
    "{\n"
    "   // every vertex has its type coded on VertexTexCoord1.x,\n"
    "   // and its lodNumber coded in VertexTexCoord1.y\n"
    "   int instanceTypeIndex = int(VertexTexCoord1.x);\n"
    "   int instanceLodNumber = int(VertexTexCoord1.y);\n"
    "   int indirectTargetAddress  = 6*(instanceTypes[instanceTypeIndex].lods[instanceLodNumber].indirectTargetParams.z + gl_InstanceID);\n"
    "   mat4 instanceMatrix = mat4(\n"
    "       imageLoad(indirectTarget, indirectTargetAddress + 0),\n"
    "       imageLoad(indirectTarget, indirectTargetAddress + 1),\n"
    "       imageLoad(indirectTarget, indirectTargetAddress + 2),\n"
    "       imageLoad(indirectTarget, indirectTargetAddress + 3) );\n"
    "   vec4 extraParams = imageLoad(indirectTarget, indirectTargetAddress + 4);\n"
    "   vec4 idParams = imageLoad(indirectTarget, indirectTargetAddress + 5);\n"
    "\n"
    "   sMask = int(idParams.z);\n"
    "\n"
    "   // simple tree waving in the wind. Amplitude, frequency and offset are coded in extraParams\n"
    "   vec4 mPos = instanceMatrix * vec4(VertexPosition.xyz,1.0);\n"
    "   float wavingAmplitute = VertexPosition.z * extraParams.y;\n"
    "   mPos.xy += windDirection * wavingAmplitute * sin( extraParams.z*osg_FrameTime + extraParams.w );\n"
    "\n"
    "   mat4 mvMatrix = gl_ModelViewMatrix * instanceMatrix;\n"
    "   \n"
    "   // Do fixed functionality vertex transform\n"
    "   vec4  ecPos  = osg_ModelViewMatrix * mPos;\n"
    "   gl_Position = gl_ProjectionMatrix * ecPos;\n"
    "\n"
    "   ecPosition3 = ecPos.xyz / ecPos.w;\n"
    "   ecNormal = normalize( mat3(mvMatrix) * VertexNormal.xyz );\n"
    "   \n"
    "   TexCoord = VertexTexCoord0.xy;\n"
    "   color.xyz = VertexColor.xyz * extraParams.x;\n"
    "   color.a = VertexColor.a;\n"
    "}\n";


char SHADER_STATIC_DRAW_1_FRAGMENT[] = 
    "#version 420 compatibility\n"
    "\n"
    "in vec3 ecPosition3;\n"
    "in vec3 ecNormal;\n"
    "in vec2 TexCoord;\n"
    "in vec4 color;\n"
    "\n"
    "flat in int sMask;\n"
    "\n"
    "layout(location = 0) out vec4 FragColor;\n"
    "\n"
    "void main()\n"
    "{\n"
    "// simple fragment shader that calculates ambient + diffuse lighting with osg::LightSource\n"
    "    vec3 lightDir = normalize(vec3(gl_LightSource[0].position));\n"
    "    vec3 normal   = normalize(ecNormal);\n"
    "    float NdotL   = max(dot(normal, lightDir), 0.0);\n"
    "    FragColor =  NdotL * color * gl_LightSource[0].diffuse +  color * gl_LightSource[0].ambient;\n"
    "    gl_SampleMask[0] = sMask;\n"
    "}\n";


char SHADER_DYNAMIC_CULL_VERTEX[] = 
    "#version 420 compatibility\n"
    "\n"
    "uniform mat4 osg_ViewMatrixInverse;\n"
    "\n"
    "uniform samplerBuffer dynamicInstancesData;\n"
    "uniform int dynamicInstancesDataSize; // = sizeof(DynamicInstance) / sizeof(osg::Vec4f)\n"
    "\n"
    "layout(R32I) coherent uniform iimageBuffer indirectCommand0;\n"
    "uniform int indirectCommandSize; // = sizeof(DrawArraysIndirectCommand) / sizeof(unsigned int) = 4\n"
    "\n"
    "layout(R32I) coherent uniform iimageBuffer getIndirectCommand( int index )\n"
    "{\n"
    "  if(index==0) return indirectCommand0;\n"
    "}\n"
    "\n"
    "layout(RGBA32I) coherent uniform iimageBuffer indirectTarget0;\n"
    "\n"
    "layout(RGBA32I) coherent uniform iimageBuffer getIndirectTarget( int index )\n"
    "{\n"
    "  if(index==0) return indirectTarget0;\n"
    "}\n"
    "\n"
    "struct InstanceLOD\n"
    "{\n"
    "    vec4 bbMin;\n"
    "    vec4 bbMax;\n"
    "    ivec4 indirectTargetParams;    // x=targetID, y=indexInTarget, z=offsetInTarget, w=lodMaxQuantity\n"
    "    vec4 distances;               // x=minDistance, y=minFadeDistance, z=maxFadeDistance, w=maxDistance\n"
    "};\n"
    "\n"
    "const int OSGGPUCULL_MAXIMUM_LOD_NUMBER = 8;\n"
    "\n"
    "struct InstanceType\n"
    "{\n"
    "    vec4 bbMin;\n"
    "    vec4 bbMax;\n"
    "    ivec4 params;\n"
    "    InstanceLOD lods[OSGGPUCULL_MAXIMUM_LOD_NUMBER];\n"
    "};\n"
    "\n"
    "layout(std140) uniform instanceTypesData\n"
    "{\n"
    "    InstanceType instanceTypes[32];\n"
    "};\n"
    "\n"
    "layout(location = 0) in vec3 VertexPosition; // xyz - used only for debugging purposes\n"
    "layout(location = 10) in vec4 InstanceID; // typeid, id - points to the data in dynamicInstancesData buffer\n"
    "\n"
    "bool boundingBoxInViewFrustum( in mat4 matrix, in vec3 bbMin, in vec3 bbMax )\n"
    "{\n"
    "    vec4 BoundingBox[8];\n"
    "    BoundingBox[0] = matrix * vec4( bbMax.x, bbMax.y, bbMax.z, 1.0);\n"
    "    BoundingBox[1] = matrix * vec4( bbMin.x, bbMax.y, bbMax.z, 1.0);\n"
    "    BoundingBox[2] = matrix * vec4( bbMax.x, bbMin.y, bbMax.z, 1.0);\n"
    "    BoundingBox[3] = matrix * vec4( bbMin.x, bbMin.y, bbMax.z, 1.0);\n"
    "    BoundingBox[4] = matrix * vec4( bbMax.x, bbMax.y, bbMin.z, 1.0);\n"
    "    BoundingBox[5] = matrix * vec4( bbMin.x, bbMax.y, bbMin.z, 1.0);\n"
    "    BoundingBox[6] = matrix * vec4( bbMax.x, bbMin.y, bbMin.z, 1.0);\n"
    "    BoundingBox[7] = matrix * vec4( bbMin.x, bbMin.y, bbMin.z, 1.0);\n"
    "\n"
    "    int outOfBound[6] = int[6]( 0, 0, 0, 0, 0, 0 );\n"
    "    for (int i=0; i<8; i++)\n"
    "    {\n"
    "        outOfBound[0] += int( BoundingBox[i].x >  BoundingBox[i].w );\n"
    "        outOfBound[1] += int( BoundingBox[i].x < -BoundingBox[i].w );\n"
    "        outOfBound[2] += int( BoundingBox[i].y >  BoundingBox[i].w );\n"
    "        outOfBound[3] += int( BoundingBox[i].y < -BoundingBox[i].w );\n"
    "        outOfBound[4] += int( BoundingBox[i].z >  BoundingBox[i].w );\n"
    "        outOfBound[5] += int( BoundingBox[i].z < -BoundingBox[i].w );\n"
    "    }\n"
    "    return (outOfBound[0] < 8 ) && ( outOfBound[1] < 8 ) && ( outOfBound[2] < 8 ) && ( outOfBound[3] < 8 ) && ( outOfBound[4] < 8 ) && ( outOfBound[5] < 8 );\n"
    "}\n"
    "\n"
    "void main(void) \n"
    "{\n"
    "    // get object matrices\n"
    "    int instanceIndex = int(InstanceID.y);\n"
    "    int instanceAddress = instanceIndex * dynamicInstancesDataSize;\n"
    "    mat4 instanceMatrix = mat4 (\n"
    "        texelFetch(dynamicInstancesData, instanceAddress),\n"
    "        texelFetch(dynamicInstancesData, instanceAddress + 1),\n"
    "        texelFetch(dynamicInstancesData, instanceAddress + 2),\n"
    "        texelFetch(dynamicInstancesData, instanceAddress + 3) );\n"
    "    mat4 mvpoMatrix = gl_ModelViewProjectionMatrix * instanceMatrix;\n"
    "\n"
    "    // gl_Position is created only for debugging purposes\n"
    "    gl_Position = mvpoMatrix * vec4(0.0,0.0,0.0,1.0);\n"
    "\n"
    "    int instanceTypeIndex = int(InstanceID.x);\n"
    "\n"
    "    if( boundingBoxInViewFrustum( mvpoMatrix, instanceTypes[instanceTypeIndex].bbMin.xyz, instanceTypes[instanceTypeIndex].bbMax.xyz ) )\n"
    "    {\n"
    "        float distanceToObject = distance(osg_ViewMatrixInverse[3].xyz / osg_ViewMatrixInverse[3].w, instanceMatrix[3].xyz / instanceMatrix[3].w );\n"
    "        for(int i=0;i<instanceTypes[instanceTypeIndex].params.x;++i)\n"
    "        {\n"
    "            if( boundingBoxInViewFrustum( mvpoMatrix, instanceTypes[instanceTypeIndex].lods[i].bbMin.xyz, instanceTypes[instanceTypeIndex].lods[i].bbMax.xyz ) &&\n"
    "                ( distanceToObject >= instanceTypes[instanceTypeIndex].lods[i].distances.x ) && ( distanceToObject < instanceTypes[instanceTypeIndex].lods[i].distances.w ) )\n"
    "            {\n"
    "                float fadeAlpha  = 4.0 * ( clamp( (distanceToObject-instanceTypes[instanceTypeIndex].lods[i].distances.x)/( instanceTypes[instanceTypeIndex].lods[i].distances.y - instanceTypes[instanceTypeIndex].lods[i].distances.x), 0.0, 1.0 ) \n"
    "                    +  clamp( (distanceToObject- instanceTypes[instanceTypeIndex].lods[i].distances.z)/( instanceTypes[instanceTypeIndex].lods[i].distances.w - instanceTypes[instanceTypeIndex].lods[i].distances.z), 0.0, 1.0 ) );\n"
    "                int sampleMask = ( 0xF0 >> int(fadeAlpha) ) & 0xF;\n"
    "                int indirectCommandIndex   = instanceTypes[instanceTypeIndex].lods[i].indirectTargetParams.x;\n"
    "                int indirectCommandAddress = instanceTypes[instanceTypeIndex].lods[i].indirectTargetParams.y;\n"
    "                int objectIndex            = imageAtomicAdd( getIndirectCommand( indirectCommandIndex ), indirectCommandAddress*indirectCommandSize+1, 1 );\n"
    "                int indirectTargetAddress  = instanceTypes[instanceTypeIndex].lods[i].indirectTargetParams.z + objectIndex;\n"
    "                ivec4 indirectCommandData  = ivec4( instanceTypeIndex, instanceIndex, sampleMask, 0 );\n"
    "                imageStore( getIndirectTarget(indirectCommandIndex), indirectTargetAddress, indirectCommandData );\n"
    "            }\n"
    "        }\n"
    "    }\n"
    "}\n";


char SHADER_DYNAMIC_CULL_FRAGMENT[] = 
    "#version 420 compatibility\n"
    "\n"
    "layout(location = 0, index = 0) out vec4 FragmentColor;\n"
    "\n"
    "void main()\n"
    "{\n"
    "  FragmentColor = vec4(1.0,1.0,0.1,1.0);\n"
    "}\n";


char SHADER_DYNAMIC_DRAW_0_VERTEX[] = 
    "#version 420 compatibility\n"
    "\n"
    "uniform samplerBuffer dynamicInstancesData;\n"
    "uniform int dynamicInstancesDataSize; // = sizeof(DynamicInstance) / sizeof(osg::Vec4f)\n"
    "\n"
    "layout(location = 0) in vec4 VertexPosition;\n"
    "layout(location = 2) in vec3 VertexNormal;\n"
    "layout(location = 3) in vec4 VertexColor;\n"
    "layout(location = 8) in vec2 VertexTexCoord0;\n"
    "layout(location = 9) in vec3 VertexTexCoord1;\n"
    "\n"
    "struct InstanceLOD\n"
    "{\n"
    "    vec4 bbMin;\n"
    "    vec4 bbMax;\n"
    "    ivec4 indirectTargetParams;    // x=targetID, y=indexInTarget, z=offsetInTarget, w=lodMaxQuantity\n"
    "    vec4 distances;               // x=minDistance, y=minFadeDistance, z=maxFadeDistance, w=maxDistance\n"
    "};\n"
    "\n"
    "const int OSGGPUCULL_MAXIMUM_LOD_NUMBER = 8;\n"
    "\n"
    "struct InstanceType\n"
    "{\n"
    "    vec4 bbMin;\n"
    "    vec4 bbMax;\n"
    "    ivec4 params;\n"
    "    InstanceLOD lods[OSGGPUCULL_MAXIMUM_LOD_NUMBER];\n"
    "};\n"
    "\n"
    "layout(std140) uniform instanceTypesData\n"
    "{\n"
    "    InstanceType instanceTypes[32];\n"
    "};\n"
    "\n"
    "layout(RGBA32I) coherent uniform iimageBuffer indirectTarget;\n"
    "\n"
    "out vec3 ecPosition3;\n"
    "out vec3 ecNormal;\n"
    "out vec2 TexCoord;\n"
    "out vec4 color;\n"
    "flat out int sMask;\n"
    "  \n"
    "void main()\n"
    "{\n"
    "   // every vertex has its type coded on VertexTexCoord1.x,\n"
    "   // its lodNumber coded in VertexTexCoord1.y\n"
    "   // and bone index coded in VertexTexCoord1.z\n"
    "   int instanceTypeIndex = int(VertexTexCoord1.x);\n"
    "   int instanceLodNumber = int(VertexTexCoord1.y);\n"
    "   int boneIndex = int(VertexTexCoord1.z);\n"
    "   int indirectTargetAddress  = instanceTypes[instanceTypeIndex].lods[instanceLodNumber].indirectTargetParams.z + gl_InstanceID;\n"
    "\n"
    "   // if we know instance type, lod number and gl_InstanceID, we can reconstruct all instance data\n"
    "   ivec4 indirectCommandData = imageLoad(indirectTarget, indirectTargetAddress);\n"
    "   // now its time to read instance matrix and other additional params\n"
    "   int instanceAddress = indirectCommandData.y * dynamicInstancesDataSize;\n"
    "   mat4 instanceMatrix = mat4 (\n"
    "       texelFetch(dynamicInstancesData, instanceAddress),\n"
    "       texelFetch(dynamicInstancesData, instanceAddress + 1),\n"
    "       texelFetch(dynamicInstancesData, instanceAddress + 2),\n"
    "       texelFetch(dynamicInstancesData, instanceAddress + 3) );\n"
    "\n"
    "   vec4 instanceExtraParams = texelFetch(dynamicInstancesData, instanceAddress + 4);\n"
    "   // we also need boneMatrix to animate an object\n"
    "   int boneAddress = instanceAddress + 6 + 4*boneIndex;\n"
    "   mat4 boneMatrix = mat4 (\n"
    "       texelFetch(dynamicInstancesData, boneAddress),\n"
    "       texelFetch(dynamicInstancesData, boneAddress + 1),\n"
    "       texelFetch(dynamicInstancesData, boneAddress + 2),\n"
    "       texelFetch(dynamicInstancesData, boneAddress + 3) );\n"
    "\n"
    "   sMask = indirectCommandData.z;\n"
    "\n"
    "   mat4 mvMatrix = gl_ModelViewMatrix * instanceMatrix * boneMatrix;\n"
    "   \n"
    "   // Do fixed functionality vertex transform\n"
    "   vec4  ecPos  = mvMatrix * vec4(VertexPosition.xyz,1.0);\n"
    "   gl_Position = gl_ProjectionMatrix * ecPos;\n"
    "\n"
    "   ecPosition3 = ecPos.xyz / ecPos.w;\n"
    "   ecNormal = normalize( mat3(mvMatrix) * VertexNormal.xyz );\n"
    "   \n"
    "   TexCoord = VertexTexCoord0.xy;\n"
    "   color.xyz = VertexColor.xyz * instanceExtraParams.x;\n"
    "   color.a = VertexColor.a;\n"
    "}\n";


char SHADER_DYNAMIC_DRAW_0_FRAGMENT[] = 
    "#version 420 compatibility\n"
    "\n"
    "in vec3 ecPosition3;\n"
    "in vec3 ecNormal;\n"
    "in vec2 TexCoord;\n"
    "in vec4 color;\n"
    "\n"
    "flat in int sMask;\n"
    "\n"
    "layout(location = 0) out vec4 FragColor;\n"
    "\n"
    "void main()\n"
    "{\n"
    "// simple fragment shader that calculates ambient + diffuse lighting with osg::LightSource\n"
    "    vec3 lightDir = normalize(vec3(gl_LightSource[0].position));\n"
    "    vec3 normal   = normalize(ecNormal);\n"
    "    float NdotL   = max(dot(normal, lightDir), 0.0);\n"
    "    FragColor =  NdotL * color * gl_LightSource[0].diffuse +  color * gl_LightSource[0].ambient;\n"
    "    gl_SampleMask[0] = sMask;\n"
    "}\n";


#endif

