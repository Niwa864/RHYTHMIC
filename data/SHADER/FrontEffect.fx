float4x4 proj;
float3 viewProt;
float alpha;
float AddU;
float AddV;
texture tex;
float1 time;
float screenW;
float screenH;

sampler smp = sampler_state {
	texture = <tex>;
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

	// スクリーン座標に対して(-0.5, -0.5)のオフセット
	Out.pos /= Out.pos.w;

	Out.pos.x = (Out.pos.x * screenW - 0.5f) / screenW;
	Out.pos.y = (Out.pos.y * screenH + 0.5f) / screenH;

	return Out;
}

//========================================================================================

float hash(float2 p) {
	p = 50.0*frac(p*0.3183099 + float2(0.71, 0.113));
	return -1.0 + 2.0*frac(p.x*p.y*(p.x + p.y));
}

// ノイズを生成する
float noise2(in float2 p) {
	float2 i = floor(p);
	float2 f = frac(p);
	float2 u = f * f * (3.0 - 2.0 * f);
	return lerp(
		lerp(hash(i + float2(0.0, 0.0)),
			hash(i + float2(1.0, 0.0)), u.x),
		lerp(hash(i + float2(0.0, 1.0)),
			hash(i + float2(1.0, 1.0)), u.x), u.y
	);
}

float _noise(in float2 uv) {
	float f = 0.0;
	uv *= 8.0;
	float2x2 m = float2x2(1.6, 1.2, -1.2, 1.6);
	f = 0.5000*noise2(uv);
	uv = mul(m, uv);
	f += 0.2500 * noise2(uv);
	uv = mul(m, uv);
	f += 0.1250 * noise2(uv);
	uv = mul(m, uv);
	f += 0.0625 * noise2(uv);
	uv = mul(m, uv);
	f = 0.5 + 0.5 * f;
	return f;
}

float4 ps_noise(float2 inColor : TEXCOORD0, float4 outColor : COLOR0) : COLOR0{

	float2 p = inColor;// / proj.xy * float2(proj.x / proj.y, 1.0f);
	float3 color = float3(0.1,0.1,0.1);
	p.x += time * 3.0f;
	p.y += time * 3.0f;

	p.x *= 0.01f;
	p.x *= 4.0f;

	float f = _noise(p * 3.0f);

	// ノイズの暗い部分を切り捨てて鮮明にする
	f -= 0.65;
	// 適当に明るくする
	f *= 13.0;

	// 色を 0.0 ～ 1.0 の範囲に直しておく
	f = clamp(f, 0.0, 1.0);

	float3 movie = tex2D(smp, inColor/* / proj.xy*/).xyz;

	outColor.rgb = lerp(movie.xyz, color, float3(f, f, f));
	if (outColor.r < 0.7f)
		outColor.a = 0.3f;
	else
		outColor.a = 0.0f;

	return outColor;
}

float4 ps_fade(float2 inColor : TEXCOORD0, float4 outColor : COLOR0) : COLOR0{
	outColor.rgb = 0.0f;
	outColor.a = alpha;
	return outColor;
}

float4 ps_mosaic(float2 UV : TEXCOORD0) :COLOR0
{
	float4 txl0 = tex2D(smp, UV + float2(0.0f, 0.0f))* 0.2f;
	float4 txl1 = tex2D(smp, UV + float2(-AddU, -AddV))* 0.1f;
	float4 txl2 = tex2D(smp, UV + float2(0.0f, -AddV))* 0.1f;
	float4 txl3 = tex2D(smp, UV + float2(+AddU, -AddV))* 0.1f;
	float4 txl4 = tex2D(smp, UV + float2(-AddU, 0.0f))* 0.1f;
	float4 txl5 = tex2D(smp, UV + float2(+AddU, 0.0f))* 0.1f;
	float4 txl6 = tex2D(smp, UV + float2(-AddU, +AddV))* 0.1f;
	float4 txl7 = tex2D(smp, UV + float2(0.0f, +AddV))* 0.1f;
	float4 txl8 = tex2D(smp, UV + float2(+AddU, +AddV))* 0.1f;
	float4 color = txl0 + txl1 + txl2 + txl3 + txl4 + txl5 + txl6 + txl7 + txl8;
	return color;
}

float4 ps_combo(float4 outColor : COLOR0) : COLOR0
{
	outColor.r = 255.0f / 255.0f;
	outColor.g = 165.0f / 255.0f;
	outColor.b = 0.0f;
	outColor.a = 30.0f / 255.0f;
	return outColor;
}

technique Tech {
	pass p0 {
		VertexShader = compile vs_2_0 vs_main();
		PixelShader = compile ps_3_0 ps_noise();

		AlphaBlendEnable = true;
		SrcBlend = SRCALPHA;
		DestBlend = INVSRCALPHA;
	}

	pass p1 {
		VertexShader = compile vs_2_0 vs_main();
		PixelShader = compile ps_3_0 ps_fade();

		AlphaBlendEnable = true;
		SrcBlend = SRCALPHA;
		DestBlend = INVSRCALPHA;
	}

	pass p2 {
		VertexShader = compile vs_2_0 vs_main();
		PixelShader = compile ps_2_0 ps_mosaic();
	}

	pass p3
	{
		VertexShader = compile vs_2_0 vs_main();
		PixelShader = compile ps_2_0 ps_combo();

		AlphaBlendEnable = true;
		SrcBlend = SRCALPHA;
		DestBlend = INVSRCALPHA;
	}
}