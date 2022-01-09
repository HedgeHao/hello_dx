#pragma once

#ifndef _MODELPOINTCLOUD_H_
#define _MODELPOINTCLOUD_H_

#include <d3d11.h>
#include <directxmath.h>

#include <librealsense2/rs.hpp>

#include "texture.h"

using namespace DirectX;

#define WIDTH 640
#define HEIGHT 480

class ModelPointCloud {
 private:
  struct VertexType {
    XMFLOAT3 position;
    XMFLOAT2 texture;
  };

  D3D11_SUBRESOURCE_DATA vertexData, indexData;
  VertexType* vertices;
  unsigned int* indices;
  ID3D11Device* device;
  ID3D11DeviceContext* d3dContext;
  D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
  ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
  int m_vertexCount, m_indexCount;
  TextureClass* m_TextureClass;

 public:
  ModelPointCloud(){};
  ModelPointCloud(const ModelPointCloud&){};
  ~ModelPointCloud(){};

  bool Initialize(ID3D11Device* d, ID3D11DeviceContext* context) {
    device = d;
    d3dContext = context;
    HRESULT result;

    vertices = new VertexType[WIDTH * HEIGHT];
    m_vertexCount = 0;

    indices = new unsigned int[WIDTH * HEIGHT];
    m_indexCount = 0;


    unsigned count = 0;
    for (unsigned int y = 0; y < HEIGHT; y++) {
      for (unsigned int x = 0; x < WIDTH; x++) {
        vertices[count].position = XMFLOAT3(0, 0, 0);
        vertices[count].texture = XMFLOAT2((float)x/(float)WIDTH, (float)y/(float)HEIGHT);

        indices[count] = count;
        count++;
      }
    }

    // Set up the description of the static vertex buffer.
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth =
        sizeof(VertexType) * WIDTH * HEIGHT;  // Buffer Size 要先開好
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
    indexBufferDesc.ByteWidth =
        sizeof(unsigned int) * WIDTH * HEIGHT;  // Buffer Size 要先開好
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

    result = LoadTexture(device, context);
    if (!result) {
      return false;
    }

    return true;
  };

  char d[100];
  void update(rs2::depth_frame frame) {
    m_vertexCount = 0;
    for (int y = 0; y < 480; y++) {
      for (int x = 0; x < 640; x++) {
        vertices[m_vertexCount].position.x = x * 0.005;
        vertices[m_vertexCount].position.y = y * 0.005;
        vertices[m_vertexCount].position.z = frame.get_distance(x, y) * 0.5;
        m_vertexCount++;
      }
    }
    d3dContext->UpdateSubresource(m_vertexBuffer, 0, nullptr, vertices, 0, 0);
    m_indexCount = m_vertexCount;
    d3dContext->UpdateSubresource(m_indexBuffer, 0, nullptr, indices, 0, 0);
  }

  void update(rs2::points points, rs2::video_frame* color) {
    

    const rs2::vertex* rsVertices = points.get_vertices();
    const rs2::texture_coordinate *rsCoord = points.get_texture_coordinates();
    const int size = points.get_data_size();

    float minU = 0, maxU = 1, minV = 0, maxV = 1; 
   /* for (unsigned int i = 0; i < points.size(); i++) {
      if (rsVertices[i].z) {
         if (rsCoord[i].u > maxU) {
          maxU = rsCoord[i].u;
         } else if (rsCoord[i].u < minU) {
           minU = rsCoord[i].u; 
        }

          if (rsCoord[i].v > maxV) {
          maxV = rsCoord[i].v;
        } else if (rsCoord[i].v < minV) {
          minV = rsCoord[i].v;
        }
      }
    }*/

     float factorU = maxU - minU;
    float factorV = maxV - minV;

    m_vertexCount = 0;
    for (unsigned int i = 0; i < points.size(); i++) {
      if (rsVertices[i].z) {
        m_vertexCount++;
        vertices[m_vertexCount - 1].position.x = rsVertices[i].x;
        vertices[m_vertexCount - 1].position.y = rsVertices[i].y * -1;
        vertices[m_vertexCount - 1].position.z = rsVertices[i].z - 0.5;
        vertices[m_vertexCount - 1].texture.x = (rsCoord[i].u - minU)*factorU;
        vertices[m_vertexCount - 1].texture.y = (rsCoord[i].v - minV)*factorV;
      }
    }
    d3dContext->UpdateSubresource(m_vertexBuffer, 0, nullptr, vertices, 0, 0);

    m_indexCount = m_vertexCount;
    d3dContext->UpdateSubresource(m_indexBuffer, 0, nullptr, indices, 0, 0);

    m_TextureClass->update(device, d3dContext, color);
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
  ID3D11ShaderResourceView* GetTexture() {
    return m_TextureClass->GetTexture();
  };

  bool LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext) {
    bool result;

    // Create the texture object.
    m_TextureClass = new TextureClass;
    if (!m_TextureClass) {
      return false;
    }

    // Initialize the texture object.
    result = m_TextureClass->Initialize(device, deviceContext);
    if (!result) {
      return false;
    }

    return true;
  }

  void ReleaseTexture() {
    // Release the texture object.
    if (m_TextureClass) {
      m_TextureClass->Shutdown();
      delete m_TextureClass;
      m_TextureClass = 0;
    }

    return;
  }

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