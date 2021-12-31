#pragma once

#ifndef _MODEFLOOR_H_
#define _MODEFLOOR_H_

#include <d3d11.h>
#include <directxmath.h>
using namespace DirectX;

#define FLOOR_RANGE 45

class ModelFloor {
private:
  struct VertexType {
    XMFLOAT3 position;
    XMFLOAT4 color;
  };

  ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
  int m_vertexCount, m_indexCount;

public:
  ModelFloor(){};
  ModelFloor(const ModelFloor &){};
  ~ModelFloor(){};

  bool Initialize(ID3D11Device *device) {
    VertexType *vertices;
    D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
    HRESULT result;

    // Set the number of vertices in the vertex array.
    m_vertexCount = FLOOR_RANGE * 2 * 2;

    // Create the vertex array.
    vertices = new VertexType[m_vertexCount];
    if (!vertices) {
      return false;
    }

    /*
    [長方體]
  */
    unsigned int index = 0;
    for (int i = 1; i <= FLOOR_RANGE / 2; i++) {
      vertices[index].position = XMFLOAT3(-i, 0.0f, 25.0f);
      vertices[index + 1].position = XMFLOAT3(-i, 0.0f, -25.0f);
      vertices[index + 2].position = XMFLOAT3(i, 0.0f, 25.0f);
      vertices[index + 3].position = XMFLOAT3(i, 0.0f, -25.0f);
      index += 4;
    }
    vertices[index++].position = XMFLOAT3(0, 0, 25.0);
    vertices[index++].position = XMFLOAT3(0, 0, -25.0);

    for (int i = 1; i <= FLOOR_RANGE / 2; i++) {
      vertices[index].position = XMFLOAT3(25.0, 0.0f, -i);
      vertices[index + 1].position = XMFLOAT3(-25.0, 0.0f, -i);
      vertices[index + 2].position = XMFLOAT3(25.0, 0.0f, i);
      vertices[index + 3].position = XMFLOAT3(-25, 0.0f, i);
      index += 4;
    }
    vertices[index++].position = XMFLOAT3(25.0f, 0, 0);
    vertices[index++].position = XMFLOAT3(-25.0f, 0, 0);

   for(int i=0;i<m_vertexCount;i++){
       vertices[i].color = XMFLOAT4(0.3f,0.3f,0.3f,1.0f);
   }

    // Create the index array.
    unsigned int indices[FLOOR_RANGE * 2 * 2];
    // Set the number of indices in the index array.
    m_indexCount = sizeof(indices) / sizeof(indices[0]);
    for(int i=0;i<m_indexCount;i++){
        indices[i] = i;
    }

    /*
        [實際用DirectX建立buffer]
    */
    // Set up the description of the static vertex buffer.
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
    vertexBufferDesc.StructureByteStride = 0;

    // Give the subresource structure a pointer to the vertex data.
    vertexData.pSysMem = vertices;
    vertexData.SysMemPitch = 0;
    vertexData.SysMemSlicePitch = 0;

    // Now create the vertex buffer.
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
    if (FAILED(result)) {
      return false;
    }

    // Set up the description of the static index buffer.
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned int) * m_indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
    indexBufferDesc.StructureByteStride = 0;

    // Give the subresource structure a pointer to the index data.
    indexData.pSysMem = indices;
    indexData.SysMemPitch = 0;
    indexData.SysMemSlicePitch = 0;

    // Create the index buffer.
    result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
    if (FAILED(result)) {
      return false;
    }

    // Release the arrays now that the vertex and index buffers have been created and loaded.
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