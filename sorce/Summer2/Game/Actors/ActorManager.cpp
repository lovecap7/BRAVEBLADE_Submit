#include "ActorManager.h"
#include "GroupManager.h"
#include "SearchPlace.h"
#include "../../General/Rigidbody.h"
#include "../../General/Math/MyMath.h"
#include "../UI/UIManager.h"
#include "../GameRule/Score.h"
#include "../../General/Collision/Physics.h"
#include "../../General/HitPoints.h"
#include "../../General/AttackPoints.h"
#include "../../General/Effect/EffekseerManager.h"
#include "../../General/StringUtil.h"
#include <DxLib.h>
#include <cassert>
//配置データ
#include "../../General/CSVDataLoader.h"
//アクター
#include "Actor.h"
//プレイヤー
#include "Character/Player/Player.h"
//敵
#include "Character/Enemy/PurpleDinosaur/PurpleDinosaur.h"
#include "Character/Enemy/SmallDragon/SmallDragon.h"
#include "Character/Enemy/BossDragon/BossDragon.h"
#include "Character/Enemy/BossMuscle/BossMuscle.h"
#include "Character/Enemy/BossKing/BossKing.h"
#include "Character/Enemy/Bomber/Bomber.h"
#include "Character/Enemy/EnemyBase.h"
//ステージ
#include "Stage/InvisibleWall.h"
#include "Stage/StageObjectCollision.h"
#include "Stage/StageObjectDraw.h"
#include "Stage/Sky.h"
#include "Stage/EventAreaBase.h"
#include "Stage/ZMoveCameraArea.h"
#include "Stage/AllKillArea.h"
#include "Stage/BossArea.h"
//アイテム
#include "Item/Heart.h"
#include "Item/UltGageUp.h"
#include "Item/Bomb.h"
#include "Item/AttackUp.h"
#include "Item/DefenseUp.h"
//攻撃
#include "Attack/Slash.h"
#include "Attack/Strike.h"
#include "Attack/AreaOfEffectAttack.h"
#include "Attack/Bullet.h"
#include "Attack/Blast.h"
#include "Attack/Breath.h"
#include "Attack/ULT.h"
#include "Attack/WaveAttack.h"
#include "Attack/HomingBullet.h"

namespace
{
	//ボスが完全に消えるまでのフレーム数
	constexpr int kBossDisappearFrameMax = 60;
	//エリアを構成するパーツの数
	constexpr int kAreaPartsNum = 2;
	//アイテム生成確率
	constexpr int kCreateItemRandRate = 10;
	//アイテムの生成上限
	constexpr int kCreateItemMax = 10;
}

ActorManager::ActorManager(std::weak_ptr<GameCamera> camera):
	m_actorId(0),
	m_camera(camera),
	m_isUpdate(true),
	m_delayFrame(0),
	m_bossDisappearFrame(0)
{
	//CSV
	m_csvLoader = std::unique_ptr<CSVDataLoader>();
	//グループマネージャー
	m_groupManager = std::make_shared<GroupManager>();
	m_groupManager->Init();
	//ハンドルロード
	LoadHandle();
	//攻撃の情報を作成
	CreateAttackData();
}

ActorManager::~ActorManager()
{
}

//アクターを追加
void ActorManager::Entry(std::shared_ptr<Actor> actor)
{
	//すでに登録されているならしない
	auto it = std::find(m_actors.begin(), m_actors.end(), actor);
	if (it != m_actors.end())return;
	//アクターの初期化
	actor->Init();
	//アクターのIDを設定
	actor->SetID(m_actorId);
	m_actorId++;
	//アクターを追加
	m_actors.emplace_back(actor);
	//グループタグがあるなら
	if (actor->GetGroupTag() != "Untagged")
	{
		//登録
		m_groupManager->Entry(actor);
	}
}

