#pragma once

#include <GameObject.h>

class RailField : public GameObject {
public:
	void initialize() override;
	void update() override;

#ifdef _DEBUG
public:
	void debug_draw() const;
#endif // _DEBUG

private:
	Vector3 fieldNormal;
	float distance;
};

