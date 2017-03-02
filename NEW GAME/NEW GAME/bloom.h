#pragma once
#include "rendertarget.h"
#include "system.h"

class Bloom
{
public:
	Bloom();
	~Bloom();
	void Init(const char* model, LPDIRECT3DDEVICE9 pd3dDevice);
	void Init();
	void Render();
	void UpdateWeight(float dispersion);
private:
	LPD3DXEFFECT		effect;
	CRenderTarget		luminanceRenderTarget;
	CRenderTarget		downSamplingRenderTarget[2];
	static const int	NUM_WEIGHTS = 8;
	float				weights[NUM_WEIGHTS];
};