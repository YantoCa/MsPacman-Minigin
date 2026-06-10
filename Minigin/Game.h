#pragma once
 
class Game {
public:
	Game() = default;
	virtual ~Game() = default;

	virtual void Initialize() = 0;
	virtual void Update(float) {}
	virtual void Render() {};
};