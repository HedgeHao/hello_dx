#include "model.h"

Model::Model() {
  m_vertexBuffer = 0;
  m_indexBuffer = 0;
}

Model::Model(const Model &other) {}

Model::~Model() {}

bool Model::Initialize(ID3D11Device *device) {
  bool result;

  // Initialize the vertex and index buffers.
  result = InitializeBuffers(device);
  if (!result) {
    return false;
  }

  return true;
}

void Model::Shutdown() {
  // Shutdown the vertex and index buffers.
  ShutdownBuffers();

  return;
}

void Model::Render(ID3D11DeviceContext *deviceContext) {
  // Put the vertex and index buffers on the graphics pipeline to prepare them
  // for drawing.
  RenderBuffers(deviceContext);

  return;
}

int Model::GetIndexCount() { return m_indexCount; }

/*
        [建立vertext, index buffer]
*/
bool Model::InitializeBuffers(ID3D11Device *device) {
  VertexType *vertices;
  D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
  D3D11_SUBRESOURCE_DATA vertexData, indexData;
  HRESULT result;

  // Set the number of vertices in the vertex array.
  m_vertexCount = 8;

  // Create the vertex array.
  vertices = new VertexType[m_vertexCount];
  if (!vertices) {
    return false;
  }

  // Create the index array.
  unsigned int indices[] = {0, 1, 1, 2, 2, 3, 3, 0, 4, 5, 5, 6,
                            6, 7, 7, 4, 0, 4, 3, 7, 1, 5, 2, 6};
  // Set the number of indices in the index array.
  m_indexCount = sizeof(indices) / sizeof(indices[0]);

  /*
    [長方體]
  */
  vertices[0].position = XMFLOAT3(0.2f, 0.2f, 0.0f);
  vertices[1].position = XMFLOAT3(-0.2f, 0.2f, 0.0f);
  vertices[2].position = XMFLOAT3(-0.2f, -0.2f, 0.0f);
  vertices[3].position = XMFLOAT3(0.2f, -0.2f, 0);
  vertices[4].position = XMFLOAT3(0.4f, 0.4f, 0.2f);
  vertices[5].position = XMFLOAT3(-0.4f, 0.4f, 0.2f);
  vertices[6].position = XMFLOAT3(-0.4f, -0.4f, 0.2f);
  vertices[7].position = XMFLOAT3(0.4f, -0.4f, 0.2f);
  vertices[0].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
  vertices[1].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
  vertices[2].color = XMFLOAT4(0.5f, 0.2f, 0.7f, 1.0f);
  vertices[3].color = XMFLOAT4(0.5f, 0.2f, 0.7f, 1.0f);
  vertices[4].color = XMFLOAT4(0.3f, 0.7f, 0.5f, 1.0f);
  vertices[5].color = XMFLOAT4(0.3f, 0.7f, 0.5f, 1.0f);
  vertices[6].color = XMFLOAT4(0.7f, 0.4f, 0.2f, 1.0f);
  vertices[7].color = XMFLOAT4(0.7f, 0.4f, 0.2f, 1.0f);

  /*
    [三角形]
  */
  // Load the vertex array with data.
  // vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);  // Bottom left.
  // vertices[0].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

  // vertices[1].position = XMFLOAT3(0.0f, 1.0f, 0.0f);  // Top middle.
  // vertices[1].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

  // vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);  // Bottom right.
  // vertices[2].color = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

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
  result =
      device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
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

  // Release the arrays now that the vertex and index buffers have been created
  // and loaded.
  delete[] vertices;
  vertices = 0;

  return true;
}

void Model::ShutdownBuffers() {
  // Release the index buffer.
  if (m_indexBuffer) {
    m_indexBuffer->Release();
    m_indexBuffer = 0;
  }

  // Release the vertex buffer.
  if (m_vertexBuffer) {
    m_vertexBuffer->Release();
    m_vertexBuffer = 0;
  }

  return;
}

/*
        [把要畫的資料bind上去, 指定畫的方式]
*/
void Model::RenderBuffers(ID3D11DeviceContext *deviceContext) {
  unsigned int stride;
  unsigned int offset;

  // Set vertex buffer stride and offset.
  stride = sizeof(VertexType);
  offset = 0;

  // Set the vertex buffer to active in the input assembler so it can be
  // rendered.
  deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

  // Set the index buffer to active in the input assembler so it can be
  // rendered.
  deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

  // Set the type of primitive that should be rendered from this vertex buffer,
  // in this case triangles.
  deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

  return;
}
