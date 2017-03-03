/*!
 *@brief	簡単なディフューズライティングのシェーダー。
 */


float4x4 g_worldMatrix;			//ワールド行列。
float4x4 g_viewMatrix;			//ビュー行列。
float4x4 g_projectionMatrix;	//プロジェクション行列。
float4x4 g_rotationMatrix;		//回転行列。法線を回転させるために必要になる。ライティングするなら必須。
float3   g_eyePos;

#define DIFFUSE_LIGHT_NUM	4		//ディフューズライトの数。
float4	g_diffuseLightDirection[DIFFUSE_LIGHT_NUM];	//ディフューズライトの方向。
float4	g_diffuseLightColor[DIFFUSE_LIGHT_NUM];		//ディフューズライトのカラー。
float4	g_ambientLight;								//環境光。

int g_isShadowReciever;				//シャドウレシーバー？１ならシャドウレシーバー。
float4x4 g_lightViewMatrix;			//ライトビュー行列。
float4x4 g_lightProjectionMatrix;	//ライトプロジェクション行列。

//bool g_isHasNormalMap;				//法線マップ保持しているか？
int g_isHasNormalMap;				//法線マップ保持しているか？


texture g_diffuseTexture;		//ディフューズテクスチャ。
sampler g_diffuseTextureSampler = 
sampler_state
{
	Texture = <g_diffuseTexture>;
    MipFilter = NONE;
    MinFilter = NONE;
    MagFilter = NONE;
    AddressU = Wrap;
	AddressV = Wrap;
};

texture g_shadowMapTexture;		//シャドウマップテクスチャ。
sampler g_shadowMapTextureSampler =
sampler_state
{
	Texture = <g_shadowMapTexture>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = CLAMP;
	AddressV = CLAMP;
};

