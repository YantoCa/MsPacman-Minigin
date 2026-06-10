#pragma once
#include "Game.h"

class MsPacman final : public Game {
public:
	MsPacman() = default;
	~MsPacman() override = default;

	void Initialize() override;
};