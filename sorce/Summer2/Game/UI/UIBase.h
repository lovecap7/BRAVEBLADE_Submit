#pragma once
#include <memory>

class UIBase abstract :
	public std::enable_shared_from_this<UIBase>
{
public:
	//�D��x
	enum class Priority : int
	{
		Low = 0,	//��
		Middle = 1,	//����
		High = 2,	//��
	};
	//�D��x�擾
	Priority GetPriority()const { return m_priority; };
protected:
	bool m_isDelete;	//�폜
	bool m_isDraw;		//�`�悷�邩
	//�O�ɕ`�悷��
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
	//�`�悷�邩
	void SetIsDraw(bool isDraw) { m_isDraw = isDraw; };
	bool IsDraw()const { return m_isDraw; };
	//�`���O�ɂ���
	bool IsFront()const { return m_isFront; };
};

