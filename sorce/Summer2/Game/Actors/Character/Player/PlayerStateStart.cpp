#include "PlayerStateStart.h"
#include "Player.h"
#include "PlayerStateIdle.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collision/Collidable.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../Game/UI/UIManager.h"
namespace
{
	//������
	constexpr float kMoveDeceRate = 0.8f;
	//�S�̃t���[��
	constexpr float kTotalAnimFrame = 37.0f;
	//�A�j���[�V����
	const char* kAnim = "Player|Ult";
}

PlayerStateStart::PlayerStateStart(std::weak_ptr<Actor> player) :
	PlayerStateBase(player)
{
	//�X�^�[�g���
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	coll->GetModel()->SetAnim(kAnim, false);
	coll->SetCollState(CollisionState::Normal);
	//�S�̃t���[���ݒ�
	coll->GetModel()->SetTotalAnimFrame(kTotalAnimFrame);
	//UI��\��
	UIManager::GetInstance().StopDraw();
	UIManager::GetInstance().StopUpdate();
}

PlayerStateStart::~PlayerStateStart()
{
	//UI�\��
	UIManager::GetInstance().StartDraw();
	UIManager::GetInstance().StartUpdate();
}

void PlayerStateStart::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
}

void PlayerStateStart::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//���͂��~�߂�
	auto& input = Input::GetInstance();
	input.StopUpdate();
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	auto model = coll->GetModel();
	//���f���̃A�j���[�V�������I�������
	if (model->IsFinishAnim())
	{
		//���͂��J�n
		input.StartUpdate();
		//�ҋ@
		ChangeState(std::make_shared<PlayerStateIdle>(m_owner));
		return;
	}
	if (model->GetNowAnimFrame() == model->GetTotalAnimFrame() / 2.0f)
	{
		//VC
		coll->CharacterVC("Start");
	}
	//���񂾂񌸑�
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}
