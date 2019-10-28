#pragma once
#include "RenderableGameObject.h"
class Light : public RenderableGameObject
{
public:
	bool Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_vertexShader>& cb_vs_vertexShader);

	XMFLOAT3 lightColor = XMFLOAT3(1.0f, 1.0f, 1.0f);
	float lightStrength = 1.0f;
	float constantAttenuationFactor = 1.0f;
	float linearAttenuationFactor = 0.09f;
	float exponentAttenuationFactor = 0.032f;

};

