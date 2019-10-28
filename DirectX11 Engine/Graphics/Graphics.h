#pragma once
#include <SpriteBatch.h>cb_ps_light
#include <SpriteFont.h>
#include <memory>
#include <WICTextureLoader.h>
#include "AdapterReader.h"
#include "Light.h"
#include "Shaders.h"
#include "Camera.h"
#include "..\\Timer.h"
#include "ImGUI\\imgui.h"
#include "ImGUI\\imgui_impl_win32.h"
#include "ImGUI\\imgui_impl_dx11.h"


class Graphics
{
public:
	bool Init(HWND hWnd, int width, int height);
	void RenderFrame();
	Camera camera;
	//RenderableGameObject gObj;
	RenderableGameObject gObj;
	Light pointLight;
	Light directionalLight;
	bool wireframe = false;
	float cameraSpeed = 0.005f;
	float modelRotationSpeed[3] = { 0.0f, 0.0f, 0.0f };

	float shininess = 8.0f;
private:
	bool InitDirectX(HWND hWnd);
	bool InitShaders();
	bool InitScene();
	void InitImGUI(HWND hWnd);

	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;

	VertexShader vertexShader;
	PixelShader pixelShader;
	PixelShader pixelShader_noLight;
	PixelShader pixelShader_lighting;
	ConstantBuffer<CB_VS_vertexShader> cb_vs_vertexShader;
	ConstantBuffer<CB_PS_light> cb_ps_light;
	ConstantBuffer<CB_PS_lighting> cb_ps_lighting;

	XMFLOAT4 directionalLightColor = XMFLOAT4(1.0f,1.0f,1.0f,1.0f);
	XMFLOAT3 directionalLightDirection = XMFLOAT3(0.25f, 0.5f, -1.0f);

	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState_CullFront;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState_FillWire;

	Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;

	std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
	std::unique_ptr<DirectX::SpriteFont> spriteFont;

	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pinkTexture;

	int windowWidth, windowHeight = 0;
	Timer fpsTimer;
};