#pragma once
#include <map>
#include <string>
#include <list>
#include <memory>
/// <summary>
///非同期ロード
/// </summary>
class LoadingManager final
{
private:
	//シングルトンの準備
	LoadingManager() = default;
	~LoadingManager() = default;
	//コピー禁止
	LoadingManager(const LoadingManager&) = delete;
	LoadingManager& operator = (const LoadingManager&) = delete;
	//ムーブ禁止
	LoadingManager(LoadingManager&&) = delete;
	LoadingManager& operator = (LoadingManager&&) = delete;
public:
	//インスタンスを取得
	static LoadingManager& GetInstance()
	{
		static LoadingManager instance;
		return instance;
	}
	//初期化
	void Init();
	//更新
	void Update();
	//描画
	void Draw() const;
	//終了
	void End();
	//非同期ロード開始
	void StartLoading();
	//終了
	void StopLoading();
	//非同期ロード中
	bool IsLoading()const;
private:
	//ロード数
	int m_loadingNum;
	//非同期ロードフレームをカウント
	int m_countFrame;
};
