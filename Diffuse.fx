uniform extern float4x4 gWorldInverseTranspose;
uniform extern float4x4 gWorldViewProjection;
uniform extern float4x4	gWorld;

uniform extern float4 gDiffuseMtl;
uniform extern float4 gDiffuseLight;
uniform extern float3 gLightVecW;
uniform	extern float3 gEyePosW;

struct OutputVS 
{
	float4 posH : POSITION0;
	float4 color : COLOR0;
};

OutputVS DiffuseVS(float3 posL : POSITION0, float3 normalL : NORMAL0)
{
	OutputVS outVS = (OutputVS)0;

	float3 normalW = mul(float4(normalL, 0.0f), gWorldInverseTranspose).xyz;
	normalW = normalize(normalW);

	// float s = max(dot(gLightVecW, normalW), 0.0f);
	// outVS.color.rgb = s*(gDiffuseMtl*gDiffuseLight).rgb + float4(0.2, 0.2, 0.2, 0.0).rgb;
	// outVS.color.a = gDiffuseMtl.a;
	
	float4 gSpecular = float4(0.2f, 0.2f, 0.2f, 0.0f);
	float4 gAmbient	= float4(0.2f, 0.1f, 0.2f, 0.f);
	float gSpecularPower = 128;
	
	float3 posW = mul(float4(posL, 1.0f), gWorld).xyz;
	float3 toEye = normalize(gEyePosW - posW);
	float3 r = reflect(-gLightVecW, normalW);
	float t = pow(max(dot(r, toEye), 0.0f), gSpecularPower);
	float s = max(dot(gLightVecW, normalW), 0.0f);
	
	float3 spec = t*(gSpecular).rgb;
	float3 diffuse = s*(gDiffuseMtl*gDiffuseLight).rgb;
	float3 ambient = gAmbient;
	
	outVS.color.rgb = ambient + diffuse + spec;
	outVS.color.a = gDiffuseMtl.a;

	outVS.posH = mul(float4(posL, 1.0f), gWorldViewProjection);
	return outVS;
}

float4 DiffusePS(float4 c : COLOR0) : COLOR
{
	return c;
}

technique DiffuseTech
{
	pass P0
	{
		vertexShader = compile vs_2_0 DiffuseVS();
		pixelshader = compile ps_2_0 DiffusePS();
	}
}