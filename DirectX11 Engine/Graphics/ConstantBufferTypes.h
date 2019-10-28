#pragma once
#include <DirectXMath.h>

struct CB_VS_vertexShader
{
	DirectX::XMMATRIX wvpMatrix;
	DirectX::XMMATRIX worldMatrix;
};

struct CB_PS_light
{
	DirectX::XMFLOAT3 ambientLightColor;			//12
	float ambientLightStrength;						//4

	DirectX::XMFLOAT3 pointLightColor;			//12
	float pointLightStrength;						//4

	DirectX::XMFLOAT3 pointLightPosition;			//12
	float pointLightConstantAttenuationFactor;	//4

	float pointLightLinearAttenuationFactor;		//4 
	float pointLightExponentAttenuationFactor;	//4
};

struct CB_PS_lighting
{
	DirectX::XMFLOAT3 ambientLightColor;			//12
	float ambientLightStrength;						//4
	
	DirectX::XMFLOAT3 directionalLightColor;		//12
	float directionalLightStrength;					//4

	DirectX::XMFLOAT3 directionalLightDirection;	//12
	float pointLightStrength;						//4

	DirectX::XMFLOAT3 pointLightColor;				//12
	float pointLightConstantAttenuationFactor;		//4

	DirectX::XMFLOAT3 pointLightPosition;			//12
	float pointLightLinearAttenuationFactor;		//4 

	DirectX::XMFLOAT3 cameraPosition;				//12
	float shininess;							//4

	int grayscale;									//4
	float pointLightExponentAttenuationFactor;		//4

};