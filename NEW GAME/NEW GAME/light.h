#pragma once


const int LIGHT_NUM = 4;

class Light
{
public:
	Light();
	~Light();
	void Init();
	void Update();
	D3DXVECTOR4* GetLightDirection()
	{
		return diffuseLightDirection;
	}
	D3DXVECTOR4* GetLightColor()
	{
		return diffuseLightColor;
	}
	D3DXVECTOR4 GetambientLight()
	{
		return ambientLight;
	}

private:
	D3DXVECTOR4 			diffuseLightDirection[LIGHT_NUM];	//ライトの方向。
	D3DXVECTOR4				diffuseLightColor[LIGHT_NUM];		//ライトの色。
	D3DXVECTOR4				ambientLight;						//環境光

};