#pragma once
#pragma once
#include <d3d11.h>
#include "ConstantBufferTypes.h"
#include "..\\ErrorLogger.h"
#include <wrl/client.h>

template<class T>

class ConstantBuffer
{
private:
	ConstantBuffer(const ConstantBuffer<T>& rhs);

	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	ID3D11DeviceContext* deviceContext = nullptr;

public:
	T data;

	ConstantBuffer() {}
	const ID3D11Buffer* Get() { return this->buffer.Get(); }
	ID3D11Buffer* const* GetAddressOf()const { return this->buffer.GetAddressOf(); }

	HRESULT Init(ID3D11Device* device, ID3D11DeviceContext * deviceContext)
	{
		if (this->buffer.Get() != nullptr)
			this->buffer.Reset();

		this->deviceContext = deviceContext;

		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));

		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.ByteWidth = static_cast<UINT>(sizeof(T) + (16 - (sizeof(T) % 16)));
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		return device->CreateBuffer(&desc, 0, this->buffer.GetAddressOf());
	}

	bool ApplyChanges()
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		HRESULT hr = this->deviceContext->Map(this->buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(hr))
		{
			ErrorLogger::Log(hr, L"Failed to map constant buffer");
			return false;
		}
		CopyMemory(mappedResource.pData, &data, sizeof(T));
		this->deviceContext->Unmap(this->buffer.Get(), 0);
		return true;
	}
};

