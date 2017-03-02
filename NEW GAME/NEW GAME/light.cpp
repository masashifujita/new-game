#include "stdafx.h"
#include "light.h"

Light::Light(){}

Light::~Light(){}

void Light::Init()
{
	//ライトの初期化
	ZeroMemory(diffuseLightDirection, sizeof(diffuseLightDirection));
	ZeroMemory(diffuseLightColor, sizeof(diffuseLightColor));
}

void Light::Update()
{
	diffuseLightDirection[0] = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
	diffuseLightDirection[1] = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
	diffuseLightDirection[2] = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
	diffuseLightDirection[3] = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);

	//ディフューズライト。
	diffuseLightColor[0] = D3DXVECTOR4(10.2f, 10.2f, 0.2f, 1.0f);
	diffuseLightColor[1] = D3DXVECTOR4(0.2f, 0.2f, 0.2f, 1.0f);
	diffuseLightColor[2] = D3DXVECTOR4(0.3f, 0.3f, 0.3f, 1.0f);
	diffuseLightColor[3] = D3DXVECTOR4(0.2f, 0.2f, 0.2f, 1.0f);

	//環境光。
	ambientLight = D3DXVECTOR4(0.3f, 0.3f, 0.3f, 1.0f);
}