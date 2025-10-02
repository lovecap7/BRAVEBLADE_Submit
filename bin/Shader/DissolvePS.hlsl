// ピクセルシェーダーの入力
struct PS_INPUT
{
    float4 pos : SV_Position;
    float4 dif : COLOR0; // ディフューズカラー
    //float4 spc : COLOR1; // ディフューズカラー
    float2 uv : TEXCOORD0; // テクスチャ座標
};

// ピクセルシェーダーの出力
struct PS_OUTPUT
{
    float4 color : SV_TARGET0; // 色
};

//定数バッファ
cbuffer buff : register(b4)
{
    float disValue; //ディゾルブの閾値
    //16バイトアライメント調整用
    float dummy1; 
    float dummy2; 
    float dummy3; 
}

SamplerState sam : register(s0); //サンプラ(後述)
Texture2D tex : register(t0); //テクスチャ
Texture2D ptn : register(t1); //パターン
						  
//ディゾルブをする
//0 ~ 1の値からディゾルブを行う
PS_OUTPUT main(PS_INPUT input)
{
    PS_OUTPUT output;

    //パターンテクスチャの赤チャンネルの値（0.0～1.0）を取得
    float discval = ptn.Sample(sam, input.uv).r;
    
    //UVのY座標と disValue の差を元に、エッジ周辺を滑らかにフェードアウトさせるための補正値
    float yEdge = 1.0 - saturate(abs(disValue - input.uv.y));

    //0~1の値を元に、ディゾルブのしきい値を計算
    float threshold = input.uv.y + (yEdge * discval);
    
    //ディゾルブ条件：基準を下回っていたら描画破棄
    if (threshold < disValue)
    {
        discard;
    }
    
    //エッジ付近を光らせる(ディゾルブのしきい値を超えた部分を強調)
    float edgeColor = 1.0 - saturate(distance(disValue, threshold));
    edgeColor = pow(edgeColor, 5.0); //エッジの光り方を調整

    // 通常のテクスチャ色を返す
    float4 col = tex.Sample(sam, input.uv);
    //透明部分を削除
    if (col.a == 0.0)
    {
        discard;
    }
    //合成
    output.color = col + edgeColor * float4(1, 1, 0, 1);
    return output;
}