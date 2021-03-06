#include "stdafx.h"
#include "Camera.h"
#include "puyo.h"

//コンストラクタ。
Camera::Camera()
{
	Near = 0.1f;
	Far = 1000.0f;
	aspect = 1.0f;
}
//デストラクタ
Camera::~Camera(){}

//カメラの初期化。
void Camera::Init()
{
	vEyePt = D3DXVECTOR3(0.0f, 7.0f, -20.0f);
	vLookatPt = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vUpVec = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

}
 
void Camera::Update()
{

	D3DXMatrixLookAtLH(&viewMatrix, &vEyePt, &vLookatPt, &vUpVec);
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, D3DX_PI / 4, aspect, Near, Far);
}
