#include "particleemitter.h"
#include "particle.h"

extern LPDIRECT3DDEVICE9 g_pd3dDevice;

using namespace std;
CParticleEmitter::CParticleEmitter()
{

}
CParticleEmitter::~CParticleEmitter()
{
}
void CParticleEmitter::Init(const SParicleEmitParameter& param)
{
	this->param = param;
	timer = 0.0f;
}
void CParticleEmitter::Update()
{
	if (timer >= param.intervalTime) {
		//パーティクルを生成。
		CParticle* p = new CParticle;
		p->Init(param);
		timer = 0.0f;
		particleList.push_back(p);
	}
	timer += 1.0f / 60.0f;
	auto it = particleList.begin();
	while (it != particleList.end())
	{
		if (!(*it)->Update())
		{
			delete *it;
			it = particleList.erase(it);
		}
		else
		{
			it++;
		}
	}
	for (auto p : particleList) {
		p->Update();
	}
}
void CParticleEmitter::Render(const D3DXMATRIX& viewMatrix, const D3DXMATRIX& projMatrix)
{
	for (auto p : particleList) {
		p->Render(viewMatrix, projMatrix);
	}
}