void ActorManager::Exit(std::shared_ptr<Actor> actor)
{
	//登録されていないならしない
	auto it = std::find(m_actors.begin(), m_actors.end(), actor);
	if (it == m_actors.end())return;
	//グループに登録されている場合は解除
	m_groupManager->Exit(actor);
	actor->End();
	m_actors.erase(it);
}


void ActorManager::Init(Stage::StageIndex index)
{
	m_stageIndex = index;
	//ステージ情報をロード
	LoadStage(m_stageIndex);
	//アクターを実装
	CheckNextAddActors();
}

void ActorManager::Update(const std::weak_ptr<Score> score)
{
	//遅延処理
	UpdateDelay();
	//更新をしないなら
	if (!m_isUpdate || m_delayFrame > 0)return;
	//新規アクターの追加
	CheckNextAddActors();
	//グループ処理
	m_groupManager->Update();
	//アクターの更新
	for (auto& actor : m_actors)
	{
		actor->Update(m_camera,shared_from_this());
	}
	//ボスが倒されたとき
	if (IsBossDead())
	{
		//攻撃をすべて削除
		AllDeleteAttack();
		//消滅したとき
		if (m_boss.expired())
		{
			//経過フレームを増やす
			m_bossDisappearFrame++;
			//ボスが完全に消えるまでのフレーム数を超えたら
			if (IsBossDisappear())
			{
				//プレイヤーの体力からスコアを加算
				score.lock()->AddHPScore(m_player.lock()->GetHitPoints());
			}
		}
	}
	//消滅フラグチェック
	CheckDeleteActors(score);
}

void ActorManager::UpdateDelay()
{
	if (m_delayFrame > 0)
	{
		--m_delayFrame;
		if (m_delayFrame <= 0)
		{
			m_isUpdate = true;
		}
	}
}

void ActorManager::Draw() const
{
	//アクターの描画
	for (auto& actor : m_actors)
	{
#if _DEBUG
		//攻撃が可能かどうかのデバッグ表示
		if (actor->GetGameTag() == GameTag::Enemy)
		{
			//攻撃の権利を持っているなら
			if (actor->CanAttack())
			{
				std::dynamic_pointer_cast<EnemyBase>(actor)->GetModel()->SetColor(1, 1, 1, 1);
			}
			else
			{
				std::dynamic_pointer_cast<EnemyBase>(actor)->GetModel()->SetColor(0, 0, 0, 1);
			}
		}
#endif
		actor->Draw();
	}
}

void ActorManager::End()
{
	//グループ削除
	m_groupManager->End();
	//ハンドルをすべて削除
	AllDeleteHandles();
	//メモリを解放
	AllDeleteActors();
}

void ActorManager::Restart()
{
	//グループ削除
	m_groupManager->End();
	//初期化
	m_groupManager->Init();
	//メモリを解放
	AllDeleteActors();
	//再配置
	Init(m_stageIndex);
}


//新規アクターの追加予定を受け取る(public)
void ActorManager::AddNextActor(std::shared_ptr<Actor> actor)
{
	//追加予定のアクターを追加
	m_nextAddActors.emplace_back(actor);
}

