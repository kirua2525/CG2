#include "Object3d.hlsli"

struct PixelShaderOutput
{

    float32_t4 color : SV_TARGET0;
	
    
};

struct Material
{
    
    float32_t4 color;
    int32_t enableLighting;
    float32_t4x4 uvTransform;
    
};

struct TransformationMatrix
{
    
    float32_t4x4 WVP;
    float32_t4x4 World;
    
};

struct DirectionalLight
{
    
    float32_t4 color;
    float32_t3 direction;
    float intensity;
    
};


ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

PixelShaderOutput main(VertexShaderOutput input)
{
      PixelShaderOutput output;
    
     float32_t4 textureColor = gTexture.Sample(gSampler, input.texcoord);
    
    float32_t3 normal = normalize(input.normal);
    
    float4 transformedUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
    textureColor = gTexture.Sample(gSampler, transformedUV.xy);
    
    
    
    if (gMaterial.enableLighting != 0)
    {     
        
       // float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
        
        float NdotL = abs(dot(normal, -normalize(gDirectionalLight.direction)));
        
        float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
        
        //float cos = saturate(dot(normal, -normalize(gDirectionalLight.direction)));
       
        //output.color = gMaterial.color * textureColor * gDirectionalLight.color * cos * gDirectionalLight.intensity;
        
        output.color.rgb = gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
        output.color.a = gMaterial.color.a * textureColor.a;
        
        
    } else {
        
        output.color = gMaterial.color * textureColor;
        
    }
	
    return output;
}