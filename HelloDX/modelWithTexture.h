////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MODELWITHTEXTURE_H_
#define _MODELWITHTEXTURE_H_

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <directxmath.h>
using namespace DirectX;
#include "texture.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: ModelClass
////////////////////////////////////////////////////////////////////////////////
class ModelWithTexture {
 private:
  struct VertexType {
    XMFLOAT3 position;
    XMFLOAT2 texture;
  };

 public:
  ModelWithTexture();
  ModelWithTexture(const ModelWithTexture &);
  ~ModelWithTexture();

  bool Initialize(ID3D11Device *, ID3D11DeviceContext *, const char *);
  void Shutdown();
  void Render(ID3D11DeviceContext *);

  int GetIndexCount();
  ID3D11ShaderResourceView *GetTexture();

 private:
  bool InitializeBuffers(ID3D11Device *);
  void ShutdownBuffers();
  void RenderBuffers(ID3D11DeviceContext *);
  bool LoadTexture(ID3D11Device *, ID3D11DeviceContext *, const char *);
  void ReleaseTexture();

 private:
  ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
  int m_vertexCount, m_indexCount;

  TextureClass *m_TextureClass;
};

#endif