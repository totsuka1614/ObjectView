#pragma once

#include "main.h"
#include "Camera.h"

enum SplitMode
{
	NOW_VIEW,
	SIDE_VIEW,
	FRONT_VIEW,
	UP_VIEW,

	MAX_VIEW
};
class CameraSplit : public CCamera
{
public:
	CameraSplit() {};
	~CameraSplit() {};

	void Init();
	void Update() {};
	void SetMode(SplitMode mode);

private:
	SplitMode m_eMode;

};