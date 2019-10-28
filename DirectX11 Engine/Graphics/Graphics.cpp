#include "Graphics.h"

bool Graphics::Init(HWND hWnd, int width, int height)
{
	this->windowWidth = width;
	this->windowHeight = height;
	fpsTimer.Start();

	if (!InitDirectX(hWnd))
		return false;

	if (!InitShaders())
		return false;
	
	if (!InitScene())
		return false;

	InitImGUI(hWnd);

	return true;
}

void Graphics::InitImGUI(HWND hWnd)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(this->device.Get(), this->deviceContext.Get());
	ImGui::StyleColorsDark();
}

void Graphics::RenderFrame()
{
	//this->cb_ps_light.data.pointLightColor = light.lightColor;
	//this->cb_ps_light.data.pointLightStrength = light.lightStrength;
	//this->cb_ps_light.data.pointLightPosition = light.GetPositionFloat3();

	//this->cb_ps_light.ApplyChanges();
	//this->deviceContext->PSSetConstantBuffers(0, 1, this->cb_ps_light.GetAddressOf());

	this->cb_ps_lighting.data.directionalLightColor = directionalLight.lightColor;
	this->cb_ps_lighting.data.directionalLightStrength = directionalLight.lightStrength;
	this->cb_ps_lighting.data.directionalLightDirection = directionalLightDirection;
	this->cb_ps_lighting.data.pointLightColor = pointLight.lightColor;
	this->cb_ps_lighting.data.pointLightStrength = pointLight.lightStrength;
	this->cb_ps_lighting.data.pointLightPosition = pointLight.GetPositionFloat3();
	this->cb_ps_lighting.data.pointLightConstantAttenuationFactor = pointLight.constantAttenuationFactor;
	this->cb_ps_lighting.data.pointLightLinearAttenuationFactor = pointLight.linearAttenuationFactor;
	this->cb_ps_lighting.data.pointLightExponentAttenuationFactor = pointLight.exponentAttenuationFactor;
	this->cb_ps_lighting.data.shininess = this->shininess;
	this->cb_ps_lighting.data.cameraPosition = this->camera.GetPositionFloat3();

	this->cb_ps_lighting.ApplyChanges();
	this->deviceContext->PSSetConstantBuffers(0, 1, this->cb_ps_lighting.GetAddressOf());

	static float bgColor[] = { 0.0f,0.0f,0.0f,1.0f };
	this->deviceContext->ClearRenderTargetView(this->renderTargetView.Get(), bgColor);
	this->deviceContext->ClearDepthStencilView(this->depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	this->deviceContext->IASetInputLayout(this->vertexShader.GetInputLayout());
	this->deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	if(wireframe == false)
		this->deviceContext->RSSetState(this->rasterizerState.Get());
	else
		this->deviceContext->RSSetState(this->rasterizerState_FillWire.Get());
	this->deviceContext->OMSetDepthStencilState(this->depthStencilState.Get(),0);
	this->deviceContext->OMSetBlendState(NULL, NULL, 0xFFFFFFFF); //this->blendState.Get()
	this->deviceContext->PSSetSamplers(0, 1, this->samplerState.GetAddressOf());
	this->deviceContext->VSSetShader(this->vertexShader.GetShader(),NULL,0);
	//this->deviceContext->PSSetShader(this->pixelShader.GetShader(),NULL,0);

	this->deviceContext->PSSetShader(this->pixelShader_lighting.GetShader(),NULL,0);

	for (int i = 0; i < 10; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			this->gObj.SetPosition((i * 0.5f - 2.5f)*20.0f, 0.0f, (j * 0.5f - 2.5f) * 20.0f);
			this->gObj.Draw(this->camera.GetViewMatrix() * this->camera.GetProjectionMatrix());
		}
	}

	//{
	//	this->gObj.Draw(this->camera.GetViewMatrix() * this->camera.GetProjectionMatrix());
	//}

	this->deviceContext->PSSetShader(this->pixelShader_noLight.GetShader(), NULL, 0);

	{
		this->pointLight.Draw(this->camera.GetViewMatrix() * this->camera.GetProjectionMatrix());
	}
	
	static int fpsCounter = 0;
	static std::string fpsString = "FPS: 0";
	fpsCounter += 1;
	if (fpsTimer.GetMilisecondsElapsed() > 1000.0)
	{
		fpsString = "FPS: " + std::to_string(fpsCounter);
		fpsCounter = 0; 
		this->fpsTimer.Restart();
	}
	
	this->spriteBatch->Begin();
	this->spriteFont->DrawString(this->spriteBatch.get(), StringToolkit::StringToWide(fpsString).c_str(), XMFLOAT2(10,10),DirectX::Colors::White, 0.0f, XMFLOAT2(0.0f, 0.0f), XMFLOAT2(1.0f,1.0f));
	this->spriteBatch->End();

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Debug");
	ImGui::DragFloat("Camera Speed",&cameraSpeed,0.005f,0.0f,5.0f);
	ImGui::DragFloat3("Rotation Speed",modelRotationSpeed,0.00001f,0.0f,1.0f);
	ImGui::Checkbox("Wireframe?", &wireframe);
	ImGui::DragInt("Grayscale Mode", &cb_ps_lighting.data.grayscale, 0.01f,0,2);
	ImGui::End();

	ImGui::Begin("Light Controls");
	ImGui::DragFloat3("Background Color", bgColor, 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat3("Ambient Color", &this->cb_ps_lighting.data.ambientLightColor.x, 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat("Ambient Strength", &this->cb_ps_lighting.data.ambientLightStrength, 0.01f, 0.0f, 1.0f);	

	ImGui::DragFloat3("Directional Light Color", &this->directionalLight.lightColor.x, 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat3("Directional Light Direction", &directionalLightDirection.x, 0.01f, -1.0f, 1.0f);
	ImGui::DragFloat("Directional Strength", &this->directionalLight.lightStrength, 0.01f, 0.0f, 10.0f);

	ImGui::DragFloat3("Point Light Color", &this->pointLight.lightColor.x, 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat("Point Light Strength", &this->pointLight.lightStrength, 0.01f, 0.0f, 10.0f);
	ImGui::DragFloat("Point Constant Attenuation", &this->pointLight.constantAttenuationFactor, 0.01f, 0.01f, 10.0f);
	ImGui::DragFloat("Point Linear Attenuation", &this->pointLight.linearAttenuationFactor,     0.01f, 0.0f, 10.0f);
	ImGui::DragFloat("Point Exponent Attenuation", &this->pointLight.exponentAttenuationFactor, 0.01f, 0.0f, 10.0f);

	ImGui::DragFloat("Shininess", &this->shininess, 1.0f, 1.0f, 64.0f);
	ImGui::End();

	ImGui::Render();

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	this->swapChain->Present(0, NULL);
}

bool Graphics::InitDirectX(HWND hWnd)
{
	try
	{
		std::vector<AdapterData> adapters = AdapterReader::GetAdapters();

		if (adapters.size() < 1)
		{
			ErrorLogger::Log("No DXGI Adapters Found");
			return false;
		}

		int id = 0;
		if (adapters.size() > 1)
			for (size_t i = 0; i < adapters.size(); ++i)
				if (adapters[id].description.DedicatedVideoMemory < adapters[i].description.DedicatedVideoMemory)
					id = i;

		DXGI_SWAP_CHAIN_DESC scd = { 0 };
		scd.BufferDesc.Width = this->windowWidth;
		scd.BufferDesc.Height = this->windowHeight;
		scd.BufferDesc.RefreshRate.Numerator = 60;
		scd.BufferDesc.RefreshRate.Denominator = 1;
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		scd.SampleDesc.Count = 1;
		scd.SampleDesc.Quality = 0;
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.BufferCount = 1;
		scd.OutputWindow = hWnd;
		scd.Windowed = TRUE;
		scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		HRESULT hr = D3D11CreateDeviceAndSwapChain(
			adapters[id].pAdapter,				///IDXGI Adapter (Largest VRAM)
			D3D_DRIVER_TYPE_UNKNOWN,			///D3D Driver
			NULL,								///For software driver type
			NULL,								///Flags for runtime layers
			NULL,								///Feature Levels Array
			0,									///Number of feature levels
			D3D11_SDK_VERSION,					///D3D SDK (Required)
			&scd,								///Swap Chain description
			this->swapChain.GetAddressOf(),		///&swapChain
			this->device.GetAddressOf(),		///&device
			NULL,								///Supported feature level
			this->deviceContext.GetAddressOf()	///&deviceContext
		);
		COM_ERROR_IF_FAILED(hr, "CreateDeviceAndSwapChain Failed");

		Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
		hr = this->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
		COM_ERROR_IF_FAILED(hr, "GetBuffer Failed");

		hr = this->device->CreateRenderTargetView(backBuffer.Get(), NULL, this->renderTargetView.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "CreateRenderTargetView Failed");

		CD3D11_TEXTURE2D_DESC depthStencilTextureDesc(DXGI_FORMAT_D24_UNORM_S8_UINT, this->windowWidth, this->windowHeight);
		depthStencilTextureDesc.MipLevels = 1;
		depthStencilTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		hr = this->device->CreateTexture2D(&depthStencilTextureDesc, NULL, this->depthStencilBuffer.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "CreateTexture2D for DepthStencilBuffer Failed");

		hr = this->device->CreateDepthStencilView(this->depthStencilBuffer.Get(), NULL, this->depthStencilView.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "CreateDepthStencilView for DepthStencilView Failed");

		this->deviceContext->OMSetRenderTargets(1, this->renderTargetView.GetAddressOf(), this->depthStencilView.Get());

		CD3D11_DEPTH_STENCIL_DESC depthStencilStateDesc(D3D11_DEFAULT);
		depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
		hr = this->device->CreateDepthStencilState(&depthStencilStateDesc, this->depthStencilState.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "CreateDepthStencilState for DepthStencilState Failed");

		// Setup Viewport
		CD3D11_VIEWPORT viewport(0.0f,0.0f, static_cast<float>(this->windowWidth), static_cast<float>(this->windowHeight));
		this->deviceContext->RSSetViewports(1, &viewport);

		// Create Rasterizer State
		CD3D11_RASTERIZER_DESC rasterizerDesc(D3D11_DEFAULT);
		hr = this->device->CreateRasterizerState(&rasterizerDesc, this->rasterizerState.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "CreateRasterizerState Failed");

		//RASTERIZER CULL FRONT
		CD3D11_RASTERIZER_DESC rasterizerDesc_CullFront(D3D11_DEFAULT);
		rasterizerDesc_CullFront.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;
		hr = this->device->CreateRasterizerState(&rasterizerDesc_CullFront, this->rasterizerState_CullFront.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "CreateRasterizerState Failed");


		// Create Rasterizer State
		CD3D11_RASTERIZER_DESC rasterizerDesc_FillWire(D3D11_DEFAULT);
		rasterizerDesc_FillWire.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
		hr = this->device->CreateRasterizerState(&rasterizerDesc_FillWire, this->rasterizerState_FillWire.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "CreateRasterizerState Failed");

		// Create Blend State
		D3D11_BLEND_DESC blendDesc = { 0 };

		D3D11_RENDER_TARGET_BLEND_DESC renderTargetBlendDesc = { 0 };
		renderTargetBlendDesc.BlendEnable = true;
		renderTargetBlendDesc.SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
		renderTargetBlendDesc.DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
		renderTargetBlendDesc.BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		renderTargetBlendDesc.SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
		renderTargetBlendDesc.DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
		renderTargetBlendDesc.BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		renderTargetBlendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;

		blendDesc.RenderTarget[0] = renderTargetBlendDesc;

		hr = this->device->CreateBlendState(&blendDesc, this->blendState.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "CreateBlendState Failed");

		// Setup Fonts
		this->spriteBatch = std::make_unique<SpriteBatch>(this->deviceContext.Get());
		this->spriteFont = std::make_unique<SpriteFont>(this->device.Get(), L"Data\\Fonts\\ComicSansMS_16.spritefont");

		// Create Sampler State
		CD3D11_SAMPLER_DESC samplerStateDesc(D3D11_DEFAULT);
		samplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		hr = this->device->CreateSamplerState(&samplerStateDesc, this->samplerState.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "CreateSamplerState Failed");
	}
	catch (COMException & exception)
	{
		ErrorLogger::Log(exception);
		return false;
	}
	return true;
}

bool Graphics::InitShaders()
{
	std::wstring shaderFolder = L"";
	#pragma region DetermineShaderPath
	if (IsDebuggerPresent() == TRUE)
	{
		#ifdef _DEBUG
			#ifdef _WIN64
				shaderFolder = L"..\\x64\\Debug\\";
			#else
				shaderFolder = L"..\\Debug\\";
			#endif
		#else
			#ifdef _WIN64
				shaderFolder = L"..\\x64\\Release\\";
			#else
				shaderFolder = L"..\\Release\\";
			#endif
		#endif
	}

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION",	0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,	0, 0,								D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0  },
		{"TEXCOORD",	0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0  },
		{"NORMAL",		0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0  },
	};
	UINT numElements = ARRAYSIZE(layout);

	if (!vertexShader.Init(this->device, shaderFolder + L"vertexshader.cso",layout,numElements))
		return false;
	
	//if (!pixelShader.Init(this->device, shaderFolder + L"pixelshader.cso"))
	//	return false;

	if (!pixelShader_noLight.Init(this->device, shaderFolder + L"pixelshader_noLight.cso"))
		return false;

	if (!pixelShader_lighting.Init(this->device, shaderFolder + L"lightingShader.cso"))
		return false;

	return true;
}

bool Graphics::InitScene()
{
	try
	{
		//Textures
		//HRESULT hr = CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\pinksquare.jpg", nullptr, this->pinkTexture.GetAddressOf());
		//COM_ERROR_IF_FAILED(hr, "CreateWICTextureFromFile failed for file");

		//CBuffer(s)
		HRESULT hr = cb_vs_vertexShader.Init(this->device.Get(), this->deviceContext.Get());
		COM_ERROR_IF_FAILED(hr, "Failed to initialize cb_vs_vertexShader");

		//hr = cb_ps_light.Init(this->device.Get(), this->deviceContext.Get());
		//COM_ERROR_IF_FAILED(hr, "Failed to initialize cb_ps_light.");

		hr = cb_ps_lighting.Init(this->device.Get(), this->deviceContext.Get());
		COM_ERROR_IF_FAILED(hr, "Failed to initialize cb_ps_lighting.");

		//this->cb_ps_light.data.ambientLightColor = XMFLOAT3(1.0f,1.0f,1.0f);
		//this->cb_ps_light.data.ambientLightStrength = 1.0f;
		
		this->cb_ps_lighting.data.ambientLightColor = XMFLOAT3(1.0f,1.0f,1.0f);
		this->cb_ps_lighting.data.ambientLightStrength = 0.2f;
		this->cb_ps_lighting.data.grayscale = 0;

		//Model(s)
		//if (!gObj.Init("Data\\Objects\\boat\\12219_boat_v2_L2.obj", this->device.Get(), this->deviceContext.Get(), cb_vs_vertexShader))
		//	return false;
		//gObj.SetRotation(XM_PIDIV2, 0.0f, 0.0f);
		if (!gObj.Init("Data\\Objects\\nanosuit\\Nanosuit.obj", this->device.Get(), this->deviceContext.Get(), this->cb_vs_vertexShader))
			return false;
		
		if (!pointLight.Init(this->device.Get(), this->deviceContext.Get(), this->cb_vs_vertexShader))
			return false;

		if (!directionalLight.Init(this->device.Get(), this->deviceContext.Get(), this->cb_vs_vertexShader))
			return false;


		camera.SetPosition(0.0f, 0.0f, -2.0f);
		camera.SetProjectionValues(90.0f, static_cast<float>(this->windowWidth) / static_cast<float>(this->windowHeight), 0.1f, 3000.0f);
	}
	catch (COMException & exception)
	{
		ErrorLogger::Log(exception);
		return false; 
	}
	return true;
}