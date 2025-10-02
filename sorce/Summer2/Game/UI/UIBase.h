#pragma once
#include <memory>

class UIBase abstract :
	public std::enable_shared_from_this<UIBase>
{
public:
	//—Dæ“x
	enum class Priority : int
	{
		Low = 0,	//’á
		Middle = 1,	//•’Ê
		High = 2,	//‚
	};
	//—Dæ“xæ“¾
	Priority GetPriority()const { return m_priority; };
protected:
	bool m_isDelete;	//íœ
	bool m_isDraw;		//•`‰æ‚·‚é‚©
	//‘O‚É•`‰æ‚·‚é
	bool m_isFront;
	Priority m_priority;
public:
	UIBase(bool isFront = true, Priority priority = UIBase::Priority::Middle);
	virtual~UIBase();
	virtual void Init() ;
	virtual void Update()abstract;
	virtual void Draw()const abstract;
	virtual void End();
	bool IsDelete() { return m_isDelete; };
	void Delete() { m_isDelete = true; };
	//•`‰æ‚·‚é‚©
	void SetIsDraw(bool isDraw) { m_isDraw = isDraw; };
	bool IsDraw()const { return m_isDraw; };
	//•`‰æ‚ğ‘O‚É‚·‚é
	bool IsFront()const { return m_isFront; };
};

