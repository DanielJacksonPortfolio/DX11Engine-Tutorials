#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <memory>

template<class T>

class VertexBuffer
{
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	UINT stride = sizeof(T);
	UINT vertexCount = 0;

public:
	VertexBuffer() {}
	VertexBuffer(const VertexBuffer<T>& rhs)
	{
		this->buffer = rhs.buffer;
		this->vertexCount = rhs.vertexCount;
		this->stride = rhs.stride;
	}
	VertexBuffer<T> & operator=(const VertexBuffer<T>& a)
	{
		this->buffer = a.buffer;
		this->vertexCount = a.vertexCount;
		this->stride = a.stride;
		return *this;
	}

	const ID3D11Buffer* Get() { return this->buffer.Get(); }
	ID3D11Buffer* const* GetAddressOf()const { return this->buffer.GetAddressOf(); }
	const UINT VertexCount() { return this->vertexCount; }
	const UINT Stride() { return this->stride; }
	const UINT* StridePtr() { return &this->stride; }
	
	HRESULT Init(ID3D11Device* device, T* data, UINT vertexCount)
	{
		if (this->buffer.Get() != nullptr)
			this->buffer.Reset();

		this->vertexCount = vertexCount;

		D3D11_BUFFER_DESC vertexBufferDesc;
		ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = this->stride * this->vertexCount;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA vertexBufferData;
		ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
		vertexBufferData.pSysMem = data;

		return device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, this->buffer.GetAddressOf());
	}

};