std::weak_ptr<CharacterBase> ActorManager::CreateCharacter(ActorData charaData)
{
	//キャラクターを作成
	std::shared_ptr<CharacterBase> chara;
	if (charaData.name == "Player")
	{
		chara = std::make_shared<Player>(MV1DuplicateModel(m_handles[charaData.name]), charaData.pos);
		//プレイヤーの参照
		m_player = std::dynamic_pointer_cast<Player>(chara);
	}
	else if (charaData.name == "PurpleDinosaur")
	{
		chara = std::make_shared<PurpleDinosaur>(MV1DuplicateModel(m_handles[charaData.name]), charaData.pos);
	}
	else if (charaData.name == "SmallDragon")
	{
		chara = std::make_shared<SmallDragon>(MV1DuplicateModel(m_handles[charaData.name]), charaData.pos);
	}
	else if (charaData.name == "Bomber")
	{
		chara = std::make_shared<Bomber>(MV1DuplicateModel(m_handles[charaData.name]), charaData.pos);
	}
	else if (charaData.name == "BossDragon")
	{
		chara = std::make_shared<BossDragon>(MV1DuplicateModel(m_handles[charaData.name]), charaData.pos);
		m_boss = std::dynamic_pointer_cast<EnemyBase>(chara);
	}
	else if (charaData.name == "BossMuscle")
	{
		chara = std::make_shared<BossMuscle>(MV1DuplicateModel(m_handles[charaData.name]), charaData.pos);
		m_boss = std::dynamic_pointer_cast<EnemyBase>(chara);
	}
	else if (charaData.name == "BossKing")
	{
		chara = std::make_shared<BossKing>(MV1DuplicateModel(m_handles[charaData.name]), MV1DuplicateModel(m_handles["BossKing2"]), charaData.pos);
		m_boss = std::dynamic_pointer_cast<EnemyBase>(chara);
	}
	//各項目設定
	if (chara)
	{
		chara->SetCharaData(charaData);
	}
	//キャラクターを入れる
	AddNextActor(chara);
	return chara;
}
std::weak_ptr<AttackBase> ActorManager::CreateAttack(AttackType at, std::weak_ptr<Actor> owner)
{
	//攻撃を作成
	std::shared_ptr<AttackBase> attack;
	switch (at)
	{
	case AttackType::Slash:
		attack = std::make_shared<Slash>(owner);
		break;
	case AttackType::Strike:
		attack = std::make_shared<Strike>(owner);
		break;
	case AttackType::AreaOfEffect:
		attack = std::make_shared<AreaOfEffectAttack>(owner);
		break;
	case AttackType::Bullet:
		attack = std::make_shared<Bullet>(owner);
		break;
	case AttackType::Blast:
		attack = std::make_shared<Blast>(owner);
		break;
	case AttackType::Breath:
		attack = std::make_shared<Breath>(owner);
		break;
	case AttackType::ULT:
		attack = std::make_shared<ULT>(owner);
		break;
	case AttackType::Wave:
		attack = std::make_shared<WaveAttack>(owner);
		break;
	case AttackType::Homing:
		attack = std::make_shared<HomingBullet>(owner);
		break;
	default:
		break;
	}
	//攻撃を入れる
	AddNextActor(attack);
	return attack;
}

std::weak_ptr<ItemBase> ActorManager::CreateItem(ItemType it, Vector3 pos)
{
	//攻撃を作成
	std::shared_ptr<ItemBase> item;
	switch (it)
	{
	case ItemType::Heart:
		item = std::make_shared<Heart>(MV1DuplicateModel(m_handles["Heart"]), pos);
		break;
	case ItemType::UltGageUp:
		item = std::make_shared<UltGageUp>(MV1DuplicateModel(m_handles["UltGageUp"]), pos);
		break;
	case ItemType::AttackUp:
		item = std::make_shared<AttackUp>(MV1DuplicateModel(m_handles["AttackUp"]), pos);
		break;
	case ItemType::DefenseUp:
		item = std::make_shared<DefenseUp>(MV1DuplicateModel(m_handles["DefenseUp"]), pos);
		break;
	case ItemType::Bomb:
		item = std::make_shared<Bomb>(MV1DuplicateModel(m_handles["Bomb"]), pos);
		break;
	default:
		item = std::make_shared<Heart>(MV1DuplicateModel(m_handles["Heart"]), pos);
		break;
	}
	//アイテムを入れる
	AddNextActor(item);
	return item;
}

void ActorManager::CreateRandItem(Vector3 pos)
{
	//アイテムの数が多すぎた時はこれ以上作らない
	int itemCount = 0;
	for(auto& actor : m_actors)
	{
		if (actor->GetGameTag() == GameTag::Item)
		{
			++itemCount;
			if (itemCount >= kCreateItemMax)return;
		}
	}
	//1/kCreateItemRandRateの確率でアイテムを生成
	if (MyMath::GetRand(1, kCreateItemRandRate))
	{
		ItemType itemIndex = static_cast<ItemType>(MyMath::GetRand(static_cast<int>(ItemType::Heart), static_cast<int>(ItemType::DefenseUp)));
		CreateItem(itemIndex, pos);
	}
}


