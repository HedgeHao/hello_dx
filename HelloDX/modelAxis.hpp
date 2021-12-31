#pragma once

#ifndef _MODELAXIS_H_
#define _MODELAXIS_H_

#include <d3d11.h>
#include <directxmath.h>
using namespace DirectX;

class ModelAxis {
private:
  struct VertexType {
    XMFLOAT3 position;
    XMFLOAT4 color;
  };

  ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
  int m_vertexCount, m_indexCount;

public:
  ModelAxis(){};
  ModelAxis(const ModelAxis &){};
  ~ModelAxis(){};

  bool Initialize(ID3D11Device *device) {
    VertexType *vertices;
    D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
    HRESULT result;

    m_vertexCount = 6;
    vertices = new VertexType[m_vertexCount];
    if (!vertices) {
      return false;
    }

    vertices[0].position = XMFLOAT3(0, 0, 0);
    vertices[1].position = XMFLOAT3(0, 0, 0);
    vertices[2].position = XMFLOAT3(0, 0, 0);
    vertices[3].position = XMFLOAT3(1.0f, 0, 0);
    vertices[4].position = XMFLOAT3(0, 1.0f, 0);
    vertices[5].position = XMFLOAT3(0, 0, 1.0f);
    vertices[0].color = XMFLOAT4(1.0f, 0, 0, 1.0f);
    vertices[1].color = XMFLOAT4(0, 1.0f, 0, 1.0f);
    vertices[2].color = XMFLOAT4(0, 0, 1.0f, 1.0f);
    vertices[3].color = XMFLOAT4(1.0f, 0, 0, 1.0f);
    vertices[4].color = XMFLOAT4(0, 1.0f, 0, 1.0f);
    vertices[5].color = XMFLOAT4(0, 0, 1.0f, 1.0f);

    unsigned int indices[] = {0, 3, 1, 4, 2, 5};
    m_indexCount = sizeof(indices) / sizeof(indices[0]);

    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
    vertexBufferDesc.StructureByteStride = 0;

    vertexData.pSysMem = vertices;
    vertexData.SysMemPitch = 0;
    vertexData.SysMemSlicePitch = 0;

    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
    if (FAILED(result)) {
      return false;
    }

    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned int) * m_indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
    indexBufferDesc.StructureByteStride = 0;

    indexData.pSysMem = indices;
    indexData.SysMemPitch = 0;
    indexData.SysMemSlicePitch = 0;

    result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
    if (FAILED(result)) {
      return false;
    }

    delete[] vertices;
    vertices = 0;

    return true;
  };

  void Render(ID3D11DeviceContext *deviceContext) {
    unsigned int stride;
    unsigned int offset;

    stride = sizeof(VertexType);
    offset = 0;

    deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
    deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
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