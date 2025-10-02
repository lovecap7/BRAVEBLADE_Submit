#pragma once
#include "../General/Math/Vector3.h"
#include <memory>
class Model;
class StageFigure
{
public:
	StageFigure();
	~StageFigure();
	void Init(int handle,Vector3 pos);
	void Update();
	void Draw()const;
	void End();
private:	
	//ƒ‚ƒfƒ‹
	std::shared_ptr<Model> m_model;
};

