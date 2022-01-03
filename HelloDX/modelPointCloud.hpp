#pragma once

#ifndef _MODELPOINTCLOUD_H_
#define _MODELPOINTCLOUD_H_

#include <d3d11.h>
#include <directxmath.h>
#include <librealsense2/rs.hpp>

using namespace DirectX;

#define POINTS_LENGTH 1280*720

class ModelPointCloud {
private:
	struct VertexType {
		XMFLOAT3 position;
		XMFLOAT4 color;
	};

	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	VertexType* vertices;
	unsigned int* indices;
	ID3D11Device* device;
	ID3D11DeviceContext* d3dContext;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	ID3D11Buffer* m_vertexBuffer, * m_indexBuffer;
	int m_vertexCount, m_indexCount;

public:
	ModelPointCloud() {};
	ModelPointCloud(const ModelPointCloud&) {};
	~ModelPointCloud() {};

	bool Initialize(ID3D11Device* d, ID3D11DeviceContext* context) {
		device = d;
		d3dContext = context;
		HRESULT result;

		vertices = new VertexType[POINTS_LENGTH];
		m_vertexCount = 0;

		indices = new unsigned int[POINTS_LENGTH];
		m_indexCount = 0;

		// Set up the description of the static vertex buffer.
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = POINTS_LENGTH; // Buffer Size 要先開好
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;
		vertexData.pSysMem = vertices;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;
		result =
			device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
		if (FAILED(result)) {
			return false;
		}

		// Set up the description of the static index buffer.
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(unsigned int) * POINTS_LENGTH; // Buffer Size 要先開好
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;
		indexData.pSysMem = indices;
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		// Create the index buffer.
		result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
		if (FAILED(result)) {
			return false;
		}

		return true;
	};

	void update(rs2::points points) {
		const rs2::vertex* rsVertices = points.get_vertices();
		const int size = points.get_data_size();

		const int factor = 5;
		m_vertexCount = 0;
		for (unsigned int i = 0; i < points.size(); i++) {
			if (rsVertices[i].z) {
				m_vertexCount++;
				vertices[m_vertexCount - 1].position = XMFLOAT3(rsVertices[i].x*factor, rsVertices[i].y*factor, rsVertices[i].z*factor);
				vertices[m_vertexCount - 1].color = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
				indices[m_vertexCount - 1] = m_vertexCount - 1;
			}
		}
		d3dContext->UpdateSubresource(m_vertexBuffer, 0, nullptr, vertices, 0, 0);

		m_indexCount = m_vertexCount;
		d3dContext->UpdateSubresource(m_indexBuffer, 0, nullptr, indices, 0, 0);
	}

	void Render(ID3D11DeviceContext* deviceContext) {
		unsigned int stride;
		unsigned int offset;

		stride = sizeof(VertexType);
		offset = 0;

		deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
		deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	};

	int GetIndexCount() { return m_indexCount; }

	void Shutdown() {
		if (m_indexBuffer) {
			m_indexBuffer->Release();
			m_indexBuffer = 0;
		}

		if (m_vertexBuffer) {
			m_vertexBuffer->Release();
			m_vertexBuffer = 0;
		}
	};
};

#endif