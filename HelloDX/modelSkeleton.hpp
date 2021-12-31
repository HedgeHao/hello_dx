#pragma once

#ifndef _MODELSKELETON_H_
#define _MODELSKELETON_H_

#include <d3d11.h>
#include <directxmath.h>

#include "LIPSBodyPose.h"
#include "LIPSBodyPoseDataStruct.h"
#include "LIPSBodyPoseRender.h"
#include "LIPSBodyPoseTypes.h"

using namespace DirectX;
using namespace lips;

#define MAX_PEOPLE 10

class ModelSkeleton {
private:
  struct VertexType {
    XMFLOAT3 position;
    XMFLOAT4 color;
  };

  D3D11_SUBRESOURCE_DATA vertexData, indexData;
  VertexType *vertices;
  unsigned int *indices;
  ID3D11Device *device;
  ID3D11DeviceContext *d3dContext;
  D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
  ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
  int m_vertexCount, m_indexCount;
  unsigned int numOfPairs = sizeof(POSE_PAIRS_RENDER) / sizeof(POSE_PAIRS_RENDER[0]);

public:
  ModelSkeleton(){};
  ModelSkeleton(const ModelSkeleton &){};
  ~ModelSkeleton(){};

  bool Initialize(ID3D11Device *d, ID3D11DeviceContext *context) {
    device = d;
    d3dContext = context;
    HRESULT result;

    unsigned int numberOfPoints = MAX_PEOPLE * numOfPairs * 2;
    vertices = new VertexType[numberOfPoints];
    m_vertexCount = 0;

    indices = new unsigned int[numberOfPoints];
    m_indexCount = 0;

    // Set up the description of the static vertex buffer.
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = numberOfPoints; // Buffer Size 要先開好
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
        sizeof(unsigned int) * numberOfPoints; // Buffer Size 要先開好
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

  void update(std::shared_ptr<lips::LIPSBodyPoseRecord> skeleton) {

    if (skeleton == nullptr || !skeleton)
      return;

    const float coord_factor = 0.001;
    std::vector<float> verticesV;
    m_vertexCount = 0;
    for (unsigned int person = 0; person < skeleton->getNumberPeople(); person++) {
      const int num_pairs = sizeof(POSE_PAIRS_RENDER) / sizeof(POSE_PAIRS_RENDER[0]);
      for (int pair_idx = 0; pair_idx < num_pairs; pair_idx++) {
        const enum LIPSBODYPOSE_KEYPOINT_ENUM partA = POSE_PAIRS_RENDER[pair_idx][0];
        const enum LIPSBODYPOSE_KEYPOINT_ENUM partB = POSE_PAIRS_RENDER[pair_idx][1];

        const Coord3D &keypointA = skeleton->keypoint3d(person, partA);
        const Coord3D &keypointB = skeleton->keypoint3d(person, partB);

        const float xA = keypointA.x * coord_factor;
        const float yA = keypointA.y * coord_factor;
        const float zA = keypointA.z * coord_factor;

        const float xB = keypointB.x * coord_factor;
        const float yB = keypointB.y * coord_factor;
        const float zB = keypointB.z * coord_factor;

        if (zA >= 0 && zB >= 0) // if these keypoints are both available
        {
          m_vertexCount++;
          vertices[m_vertexCount - 1].position = XMFLOAT3(xA, yA, zA);
          vertices[m_vertexCount - 1].color = XMFLOAT4(LINE_COLORS[3 * pair_idx], LINE_COLORS[3 * pair_idx + 1], LINE_COLORS[3 * pair_idx + 2], 1.0f);
          indices[m_vertexCount - 1] = m_vertexCount - 1;

          m_vertexCount++;
          vertices[m_vertexCount - 1].position = XMFLOAT3(xB, yB, zB);
          vertices[m_vertexCount - 1].color = XMFLOAT4(LINE_COLORS[3 * pair_idx], LINE_COLORS[3 * pair_idx + 1], LINE_COLORS[3 * pair_idx + 2], 1.0f);
          indices[m_vertexCount - 1] = m_vertexCount - 1;
        }
      }
      d3dContext->UpdateSubresource(m_vertexBuffer, 0, nullptr, vertices, 0, 0);

      m_indexCount = m_vertexCount;
      d3dContext->UpdateSubresource(m_indexBuffer, 0, nullptr, indices, 0, 0);
    }
  }
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