void ActorManager::AllDeleteNormalEnemy()
{
	//敵
	for (auto& actor : m_actors)
	{
		//敵を探す
		if (actor->GetGameTag() == GameTag::Enemy)
		{
			auto enemy = std::dynamic_pointer_cast<EnemyBase>(actor);
			//ボス以外なら
			if (enemy->GetEnemyGrade() != EnemyGrade::Boss)
			{
				enemy->Delete();
			}
		}
	}
}

void ActorManager::AllDeleteAttack()
{
	//攻撃
	for (auto& actor : m_actors)
	{
		//敵を探す
		if (actor->GetGameTag() == GameTag::Attack)
		{
			actor->Delete();
		}
	}
}

//プレイヤーに近い敵を取得
std::weak_ptr<Actor> ActorManager::GetNearestEnemy() const
{
	std::weak_ptr<Actor> nearestEnemy; //近い敵のポインタ
	float minDis = 1000000.0f; //初期値は大きな値
	for (auto& actor : m_actors)
	{
		if (actor->GetGameTag() == GameTag::Enemy)
		{
			//死んでるならターゲット無視
			if (std::dynamic_pointer_cast<EnemyBase>(actor)->GetHitPoints().lock()->IsDead())continue;
			//プレイヤーに近い敵を探す
			float dis = (m_player.lock()->GetPos() - actor->GetPos()).Magnitude();
			if (dis < minDis)
			{
				minDis = dis;
				nearestEnemy = actor; //近い敵を更新
			}
		}
	}
	return nearestEnemy;
}

AttackData ActorManager::GetAttackData(std::string& ownerName, std::string& attackName)
{
	AttackData attackData;
	for (auto data : m_attackDatas)
	{
		//持ち主と攻撃の名前が同じなら
		if (data.ownerName == ownerName && data.attackName == attackName)
		{
			attackData = data;
			break;
		}
	}
	return attackData;
}

bool ActorManager::IsBossDead() const
{
	if (m_boss.expired())
	{
		return true; //ボスが死んでいる
	}
	return m_boss.lock()->GetHitPoints().lock()->IsDead();
}

bool ActorManager::IsBossDisappear() const
{
	if (m_bossDisappearFrame >= kBossDisappearFrameMax)
	{
		return true; //ボスが完全に消えた
	}
	return false; //ボスはまだ消えていない
}

//アクターの消滅フラグをチェックして削除
void ActorManager::CheckDeleteActors(const std::weak_ptr<Score> score)
{
	std::list<std::shared_ptr<Actor>> deleteActer;
	for (int i = 0;i < 3;++i)
	{
		bool isOneMore = false;
		auto thisPointer = shared_from_this();
		for (auto& actor : m_actors)
		{
			bool isDead = actor->IsDelete();//死亡したかをチェック
			if (isDead)
			{
				isOneMore = true;
				//死亡したアクターの終了処理
				actor->Dead(thisPointer, score);
				//削除候補
				deleteActer.emplace_back(actor);
			}
		}
		//削除
		for (auto& actor : deleteActer)
		{
			thisPointer->Exit(actor);
		}
		deleteActer.clear();
		if (!isOneMore)break;
	}
}

void ActorManager::DelayUpdate(int frame)
{
	m_delayFrame = frame;
	m_isUpdate = false;
}

void ActorManager::HitStop(ShakePower sp, int frame)
{
	if (m_delayFrame > frame)return;
	//物理挙動止める
	Physics::GetInstance().DelayUpdate(frame);
	//カメラ揺らす
	m_camera.lock()->SetCameraShake(sp, frame);
	//エフェクトを止める
	EffekseerManager::GetInstance().DelayUpdate(frame);
	//actorの更新止める
	this->DelayUpdate(frame);
}

