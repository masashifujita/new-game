#pragma once


/*!
* @brief	�Q�[���N���X�B
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