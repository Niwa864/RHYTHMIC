float4x4 proj;
texture tex;
texture tex2;

sampler smp = sampler_state {
	texture = <tex>;
};

sampler spot = sampler_state {
	texture = <tex2>;
};

struct VS_IN {
	float4 pos : POSITION;
	float2 uv : TEXCOORD0;
};

struct VS_OUT {
	float4 pos : POSITION;
	float2 uv : TEXCOORD0;
};


// 頂点シェーダ
VS_OUT vs_main(VS_IN In) {
	VS_OUT Out = (VS_OUT)0;

	Out.pos = mul(In.pos, proj);
	Out.uv = In.uv;

	return Out;
}

float4 ps_test(VS_OUT In) : COLOR0
{
	float4 output;
	float4 cd = tex2D(smp, In.uv);
	float4 cs = tex2D(spot, In.uv);
	cs.a = 1.0f;
	output.a = (1.0 - cs.a) * cd.a + cs.a;
	output.rgb = ((cs.rgb * cs.a) + (cd.rgb * cd.a) * (1.0 - cs.a)) / output.a;
	return output;
}

technique Tech {
	pass p0 {
		VertexShader = compile vs_2_0 vs_main();
		PixelShader = compile ps_2_0 ps_test();

		AlphaBlendEnable = true;
		SrcBlend = SRCALPHA;
		DestBlend = INVSRCALPHA;
	}
}