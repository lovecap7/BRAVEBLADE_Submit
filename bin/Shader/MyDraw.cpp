#include "MyDraw.h"
#include<array>

void MyDrawUtils::DrawRotaGraph(Vector2 pos, float scale, float angle, int handle, int psH, std::list<int> texH, int alpha, int vsH)
{
	MV1SetUseOrigShader(true);//�V�F�[�_���g�����߂�true�ɂ���
	//4���_�ɕK�v�ȏ��������Ă���
	//�K�v�ȏ�񁨍��W�A���_�J���[�AUV
	//�������Arhw�͕K��1.0��
	std::array<DxLib::VERTEX2DSHADER, 4> vertices = {};
	for (auto& v : vertices) {
		v.rhw = 1.0f;//����͕K��1
		v.dif = { 255,255,255,255 };//����͔��F�ɂ��Ă���
	}

	//�摜�̑傫���擾
	int w, h;
	DxLib::GetGraphSize(handle, &w, &h);

	//���ו���Z�̎��ɂȂ�悤��
	//���_���(����)
	vertices[0].pos.x = pos.x - w * 0.5f * scale;
	vertices[0].pos.y = pos.y - h * 0.5f * scale;
	vertices[0].u = 0.0f;
	vertices[0].v = 0.0f;

	//���_���(�E��)
	vertices[1].pos.x = pos.x + w * 0.5f * scale;
	vertices[1].pos.y = pos.y - h * 0.5f * scale;
	vertices[1].u = 1.0f;
	vertices[1].v = 0.0f;

	//���_���(����)
	vertices[2].pos.x = pos.x - w * 0.5f * scale;
	vertices[2].pos.y = pos.y + h * 0.5f * scale;
	vertices[2].u = 0.0f;
	vertices[2].v = 1.0f;

	//���_���(�E��)
	vertices[3].pos.x = pos.x + w * 0.5f * scale;
	vertices[3].pos.y = pos.y + h * 0.5f * scale;
	vertices[3].u = 1.0f;
	vertices[3].v = 1.0f;

	//�e�N�X�`�����w��(���C��)
	DxLib::SetUseTextureToShader(0, handle);

	//�e�N�X�`���������Ă���Ȃ��
	if (texH.size() > 0)
	{
		int slotIndex = 1;
		for (auto& tex : texH) {
			//�X���b�g�Ƀe�N�X�`�����w��
			DxLib::SetUseTextureToShader(slotIndex, tex);
			++slotIndex;
		}
	}
	//�����x
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	//�V�F�[�_���w��
	DxLib::SetUsePixelShader(psH);
	//���_�V�F�[�_���w��
	SetUseVertexShader(vsH);
	//���ߏ���
	int alphamode, alphaparam;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	GetDrawAlphaTest(&alphamode, &alphaparam);
	SetDrawAlphaTest(DX_CMP_GREATER, 0);
	SetUseAlphaTestFlag(true);
	//�V�F�[�_��K�p���ĕ`��
	DxLib::DrawPrimitive2DToShader(vertices.data(), static_cast<int>(vertices.size()),
		DX_PRIMTYPE_TRIANGLESTRIP);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//�V�F�[�_������
	DxLib::SetUseTextureToShader(0, -1);
	if (texH.size() > 0)
	{
		int slotIndex = 1;
		for (auto& tex : texH) {
			//���Z�b�g
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
	//�V�F�[�_���g�����߂�true�ɂ���
	MV1SetUseOrigShader(true);
	//�摜�̑傫���擾
	int width, height;
	GetGraphSize(handle, &width, &height);
	//4���_�ɕK�v�ȏ��������Ă���
	std::array<DxLib::VERTEX2DSHADER, 4> vertices = {};
	for (auto& v : vertices) {
		v.rhw = 1.0;
		v.dif = DxLib::GetColorU8(255, 255, 255, 255);//�f�B�t���[�Y
		v.spc = DxLib::GetColorU8(255, 255, 255, 255);//�X�y�L����
		v.su = 0.0f;
		v.sv = 0.0f;
		v.pos.z = 0.0f;
	}

	//����	
	vertices[0].pos.x = pos.x;
	vertices[0].pos.y = pos.y;
	vertices[0].u = 0.0f;
	vertices[0].v = 0.0f;
	//�E��
	vertices[1].pos.x = pos.x + width;
	vertices[1].pos.y = pos.y;
	vertices[1].u = 1.0f;
	vertices[1].v = 0.0f;
	//����
	vertices[2].pos.x = pos.x;
	vertices[2].pos.y = pos.y + height;
	vertices[2].u = 0.0f;
	vertices[2].v = 1.0f;
	//�E��
	vertices[3].pos.x = pos.x + width;
	vertices[3].pos.y = pos.y + height;
	vertices[3].u = 1.0f;
	vertices[3].v = 1.0f;
	
	//�V�F�[�_���g��
	SetUsePixelShader(psH);
	SetUseTextureToShader(0, handle);
	//�e�N�X�`���������Ă���Ȃ��
	if (texH.size() > 0)
	{
		int slotIndex = 1;
		for (auto& tex : texH) {
			//�X���b�g�Ƀe�N�X�`�����w��
			DxLib::SetUseTextureToShader(slotIndex, tex);
			++slotIndex;
		}
	}

	//���ߏ���
	int alphamode, alphaparam;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	GetDrawAlphaTest(&alphamode, &alphaparam);
	SetDrawAlphaTest(DX_CMP_GREATER, 0);
	SetUseAlphaTestFlag(true);

	//�`��
	DrawPrimitive2DToShader(vertices.data(), static_cast<int>(vertices.size()), DX_PRIMTYPE_TRIANGLESTRIP);
	//�`���̏���
	SetUsePixelShader(-1);
	DxLib::SetUseTextureToShader(0, -1);
	if (texH.size() > 0)
	{
		int slotIndex = 1;
		for (auto& tex : texH) {
			//���Z�b�g
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

		// Y���W�͌Œ�iXZ���ʏ�̉~�j
		DrawLine3D(VECTOR{ x1, pos.y, z1 }, VECTOR{ x2, pos.y, z2 }, color);
	}
}