texture g_normalTexture;			//法線マップ
sampler g_normalMapSampler =
sampler_state
{
	Texture = <g_normalTexture>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

struct VS_INPUT{
	float4	pos		: POSITION;
	float4	color	: COLOR0;
	float3	normal	: NORMAL0;
	float2	uv		: TEXCOORD0;
	float3  tangent : TANGENT;
	float3  tex0	: TEXCOORD1;
};

struct VS_OUTPUT{
	float4	pos				: POSITION;
	float4	color			: COLOR0;
	float2	uv				: TEXCOORD0;
	float3	normal			: TEXCOORD1;
	float4	worldPos		: TEXCOORD2;
	float4	lightViewPos 	: TEXCOORD3;		//ワールド空間->ライトビュー空間->ライト射影空間に変換された座標。
	float3	tangent			: TEXCOORD4;
	float3  tex0			: TEXCOORD5;
};

/*!
 *@brief	頂点シェーダー。
 */

VS_OUTPUT VSMain( VS_INPUT In )
{
	VS_OUTPUT Out = (VS_OUTPUT)0;
	float4 pos;
	float3 normal, tangent;
	pos = mul( In.pos, g_worldMatrix );		//モデルのローカル空間からワールド空間に変換。
	Out.worldPos = pos;
	pos = mul( pos, g_viewMatrix );			//ワールド空間からビュー空間に変換。
	pos = mul( pos, g_projectionMatrix );	//ビュー空間から射影空間に変換。
	normal = mul(In.normal, g_rotationMatrix);
	tangent = mul(In.tangent, g_rotationMatrix);
	if (g_isShadowReciever == 1)
	{
		Out.lightViewPos = mul(Out.worldPos, g_lightViewMatrix);
		Out.lightViewPos = mul(Out.lightViewPos, g_lightProjectionMatrix);
	}
	Out.pos = pos;
	Out.color = In.color;
	Out.uv = In.uv;
	Out.normal = normalize(normal);
	Out.tangent = normalize(tangent);
	Out.tex0 = In.tex0;

	return Out;
}
// ディフューズ算出。
float4 DiffuseLight(float3 normal)
{
	float4 color = 0.0f;
	color += max(0, -dot(normal, g_diffuseLightDirection[0])) * g_diffuseLightColor[0];
	color += max(0, -dot(normal, g_diffuseLightDirection[1])) * g_diffuseLightColor[1];
	color += max(0, -dot(normal, g_diffuseLightDirection[2])) * g_diffuseLightColor[2];
	color += max(0, -dot(normal, g_diffuseLightDirection[3])) * g_diffuseLightColor[3];
	color.a = 1.0f;
	return color;
}
// スペキュラ算出。
// 引数：ワールド座標系での頂点ポジション。
//		 法線。
float3 SpecLight(float3 WorldPos, float3 Normal){
	float3 eye = normalize(g_eyePos - WorldPos);
	float3 R = -eye + 2.0f*dot(Normal, eye)*Normal;
	float3 spec = (float3)(0);
	for (int i = 0; i < DIFFUSE_LIGHT_NUM; i++){
		// スペキュラ算出時にテカリの強度とオブジェクトの色を加味する。
		spec += g_diffuseLightColor[i].xyz * pow(max(0.0f, dot(R, -g_diffuseLightDirection[i])), 2.0f) * g_diffuseLightColor[i].a;
	}
	return spec;
}

/*!
 *@brief		ピクセルシェーダー。
 */
float4 PSMain( VS_OUTPUT In ) : COLOR
{
	//ライトを計算。
	float3 normal;

	if (g_isHasNormalMap != 0)
	{
		// 法線マップがある。
		float3 tangent = normalize(In.tangent);
		float3 binSpaceNormal = tex2D(g_normalMapSampler, In.tex0);
		float4x4 tangentSpaceMatrix;
		//法線とタンジェントから従法線を求める
		float3 biNormal = normalize(cross(tangent, In.normal));
		//タンジェントスペースからワールドスペースに変換する行列を求める。
		tangentSpaceMatrix[0] = float4(tangent, 0.0f);
		tangentSpaceMatrix[1] = float4(biNormal, 0.0f);
		tangentSpaceMatrix[2] = float4(In.normal, 0.0f);
		tangentSpaceMatrix[3] = float4(0.0f, 0.0f, 0.0f, 1.0f);
		//-1.0～1.0の範囲にマッピングする。
		binSpaceNormal = (binSpaceNormal * 2.0f) - 1.0f;
		//タンジェントスペースからワールドスペースの法線に変換する。
		normal = tangentSpaceMatrix[0] * binSpaceNormal.x + tangentSpaceMatrix[1] * binSpaceNormal.y + tangentSpaceMatrix[2] * binSpaceNormal.z;
	}
	else{
		normal = normalize(In.normal);
	}


	float4 lig = 0.0f;	// 最終的なライトの色。
	// ディフューズ算出。
	lig = DiffuseLight(normal);
	// スペキュラ算出。
	lig.xyz += SpecLight(In.worldPos, normal);
	// 環境光(アンビエント)加算。
	lig.xyz += g_ambientLight.xyz;

	float4 color = tex2D( g_diffuseTextureSampler, In.uv );
	if (g_isShadowReciever == 1){
		//射影空間(スクリーン座標系)に変換された座標はw成分で割ってやると(-1.0f～1.0)の範囲の正規化座標系になる。
		//これをUV座標系(0.0～1.0)に変換して、シャドウマップをフェッチするためのUVとして活用する。
		float2 shadowMapUV = In.lightViewPos.xy / In.lightViewPos.w;	//この計算で(-1.0～1.0)の範囲になる。
		shadowMapUV *= float2(0.5f, -0.5f);								//この計算で(-0.5～0.5)の範囲になる。
		shadowMapUV += float2(0.5f, 0.5f);								//そしてこれで(0.0～1.0)の範囲になってＵＶ座標系に変換できた。やったね。
		float4 shadowVal = tex2D(g_shadowMapTextureSampler, shadowMapUV);	//シャドウマップは影が落ちているところはグレースケールになっている。
		color *= shadowVal;
	}

	color.xyz *= lig;

	return color;
}

float4 PSRenderToShadowMapMain(VS_OUTPUT In):COLOR
{
	return float4(0.5f, 0.5, 0.5f, 1.0f);
}

technique SkinModel
{
	pass p0
	{
		VertexShader 	= compile vs_3_0 VSMain();
		PixelShader 	= compile ps_3_0 PSMain();
	}
}

technique SkinModelRenderToShadowMap
{
	pass p0
	{
		VertexShader	= compile vs_3_0 VSMain();
		PixelShader		= compile ps_3_0 PSRenderToShadowMapMain();
	}
};