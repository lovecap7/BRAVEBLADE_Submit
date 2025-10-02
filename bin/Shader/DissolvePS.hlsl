// �s�N�Z���V�F�[�_�[�̓���
struct PS_INPUT
{
    float4 pos : SV_Position;
    float4 dif : COLOR0; // �f�B�t���[�Y�J���[
    //float4 spc : COLOR1; // �f�B�t���[�Y�J���[
    float2 uv : TEXCOORD0; // �e�N�X�`�����W
};

// �s�N�Z���V�F�[�_�[�̏o��
struct PS_OUTPUT
{
    float4 color : SV_TARGET0; // �F
};

//�萔�o�b�t�@
cbuffer buff : register(b4)
{
    float disValue; //�f�B�]���u��臒l
    //16�o�C�g�A���C�����g�����p
    float dummy1; 
    float dummy2; 
    float dummy3; 
}

SamplerState sam : register(s0); //�T���v��(��q)
Texture2D tex : register(t0); //�e�N�X�`��
Texture2D ptn : register(t1); //�p�^�[��
						  
//�f�B�]���u������
//0 ~ 1�̒l����f�B�]���u���s��
PS_OUTPUT main(PS_INPUT input)
{
    PS_OUTPUT output;

    //�p�^�[���e�N�X�`���̐ԃ`�����l���̒l�i0.0�`1.0�j���擾
    float discval = ptn.Sample(sam, input.uv).r;
    
    //UV��Y���W�� disValue �̍������ɁA�G�b�W���ӂ����炩�Ƀt�F�[�h�A�E�g�����邽�߂̕␳�l
    float yEdge = 1.0 - saturate(abs(disValue - input.uv.y));

    //0~1�̒l�����ɁA�f�B�]���u�̂������l���v�Z
    float threshold = input.uv.y + (yEdge * discval);
    
    //�f�B�]���u�����F���������Ă�����`��j��
    if (threshold < disValue)
    {
        discard;
    }
    
    //�G�b�W�t�߂����点��(�f�B�]���u�̂������l�𒴂�������������)
    float edgeColor = 1.0 - saturate(distance(disValue, threshold));
    edgeColor = pow(edgeColor, 5.0); //�G�b�W�̌�����𒲐�

    // �ʏ�̃e�N�X�`���F��Ԃ�
    float4 col = tex.Sample(sam, input.uv);
    //�����������폜
    if (col.a == 0.0)
    {
        discard;
    }
    //����
    output.color = col + edgeColor * float4(1, 1, 0, 1);
    return output;
}