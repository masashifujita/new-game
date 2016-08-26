#pragma once

class Unity;

/*!
*@brief	カメラクラス。
*/
class Camera {
public:
	Camera();
	~Camera();
	//アスペクト比を設定。
	void SetAspect(float _aspect)
	{
		aspect = _aspect;
	}
	//アスペクト比を取得。
	float GetAspect()
	{
		return aspect;
	}
	//ファーを設定。
	void SetFar(float _far)
	{
		Far = _far;
	}
	//ニアーの取得。
	float GetNear()
	{
		return Near;
	}
	//ファーの取得。
	float GetFar()
	{
		return Far;
	}
	//ニアーを設定。
	void SetNear(float _near)
	{
		Near = _near;
	}
	//視点のセッター
	void SetEyePt(D3DXVECTOR3 pt)
	{
		vEyePt = pt;
	}
	//視点のゲッター
	D3DXVECTOR3 GetEyePt()
	{
		return vEyePt;
	}
	//注視点のセッター
	void SetLookatPt(D3DXVECTOR3 pt)
	{
		vLookatPt = pt;
	}
	//注視点のゲッター。
	D3DXVECTOR3 GetLookatPt()
	{
		return vLookatPt;
	}
	//上方向のセッター
	void SetUpVec(D3DXVECTOR3 up)
	{
		vUpVec = up;
	}
	//上方向のゲッター
	D3DXVECTOR3 GetUpVec()
	{
		return vUpVec;
	}
	//ビュー行列のセッター。
	void SetViewMatrix(D3DXMATRIX mView)
	{
		viewMatrix = mView;
	}
	//ビュー行列のゲッター
	D3DXMATRIX GetViewMatrix()
	{
		return viewMatrix;
	}
	//プロジェクション行列のセッター
	void SetProjectionMatrix(D3DXMATRIX mProj)
	{
		projectionMatrix = mProj;
	}
	//プロジェクション行列のゲッター
	D3DXMATRIX GetProjectionMatrix()
	{
		return projectionMatrix;
	}
	void Update();
	void Init(Unity* unity);
private:
	D3DXMATRIX				viewMatrix;			//!<ビュー行列。カメラ行列とも言う。
	D3DXMATRIX				projectionMatrix;	//!<プロジェクション行列。ビュー空間から射影空間に変換する行列。
	D3DXVECTOR3				vEyePt;				//!<カメラの視点。この辺りはメンバ変数に変更する。
	D3DXVECTOR3				vLookatPt;			//!<カメラの注視点。どこを見ているかという情報。この辺りはメンバ変数に変更する。
	D3DXVECTOR3				vUpVec;				//!<カメラの上方向。基本的にY方向でかまわない。基本的には・・・。この辺りはメンバ変数に変更する。
	float					Far;				//!<遠平面。
	float					Near;				//!<近平面。
	float					aspect;				//!<アスペクト比
	Unity*					unity;
	D3DXMATRIX				mWorld;			//ワールド行列。
};