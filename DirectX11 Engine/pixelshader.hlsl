cbuffer lightBuffer : register(b0)
{
    float3 ambientLightColor;
    float ambientLightStrength;

    float3 pointLightColor;
    float pointLightStrength;
    float3 pointLightPosition;

    float pointLightConstantAttenuationFactor;
    float pointLightLinearAttenuationFactor;
    float pointLightExponentAttenuationFactor;
}

struct PS_INPUT
{
    float4 inPos : SV_POSITION;
    float2 inTexCoord : TEXCOORD;
    float3 inNormal : NORMAL;
    float3 inWorldPos : WORLD_POSITION;
};

Texture2D objTexture : TEXTURE : register(t0);
SamplerState objSamplerState : SAMPLER : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    float3 sampleColor = objTexture.Sample(objSamplerState, input.inTexCoord);

    float3 ambientLight = ambientLightColor * ambientLightStrength;
    float3 appliedLight = ambientLight;

    float3 vectorToLight = normalize(pointLightPosition - input.inWorldPos);

    float3 diffuseLightIntensity = max(dot(vectorToLight, input.inNormal), 0);
    
    float distanceToLight = distance(pointLightPosition, input.inWorldPos);
    float attenuationFactor = 1 / (pointLightConstantAttenuationFactor + pointLightLinearAttenuationFactor * distanceToLight + pointLightExponentAttenuationFactor * pow(distanceToLight, 2));
    
    diffuseLightIntensity *= attenuationFactor;

    float3 diffuseLight = diffuseLightIntensity * pointLightStrength * pointLightColor;
    appliedLight += diffuseLight;

    float3 finalColor = sampleColor * appliedLight;
;
    return float4(finalColor, 1.0f);
}