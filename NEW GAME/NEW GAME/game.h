#pragma once


/*!
* @brief	ゲームクラス。
*/
class Game{
public:
	Game();

	~Game();

	void Start();

	void Update();

	void Render();
private:
	Camera camera;
	Light light;
};

extern Game* game;