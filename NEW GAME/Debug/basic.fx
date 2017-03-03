/*!
 *@brief	�ȒP�ȃf�B�t���[�Y���C�e�B���O�̃V�F�[�_�[�B
 */


float4x4 g_worldMatrix;			//���[���h�s��B
float4x4 g_viewMatrix;			//�r���[�s��B
float4x4 g_projectionMatrix;	//�v���W�F�N�V�����s��B
float4x4 g_rotationMatrix;		//��]�s��B�@������]�����邽�߂ɕK�v�ɂȂ�B���C�e�B���O����Ȃ�K�{�B
float3   g_eyePos;

#define DIFFUSE_LIGHT_NUM	4		//�f�B�t���[�Y���C�g�̐��B
float4	g_diffuseLightDirection[DIFFUSE_LIGHT_NUM];	//�f�B�t���[�Y���C�g�̕����B
float4	g_diffuseLightColor[DIFFUSE_LIGHT_NUM];		//�f�B�t���[�Y���C�g�̃J���[�B
float4	g_ambientLight;								//�����B

int g_isShadowReciever;				//�V���h�E���V�[�o�[�H�P�Ȃ�V���h�E���V�[�o�[�B
float4x4 g_lightViewMatrix;			//���C�g�r���[�s��B
float4x4 g_lightProjectionMatrix;	//���C�g�v���W�F�N�V�����s��B

//bool g_isHasNormalMap;				//�@���}�b�v�ێ����Ă��邩�H
int g_isHasNormalMap;				//�@���}�b�v�ێ����Ă��邩�H


texture g_diffuseTexture;		//�f�B�t���[�Y�e�N�X�`���B
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

texture g_shadowMapTexture;		//�V���h�E�}�b�v�e�N�X�`���B
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

texture g_normalTexture;			//�@���}�b�v
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
	float4	lightViewPos 	: TEXCOORD3;		//���[���h���->���C�g�r���[���->���C�g�ˉe��Ԃɕϊ����ꂽ���W�B
	float3	tangent			: TEXCOORD4;
	float3  tex0			: TEXCOORD5;
};

/*!
 *@brief	���_�V�F�[�_�[�B
 */

VS_OUTPUT VSMain( VS_INPUT In )
{
	VS_OUTPUT Out = (VS_OUTPUT)0;
	float4 pos;
	float3 normal, tangent;
	pos = mul( In.pos, g_worldMatrix );		//���f���̃��[�J����Ԃ��烏�[���h��Ԃɕϊ��B
	Out.worldPos = pos;
	pos = mul( pos, g_viewMatrix );			//���[���h��Ԃ���r���[��Ԃɕϊ��B
	pos = mul( pos, g_projectionMatrix );	//�r���[��Ԃ���ˉe��Ԃɕϊ��B
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
// �f�B�t���[�Y�Z�o�B
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
// �X�y�L�����Z�o�B
// �����F���[���h���W�n�ł̒��_�|�W�V�����B
//		 �@���B
float3 SpecLight(float3 WorldPos, float3 Normal){
	float3 eye = normalize(g_eyePos - WorldPos);
	float3 R = -eye + 2.0f*dot(Normal, eye)*Normal;
	float3 spec = (float3)(0);
	for (int i = 0; i < DIFFUSE_LIGHT_NUM; i++){
		// �X�y�L�����Z�o���Ƀe�J���̋��x�ƃI�u�W�F�N�g�̐F����������B
		spec += g_diffuseLightColor[i].xyz * pow(max(0.0f, dot(R, -g_diffuseLightDirection[i])), 2.0f) * g_diffuseLightColor[i].a;
	}
	return spec;
}

/*!
 *@brief		�s�N�Z���V�F�[�_�[�B
 */
float4 PSMain( VS_OUTPUT In ) : COLOR
{
	//���C�g���v�Z�B
	float3 normal;

	if (g_isHasNormalMap != 0)
	{
		// �@���}�b�v������B
		float3 tangent = normalize(In.tangent);
		float3 binSpaceNormal = tex2D(g_normalMapSampler, In.tex0);
		float4x4 tangentSpaceMatrix;
		//�@���ƃ^���W�F���g����]�@�������߂�
		float3 biNormal = normalize(cross(tangent, In.normal));
		//�^���W�F���g�X�y�[�X���烏�[���h�X�y�[�X�ɕϊ�����s������߂�B
		tangentSpaceMatrix[0] = float4(tangent, 0.0f);
		tangentSpaceMatrix[1] = float4(biNormal, 0.0f);
		tangentSpaceMatrix[2] = float4(In.normal, 0.0f);
		tangentSpaceMatrix[3] = float4(0.0f, 0.0f, 0.0f, 1.0f);
		//-1.0�`1.0�͈̔͂Ƀ}�b�s���O����B
		binSpaceNormal = (binSpaceNormal * 2.0f) - 1.0f;
		//�^���W�F���g�X�y�[�X���烏�[���h�X�y�[�X�̖@���ɕϊ�����B
		normal = tangentSpaceMatrix[0] * binSpaceNormal.x + tangentSpaceMatrix[1] * binSpaceNormal.y + tangentSpaceMatrix[2] * binSpaceNormal.z;
	}
	else{
		normal = normalize(In.normal);
	}


	float4 lig = 0.0f;	// �ŏI�I�ȃ��C�g�̐F�B
	// �f�B�t���[�Y�Z�o�B
	lig = DiffuseLight(normal);
	// �X�y�L�����Z�o�B
	lig.xyz += SpecLight(In.worldPos, normal);
	// ����(�A���r�G���g)���Z�B
	lig.xyz += g_ambientLight.xyz;

	float4 color = tex2D( g_diffuseTextureSampler, In.uv );
	if (g_isShadowReciever == 1){
		//�ˉe���(�X�N���[�����W�n)�ɕϊ����ꂽ���W��w�����Ŋ����Ă���(-1.0f�`1.0)�͈̔͂̐��K�����W�n�ɂȂ�B
		//�����UV���W�n(0.0�`1.0)�ɕϊ����āA�V���h�E�}�b�v���t�F�b�`���邽�߂�UV�Ƃ��Ċ��p����B
		float2 shadowMapUV = In.lightViewPos.xy / In.lightViewPos.w;	//���̌v�Z��(-1.0�`1.0)�͈̔͂ɂȂ�B
		shadowMapUV *= float2(0.5f, -0.5f);								//���̌v�Z��(-0.5�`0.5)�͈̔͂ɂȂ�B
		shadowMapUV += float2(0.5f, 0.5f);								//�����Ă����(0.0�`1.0)�͈̔͂ɂȂ��Ăt�u���W�n�ɕϊ��ł����B������ˁB
		float4 shadowVal = tex2D(g_shadowMapTextureSampler, shadowMapUV);	//�V���h�E�}�b�v�͉e�������Ă���Ƃ���̓O���[�X�P�[���ɂȂ��Ă���B
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