void ActorManager::RevivalPlayer()
{
	if (!m_player.expired())
	{
		//プレイヤーを復活
		m_player.lock()->Revival();
	}
	//攻撃をすべて削除
	AllDeleteAttack();
}

//追加予定のアクターを実装
void ActorManager::CheckNextAddActors()
{
	for (auto& actor : m_nextAddActors)
	{
		Entry(actor);
	}
	m_nextAddActors.clear();//追加予定のアクターは消す
}

//攻撃データの取得
void ActorManager::CreateAttackData()
{
	m_attackDatas = m_csvLoader->LoadAttackDataCSV();
}

void ActorManager::LoadHandle()
{
	m_handles["Player"]			= { MV1LoadModel(L"Data/Model/Player/Player.mv1") };
	m_handles["InvisibleWall"]	= { MV1LoadModel(L"Data/Model/Stage/InvisibleWall.mv1") };
	m_handles["PurpleDinosaur"] = { MV1LoadModel(L"Data/Model/Enemy/PurpleDinosaur.mv1") };
	m_handles["SmallDragon"]	= { MV1LoadModel(L"Data/Model/Enemy/SmallDragon.mv1") };
	m_handles["Bomber"]			= { MV1LoadModel(L"Data/Model/Enemy/Bomber.mv1") };
	m_handles["BossDragon"]		= { MV1LoadModel(L"Data/Model/Enemy/BossDragon.mv1") };
	m_handles["BossMuscle"]		= { MV1LoadModel(L"Data/Model/Enemy/BossMuscle.mv1") };
	m_handles["BossKing"]		= { MV1LoadModel(L"Data/Model/Enemy/BossKing.mv1") };
	m_handles["BossKing2"]		= { MV1LoadModel(L"Data/Model/Enemy/BossKing2.mv1") };
	m_handles["Path"]			= { MV1LoadModel(L"Data/Model/Stage/1/Path.mv1") };
	m_handles["Block_Grass"]	= { MV1LoadModel(L"Data/Model/Stage/1/Block_Grass.mv1") };
	m_handles["Block_Stone"]	= { MV1LoadModel(L"Data/Model/Stage/2/Block_Stone.mv1") };
	m_handles["ThroneRoom"]		= { MV1LoadModel(L"Data/Model/Stage/3/ThroneRoom.mv1") };
	m_handles["Chair"]			= { MV1LoadModel(L"Data/Model/Stage/3/Chair.mv1") };
	m_handles["Cube"]			= { MV1LoadModel(L"Data/Model/Collision/Cube.mv1")};
	m_handles["Cylinder"]		= { MV1LoadModel(L"Data/Model/Collision/Cylinder.mv1") };
	m_handles["Plane"]			= { MV1LoadModel(L"Data/Model/Collision/Plane.mv1") };
	m_handles["Sky"]			= { MV1LoadModel(L"Data/Model/Stage/Sky/Sky_Daylight02.pmx") };
	m_handles["Heart"]			= { MV1LoadModel(L"Data/Model/Item/Heart.mv1") };
	m_handles["Bomb"]			= { MV1LoadModel(L"Data/Model/Item/Bomb.mv1") };
	m_handles["UltGageUp"]		= { MV1LoadModel(L"Data/Model/Item/UltGageUp.mv1") };
	m_handles["AttackUp"]		= { MV1LoadModel(L"Data/Model/Item/AttackUp.mv1") };
	m_handles["DefenseUp"]		= { MV1LoadModel(L"Data/Model/Item/DefenseUp.mv1") };
	m_handles["Tree"]			= { MV1LoadModel(L"Data/Model/Stage/Title/Tree1.mv1") };
	m_handles["Grass"]			= { MV1LoadModel(L"Data/Model/Stage/Title/Grass1.mv1") };
	m_handles["Flower"]			= { MV1LoadModel(L"Data/Model/Stage/Title/Flower1.mv1") };
	m_handles["Rock1"]			= { MV1LoadModel(L"Data/Model/Stage/Title/Rock1.mv1") };
	m_handles["Rock2"]			= { MV1LoadModel(L"Data/Model/Stage/Title/Rock2.mv1") };
	m_handles["Arrow_Side"]		= { MV1LoadModel(L"Data/Model/Stage/Arrow_Side.mv1") };

	//ロードに成功したかチェック
	for (auto& [key, value] : m_handles) {
		assert(value >= 0);
	}
}

