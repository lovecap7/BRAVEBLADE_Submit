#include "MyDraw.h"
#include<array>

void MyDrawUtils::DrawRotaGraph(Vector2 pos, float scale, float angle, int handle, int psH, std::list<int> texH, int alpha, int vsH)
{
	MV1SetUseOrigShader(true);//シェーダを使うためにtrueにする
	//4頂点に必要な情報を代入していく
	//必要な情報→座標、頂点カラー、UV
	//ただし、rhwは必ず1.0に
	std::array<DxLib::VERTEX2DSHADER, 4> vertices = {};
	for (auto& v : vertices) {
		v.rhw = 1.0f;//これは必ず1
		v.dif = { 255,255,255,255 };//これは白色にしておく
	}

	//画像の大きさ取得
	int w, h;
	DxLib::GetGraphSize(handle, &w, &h);

	//並べ方はZの字になるように
	//頂点情報(左上)
	vertices[0].pos.x = pos.x - w * 0.5f * scale;
	vertices[0].pos.y = pos.y - h * 0.5f * scale;
	vertices[0].u = 0.0f;
	vertices[0].v = 0.0f;

	//頂点情報(右上)
	vertices[1].pos.x = pos.x + w * 0.5f * scale;
	vertices[1].pos.y = pos.y - h * 0.5f * scale;
	vertices[1].u = 1.0f;
	vertices[1].v = 0.0f;

	//頂点情報(左下)
	vertices[2].pos.x = pos.x - w * 0.5f * scale;
	vertices[2].pos.y = pos.y + h * 0.5f * scale;
	vertices[2].u = 0.0f;
	vertices[2].v = 1.0f;

	//頂点情報(右下)
	vertices[3].pos.x = pos.x + w * 0.5f * scale;
	vertices[3].pos.y = pos.y + h * 0.5f * scale;
	vertices[3].u = 1.0f;
	vertices[3].v = 1.0f;

	//テクスチャを指定(メイン)
	DxLib::SetUseTextureToShader(0, handle);

	//テクスチャが入っているならば
	if (texH.size() > 0)
	{
		int slotIndex = 1;
		for (auto& tex : texH) {
			//スロットにテクスチャを指定
			DxLib::SetUseTextureToShader(slotIndex, tex);
			++slotIndex;
		}
	}
	//透明度
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	//シェーダを指定
	DxLib::SetUsePixelShader(psH);
	//頂点シェーダを指定
	SetUseVertexShader(vsH);
	//透過処理
	int alphamode, alphaparam;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	GetDrawAlphaTest(&alphamode, &alphaparam);
	SetDrawAlphaTest(DX_CMP_GREATER, 0);
	SetUseAlphaTestFlag(true);
	//シェーダを適用して描画
	DxLib::DrawPrimitive2DToShader(vertices.data(), static_cast<int>(vertices.size()),
		DX_PRIMTYPE_TRIANGLESTRIP);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//シェーダを解除
	DxLib::SetUseTextureToShader(0, -1);
	if (texH.size() > 0)
	{
		int slotIndex = 1;
		for (auto& tex : texH) {
			//リセット
			SetUseTextureToShader(slotIndex, -1);
			++slotIndex;
		}
	}
	SetUsePixelShader(-1);
	SetUseVertexShader(-1);
	MV1SetUseOrigShader(false);
}

void MyDrawUtils::DrawGraph(Vector2 pos, int handle, int psH, std::list<int> texH)
{
	//シェーダを使うためにtrueにする
	MV1SetUseOrigShader(true);
	//画像の大きさ取得
	int width, height;
	GetGraphSize(handle, &width, &height);
	//4頂点に必要な情報を代入していく
	std::array<DxLib::VERTEX2DSHADER, 4> vertices = {};
	for (auto& v : vertices) {
		v.rhw = 1.0;
		v.dif = DxLib::GetColorU8(255, 255, 255, 255);//ディフューズ
		v.spc = DxLib::GetColorU8(255, 255, 255, 255);//スペキュラ
		v.su = 0.0f;
		v.sv = 0.0f;
		v.pos.z = 0.0f;
	}

	//左上	
	vertices[0].pos.x = pos.x;
	vertices[0].pos.y = pos.y;
	vertices[0].u = 0.0f;
	vertices[0].v = 0.0f;
	//右上
	vertices[1].pos.x = pos.x + width;
	vertices[1].pos.y = pos.y;
	vertices[1].u = 1.0f;
	vertices[1].v = 0.0f;
	//左下
	vertices[2].pos.x = pos.x;
	vertices[2].pos.y = pos.y + height;
	vertices[2].u = 0.0f;
	vertices[2].v = 1.0f;
	//右下
	vertices[3].pos.x = pos.x + width;
	vertices[3].pos.y = pos.y + height;
	vertices[3].u = 1.0f;
	vertices[3].v = 1.0f;
	
	//シェーダを使う
	SetUsePixelShader(psH);
	SetUseTextureToShader(0, handle);
	//テクスチャが入っているならば
	if (texH.size() > 0)
	{
		int slotIndex = 1;
		for (auto& tex : texH) {
			//スロットにテクスチャを指定
			DxLib::SetUseTextureToShader(slotIndex, tex);
			++slotIndex;
		}
	}

	//透過処理
	int alphamode, alphaparam;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	GetDrawAlphaTest(&alphamode, &alphaparam);
	SetDrawAlphaTest(DX_CMP_GREATER, 0);
	SetUseAlphaTestFlag(true);

	//描画
	DrawPrimitive2DToShader(vertices.data(), static_cast<int>(vertices.size()), DX_PRIMTYPE_TRIANGLESTRIP);
	//描画後の処理
	SetUsePixelShader(-1);
	DxLib::SetUseTextureToShader(0, -1);
	if (texH.size() > 0)
	{
		int slotIndex = 1;
		for (auto& tex : texH) {
			//リセット
			SetUseTextureToShader(slotIndex, -1);
			++slotIndex;
		}
	}
	MV1SetUseOrigShader(false);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void MyDrawUtils::Draw3DCircle(Vector3 pos, float radius, int segmentCount, unsigned int color)
{
	float angleStep = MyMath::TwoPI_F / segmentCount;

	for (int i = 0; i < segmentCount; ++i) {
		float angle1 = angleStep * i;
		float angle2 = angleStep * (i + 1);

		float x1 = pos.x + cosf(angle1) * radius;
		float z1 = pos.z + sinf(angle1) * radius;

		float x2 = pos.x + cosf(angle2) * radius;
		float z2 = pos.z + sinf(angle2) * radius;

		// Y座標は固定（XZ平面上の円）
		DrawLine3D(VECTOR{ x1, pos.y, z1 }, VECTOR{ x2, pos.y, z2 }, color);
	}
}
