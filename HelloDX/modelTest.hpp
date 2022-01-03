#pragma once

#ifndef _MODELTEST_H_
#define _MODELTEST_H_

#include <d3d11.h>
#include <directxmath.h>

#include "sphere.h"

using namespace DirectX;

class ModelTest {
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
  ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
  int m_vertexCount, m_indexCount;

  Sphere mySphere;

 public:
  ModelTest() { mySphere = Sphere(0.005f, 5, 3); };
  ModelTest(const ModelTest&){};
  ~ModelTest(){};

  bool Initialize(ID3D11Device* d, ID3D11DeviceContext* context) {
    device = d;
    d3dContext = context;
    HRESULT result;

    vertices = new VertexType[mySphere.getVertexCount()];
    m_vertexCount = mySphere.getVertexCount();

    indices = new unsigned int[mySphere.getIndexSize()];
    m_indexCount = mySphere.getIndexSize();

    const float* vs = mySphere.getVertices();
    for (unsigned int i = 0; i < mySphere.getVertexCount(); i++) {
      vertices[i].position = XMFLOAT3(*(vs + (i * 3) + 0), *(vs + (i * 3) + 1),
                                      *(vs + (i * 3) + 2));
      vertices[i].color = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
    }

    char de[20];
    sprintf_s(de, "L:%d\n", mySphere.getVertexCount());
    OutputDebugStringA(de);

    // Set up the description of the static vertex buffer.
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * mySphere.getVertexCount();
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
    indexBufferDesc.ByteWidth = sizeof(unsigned int) * mySphere.getIndexSize();
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
    indexBufferDesc.StructureByteStride = 0;
    indexData.pSysMem = mySphere.getIndices();
    indexData.SysMemPitch = 0;
    indexData.SysMemSlicePitch = 0;

    // Create the index buffer.
    result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
    if (FAILED(result)) {
      return false;
    }

    return true;
  };

  void update() {
   /* vertices[0].position = XMFLOAT3(0.0f, 0.5f, 0.0f);
    vertices[0].color = XMFLOAT4(1.0f, 1.0f, 0.5f, 1.0f);
    vertices[1].position = XMFLOAT3(-1.0f, 0.0f, 0.0f);
    vertices[1].color = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);

    indices[0] = 0;
    indices[1] = 1;

    m_vertexCount = 2;
    m_indexCount = 2;

    d3dContext->UpdateSubresource(m_vertexBuffer, 0, nullptr, vertices, 0, 0);
    d3dContext->UpdateSubresource(m_indexBuffer, 0, nullptr, indices, 0, 0);*/
  }

  void Render(ID3D11DeviceContext* deviceContext) {
    unsigned int stride;
    unsigned int offset;

    stride = sizeof(VertexType);
    offset = 0;

    deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
    deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
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