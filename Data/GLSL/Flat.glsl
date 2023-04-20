#version $GLSL_VERSION_STR

#pragma vp_name       YTY_Flat_glsl_VS
#pragma vp_entryPoint flatZ_vs
#pragma vp_location   vertex_model

uniform mat4x2 inVertex[10];
uniform int polySize;

int CroseV2(vec2 v1,vec2 v2)
{
    return int(v1.x*v2.y - v1.y*v2.x);
}

void flatZ_vs(inout vec4 vertex)
{
    vec2 vAB,vBC,vCD,vDA;
    int  nAB,nBC,nCD,nDA;
    for(int index=0;index<polySize;++index)
    {
        vAB = inVertex[index][0]-inVertex[index][1];
        vBC = inVertex[index][1]-inVertex[index][2];
        vCD = inVertex[index][2]-inVertex[index][3];
        vDA = inVertex[index][3]-inVertex[index][0];

        nAB = CroseV2(vAB,vertex.xy-inVertex[index][0]);
        nBC = CroseV2(vBC,vertex.xy-inVertex[index][1]);
        nCD = CroseV2(vCD,vertex.xy-inVertex[index][2]);
        nDA = CroseV2(vDA,vertex.xy-inVertex[index][3]);

	    if((nAB>0 && nBC>0 && nCD>0 && nDA>0) ||
           (nAB<0 && nBC<0 && nCD<0 && nDA<0))
	    {
	    	vertex.z = 0.;
	    }
    }
}