void ActorManager::LoadStage(Stage::StageIndex index)
{
	//配置データを取得
	std::string charaPath;
	std::string drawPath;
	std::string collPath;
	std::string eventAreaPath;
	switch (index)
	{
	case Stage::StageIndex::Stage1:
		charaPath = "Data/CSV/Stage1/Stage1CharacterTransformData.csv";
		drawPath = "Data/CSV/Stage1/Stage1TransformData.csv";
		collPath = "Data/CSV/Stage1/Stage1CollisionTransformData.csv";
		eventAreaPath = "Data/CSV/Stage1/Stage1EventTransformData.csv";
		break;
	case Stage::StageIndex::Stage2:
		charaPath = "Data/CSV/Stage2/Stage2CharacterTransformData.csv";
		drawPath = "Data/CSV/Stage2/Stage2TransformData.csv";
		collPath = "Data/CSV/Stage2/Stage2CollisionTransformData.csv";
		eventAreaPath = "Data/CSV/Stage2/Stage2EventTransformData.csv";
		break;
	case Stage::StageIndex::Stage3:
		charaPath = "Data/CSV/Stage3/Stage3CharacterTransformData.csv";
		drawPath = "Data/CSV/Stage3/Stage3TransformData.csv";
		collPath = "Data/CSV/Stage3/Stage3CollisionTransformData.csv";
		eventAreaPath = "Data/CSV/Stage3/Stage3EventTransformData.csv";
		break;
	default:
		break;
	}
	auto characterData = m_csvLoader->LoadActorDataCSV(charaPath.c_str());
	//名前からオブジェクトを配置していく
	for (auto& charaData : characterData)
	{
		CreateCharacter(charaData);
	}
	//描画用
	//配置データを取得
	auto stageDrawData = m_csvLoader->LoadActorDataCSV(drawPath.c_str());
	//名前からオブジェクトを配置していく
	for (auto& stageData : stageDrawData)
	{
		if (stageData.name == "Path")
		{
			std::shared_ptr<StageObjectDraw> obj =
				std::make_shared<StageObjectDraw>(MV1DuplicateModel(m_handles["Path"]), stageData.pos, stageData.scale, stageData.rot);
			m_nextAddActors.emplace_back(obj);
		}
		else if (stageData.name == "Block_Grass")
		{
			//大きさを1/100しないと大きすぎるので
			stageData.scale = VScale(stageData.scale, 0.01f);
			std::shared_ptr<StageObjectDraw> obj =
				std::make_shared<StageObjectDraw>(MV1DuplicateModel(m_handles["Block_Grass"]), stageData.pos, stageData.scale, stageData.rot);
			m_nextAddActors.emplace_back(obj);
		}
		else if (stageData.name == "Block_Stone")
		{
			//大きさを1/100しないと大きすぎるので
			stageData.scale = VScale(stageData.scale, 0.01f);
			std::shared_ptr<StageObjectDraw> obj =
				std::make_shared<StageObjectDraw>(MV1DuplicateModel(m_handles["Block_Stone"]), stageData.pos, stageData.scale, stageData.rot);
			m_nextAddActors.emplace_back(obj);
		}
		else if (stageData.name == "Tree")
		{
			std::shared_ptr<StageObjectDraw> obj =
				std::make_shared<StageObjectDraw>(MV1DuplicateModel(m_handles["Tree"]), stageData.pos, stageData.scale, stageData.rot);
			m_nextAddActors.emplace_back(obj);
		}
		else if (stageData.name == "Grass")
		{
			std::shared_ptr<StageObjectDraw> obj =
				std::make_shared<StageObjectDraw>(MV1DuplicateModel(m_handles["Grass"]), stageData.pos, stageData.scale, stageData.rot);
			m_nextAddActors.emplace_back(obj);
		}
		else if (stageData.name == "Flower")
		{
			std::shared_ptr<StageObjectDraw> obj =
				std::make_shared<StageObjectDraw>(MV1DuplicateModel(m_handles["Flower"]), stageData.pos, stageData.scale, stageData.rot);
			m_nextAddActors.emplace_back(obj);
		}
		else if (stageData.name == "Rock1")
		{
			std::shared_ptr<StageObjectDraw> obj =
				std::make_shared<StageObjectDraw>(MV1DuplicateModel(m_handles["Rock1"]), stageData.pos, stageData.scale, stageData.rot);
			m_nextAddActors.emplace_back(obj);
		}
		else if (stageData.name == "Rock2")
		{
			std::shared_ptr<StageObjectDraw> obj =
				std::make_shared<StageObjectDraw>(MV1DuplicateModel(m_handles["Rock2"]), stageData.pos, stageData.scale, stageData.rot);
			m_nextAddActors.emplace_back(obj);
		}
		else if (stageData.name == "ThroneRoom")
		{
			std::shared_ptr<StageObjectDraw> obj =
				std::make_shared<StageObjectDraw>(MV1DuplicateModel(m_handles["ThroneRoom"]), stageData.pos, stageData.scale, stageData.rot);
			m_nextAddActors.emplace_back(obj);
		}
		else if (stageData.name == "Chair")
		{
			//大きさを1/100しないと大きすぎるので
			stageData.scale = VScale(stageData.scale, 0.01f);
			std::shared_ptr<StageObjectDraw> obj =
				std::make_shared<StageObjectDraw>(MV1DuplicateModel(m_handles["Chair"]), stageData.pos, stageData.scale, stageData.rot);
			m_nextAddActors.emplace_back(obj);
		}
		else if (stageData.name == "Arrow_Side")
		{
			//大きさを1/100しないと大きすぎるので
			stageData.scale = VScale(stageData.scale, 0.01f);
			stageData.rot.x += 90.0f; //横向きにする
			std::shared_ptr<StageObjectDraw> obj =
				std::make_shared<StageObjectDraw>(MV1DuplicateModel(m_handles["Arrow_Side"]), stageData.pos, stageData.scale, stageData.rot);
			m_nextAddActors.emplace_back(obj);
		}
	}
	//当たり判定用
	//配置データを取得
	auto stageCollData = m_csvLoader->LoadActorDataCSV(collPath.c_str());
	//名前からコリジョンを配置していく
	for (auto& stageData : stageCollData)
	{
		if (stageData.name == "Plane" || stageData.name == "BossAreaWall")
		{
			std::shared_ptr<StageObjectCollision> plane =
				std::make_shared<StageObjectCollision>(MV1DuplicateModel(m_handles["Plane"]), stageData.pos, stageData.scale, stageData.rot);
			m_nextAddActors.emplace_back(plane);
		}
		else if (stageData.name == "Cube")
		{
			std::shared_ptr<StageObjectCollision> cube =
				std::make_shared<StageObjectCollision>(MV1DuplicateModel(m_handles["Cube"]), stageData.pos, stageData.scale, stageData.rot);
			m_nextAddActors.emplace_back(cube);
		}
		else if (stageData.name == "Rock1")
		{
			std::shared_ptr<StageObjectCollision> cube =
				std::make_shared<StageObjectCollision>(MV1DuplicateModel(m_handles["Rock1"]), stageData.pos, stageData.scale, stageData.rot);
			m_nextAddActors.emplace_back(cube);
		}
	}
	//イベント部屋を作成
	auto eventAreaData = m_csvLoader->LoadActorDataCSV(eventAreaPath.c_str());
	std::list<std::shared_ptr<StageObjectCollision>> allKillAreaParts;
	std::list<std::shared_ptr<StageObjectCollision>> zCaneraMoveAreaParts;
	std::list<std::shared_ptr<StageObjectCollision>> bossAreaParts;
	//名前からコリジョンを配置していく
	for (auto& stageData : eventAreaData)
	{
		if (stageData.name == "AllKillAreaS" || stageData.name == "AllKillAreaE")
		{
			auto coll = std::make_shared<StageObjectCollision>(MV1DuplicateModel(m_handles["Plane"]), stageData.pos, stageData.scale, stageData.rot);
			allKillAreaParts.emplace_back(coll);
			m_nextAddActors.emplace_back(coll);
		}
		if (stageData.name == "ZMoveAreaS" || stageData.name == "ZMoveAreaE")
		{
			auto coll = std::make_shared<StageObjectCollision>(MV1DuplicateModel(m_handles["Plane"]), stageData.pos, stageData.scale, stageData.rot);
			zCaneraMoveAreaParts.emplace_back(coll);
			m_nextAddActors.emplace_back(coll);
		}
		else if (stageData.name == "BossAreaS" || stageData.name == "BossAreaE")
		{
			auto coll = std::make_shared<StageObjectCollision>(MV1DuplicateModel(m_handles["Plane"]), stageData.pos, stageData.scale, stageData.rot);
			bossAreaParts.emplace_back(coll);
			m_nextAddActors.emplace_back(coll);
		}
		//エリアを構成する2要素が揃ったら
		if (allKillAreaParts.size() >= kAreaPartsNum)
		{
			auto eventArea = std::make_shared<AllKillArea>(allKillAreaParts.front(), allKillAreaParts.back());
			m_nextAddActors.emplace_back(eventArea);
			allKillAreaParts.clear();
		}
		if (zCaneraMoveAreaParts.size() >= kAreaPartsNum)
		{
			auto eventArea = std::make_shared<ZMoveCameraArea>(zCaneraMoveAreaParts.front(), zCaneraMoveAreaParts.back());
			m_nextAddActors.emplace_back(eventArea);
			zCaneraMoveAreaParts.clear();
		}
		if (bossAreaParts.size() >= kAreaPartsNum)
		{
			auto bossArea = std::make_shared<BossArea>(bossAreaParts.front(), bossAreaParts.back());
			m_bossArea = bossArea;
			m_nextAddActors.emplace_back(bossArea);
			bossAreaParts.clear();
		}
	}
	//フィールド
	switch (m_stageIndex)
	{
	case Stage::StageIndex::Stage1:
		//空を作成
		m_actors.emplace_back(std::make_shared<Sky>(MV1DuplicateModel(m_handles["Sky"])));
		//ステージのエフェクトを作成(プレイヤーに追従)
		EffekseerManager::GetInstance().CreateTrackActorEffect("FieldEff", m_player);
		break;
	case Stage::StageIndex::Stage2:
		//空を作成
		m_actors.emplace_back(std::make_shared<Sky>(MV1DuplicateModel(m_handles["Sky"])));
		//ステージのエフェクトを作成(プレイヤーに追従)
		EffekseerManager::GetInstance().CreateTrackActorEffect("FieldEff", m_player);
		break;
	case Stage::StageIndex::Stage3:

		break;
	default:
		break;
	}
}

void ActorManager::AllDeleteActors()
{
	//アクターの終了処理
	std::list<std::shared_ptr<Actor>> deleteActer;
	for (auto& actor : m_actors)
	{
		deleteActer.emplace_back(actor);
	}
	for (auto& actor : deleteActer)
	{
		Exit(actor);
	}
	deleteActer.clear();
	m_nextAddActors.clear();
	m_player.reset();
	m_boss.reset();
}

void ActorManager::AllDeleteHandles()
{
	for (auto& handle : m_handles) {
		if (handle.second >= 0)
		{
			auto result = MV1DeleteModel(handle.second);
			assert(result == 0);
		}
	}
	m_handles.clear();
}