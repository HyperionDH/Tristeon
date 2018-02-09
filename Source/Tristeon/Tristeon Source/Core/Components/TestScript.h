﻿#pragma once
#include "Component.h"
#include "Editor/TypeRegister.h"
#include "Math/Vector3.h"

using namespace Tristeon;
using namespace Core::Components;

class TestScript : public Component
{
public:
	void update() override;
	nlohmann::json serialize() override;
	void deserialize(nlohmann::json json) override;

private:
	static DerivedRegister<TestScript> reg;
	float speed = 0;
};
