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


struct VS_INPUT{
	float4	pos		: POSITION;
	float4	color	: COLOR0;
	float3	normal	: NORMAL0;
	float2	uv		: TEXCOORD0;
};

struct VS_OUTPUT{
	float4	pos		: POSITION;
	float4	color	: COLOR0;
	float2	uv		: TEXCOORD0;
	float3	normal	: TEXCOORD1;
	float4	worldPos: TEXCOORD2;
	float4	lightViewPos 	: TEXCOORD3;		//ワールド空間->ライトビュー空間->ライト射影空間に変換された座標。

};

/*!
 *@brief	頂点シェーダー。
 */
VS_OUTPUT VSMain( VS_INPUT In )
{
	VS_OUTPUT Out;
	float4 pos; 
	pos = mul( In.pos, g_worldMatrix );		//モデルのローカル空間からワールド空間に変換。
	Out.worldPos = pos;
	pos = mul( pos, g_viewMatrix );			//ワールド空間からビュー空間に変換。
	pos = mul( pos, g_projectionMatrix );	//ビュー空間から射影空間に変換。
	if (g_isShadowReciever == 1)
	{
		Out.lightViewPos = mul(Out.worldPos, g_lightViewMatrix);
		Out.lightViewPos = mul(Out.lightViewPos, g_lightProjectionMatrix);
	}
	Out.pos = pos;
	Out.color = In.color;
	Out.uv = In.uv;
	Out.normal = mul(In.normal, g_rotationMatrix);	//法線を回す。

	return Out;
}
/*!
 *@brief	頂点シェーダー。
 */
float4 PSMain( VS_OUTPUT In ) : COLOR
{
	//ライトを計算。
	float4 lig = 0.0f;
	{
		for( int i = 0; i < DIFFUSE_LIGHT_NUM; i++ ){
			lig.xyz += max( 0.0f, dot(In.normal.xyz, -g_diffuseLightDirection[i].xyz) ) 
					* g_diffuseLightColor[i].xyz;

			float3 eye = normalize(g_eyePos - In.worldPos);
			float3 R = -eye + 2.0f*dot(In.normal, eye)*In.normal;
			float3 spec = max(0.0f, dot(R, -g_diffuseLightDirection[i]));
			spec = pow(spec, 2.0f);

			lig.xyz += spec;
		}

		lig += g_ambientLight;
	}
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
		VertexShader 	= compile vs_2_0 VSMain();
		PixelShader 	= compile ps_2_0 PSMain();
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