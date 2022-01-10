#ifndef _TEXTURECLASS_H_
#define _TEXTURECLASS_H_

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <stdio.h>

#include <librealsense2\rs.hpp>

#include "WICTextureLoader.h"

#define WIDTH 1280
#define HEIGHT 720

////////////////////////////////////////////////////////////////////////////////
// Class name: TextureClass
////////////////////////////////////////////////////////////////////////////////
class TextureClass {
 private:
  struct TargaHeader {
    unsigned char data1[12];
    unsigned short width;
    unsigned short height;
    unsigned char bpp;
    unsigned char data2;
  };

 public:
  TextureClass();
  TextureClass(const TextureClass&);
  ~TextureClass();

  bool Initialize(ID3D11Device*, ID3D11DeviceContext*);
  void Shutdown();
  void update(ID3D11Device*, ID3D11DeviceContext*, rs2::video_frame*);

  ID3D11ShaderResourceView* GetTexture();

 private:
  bool LoadTarga(const char*, int&, int&);

 private:
  unsigned char* m_targaData;
  ID3D11Resource* m_textureResource;
  ID3D11Texture2D* m_texture;

  ID3D11ShaderResourceView* m_textureView;
};

#endif