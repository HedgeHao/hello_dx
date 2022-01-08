#include "texture.h"

TextureClass::TextureClass() {
  m_targaData = 0;
  m_texture = 0;
  m_textureView = 0;
}

TextureClass::TextureClass(const TextureClass& other) {}

TextureClass::~TextureClass() {}

bool TextureClass::Initialize(ID3D11Device* device,
                              ID3D11DeviceContext* deviceContext) {
  HRESULT hResult;
  // ID3D11Resource* resource;
  // hResult = CreateWICTextureFromFile(device, deviceContext, L"car.png",
  //                                 &resource, &m_textureView);

  uint32_t* pixels = new uint32_t[640 * 480];
  uint8_t temp = 0;
  for (unsigned int i = 0; i < 640 * 480; i++) {
    temp = i % 255;
    // RGBA
    pixels[i] = temp + (temp << 8) + (temp << 16) + (0xFF << 24);
  }

  D3D11_SUBRESOURCE_DATA initData = {pixels, sizeof(uint32_t), 0};
  D3D11_TEXTURE2D_DESC desc = {};
  desc.Width = 640;
  desc.Height = 480;
  desc.MipLevels = 1;
  desc.ArraySize = 1;
  desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  desc.SampleDesc.Count = 1;
  desc.Usage = D3D11_USAGE_DEFAULT; // ** [If this is immutable. it cannot be updated]
  desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

  hResult = device->CreateTexture2D(&desc, &initData, &m_texture);

  if (SUCCEEDED(hResult)) {
    D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
    SRVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    SRVDesc.Texture2D.MipLevels = 1;

    hResult =
        device->CreateShaderResourceView(m_texture, &SRVDesc, &m_textureView);
  }

  if (FAILED(hResult)) {
    OutputDebugStringA("Load png failed\n");
    return false;
  }

  return true;
}

void TextureClass::update(ID3D11DeviceContext* context) {
  uint32_t* pixels = new uint32_t[640 * 480];
  uint8_t temp = 0;
  for (unsigned int i = 0; i < 640 * 480; i++) {
    temp = i % 255;
    // RGBA
    pixels[i] = 0xFF + (0 << 8) + (temp << 16) + (0xFF << 24);
  }

  context->UpdateSubresource(m_texture, 0, nullptr, pixels, 0, 0);
}

void TextureClass::Shutdown() {
  // Release the texture view resource.
  if (m_textureView) {
    m_textureView->Release();
    m_textureView = 0;
  }

  // Release the texture.
  if (m_texture) {
    m_texture->Release();
    m_texture = 0;
  }

  // Release the targa data.
  if (m_targaData) {
    delete[] m_targaData;
    m_targaData = 0;
  }

  return;
}

ID3D11ShaderResourceView* TextureClass::GetTexture() { return m_textureView; }

bool TextureClass::LoadTarga(const char* filename, int& height, int& width) {
  int error, bpp, imageSize, index, i, j, k;
  FILE* filePtr;
  unsigned int count;
  TargaHeader targaFileHeader;
  unsigned char* targaImage;

  // Open the targa file for reading in binary.
  error = fopen_s(&filePtr, filename, "rb");
  if (error != 0) {
    return false;
  }

  // Read in the file header.
  count =
      (unsigned int)fread(&targaFileHeader, sizeof(TargaHeader), 1, filePtr);
  if (count != 1) {
    return false;
  }

  // Get the important information from the header.
  height = (int)targaFileHeader.height;
  width = (int)targaFileHeader.width;
  bpp = (int)targaFileHeader.bpp;

  // Check that it is 32 bit and not 24 bit.
  if (bpp != 32) {
    return false;
  }

  // Calculate the size of the 32 bit image data.
  imageSize = width * height * 4;

  // Allocate memory for the targa image data.
  targaImage = new unsigned char[imageSize];
  if (!targaImage) {
    return false;
  }

  // Read in the targa image data.
  count = (unsigned int)fread(targaImage, 1, imageSize, filePtr);
  if (count != imageSize) {
    return false;
  }

  // Close the file.
  error = fclose(filePtr);
  if (error != 0) {
    return false;
  }

  // Allocate memory for the targa destination data.
  m_targaData = new unsigned char[imageSize];
  if (!m_targaData) {
    return false;
  }

  // Initialize the index into the targa destination data array.
  index = 0;

  // Initialize the index into the targa image data.
  k = (width * height * 4) - (width * 4);

  // Now copy the targa image data into the targa destination array in the
  // correct order since the targa format is stored upside down.
  for (j = 0; j < height; j++) {
    for (i = 0; i < width; i++) {
      m_targaData[index + 0] = targaImage[k + 2];  // Red.
      m_targaData[index + 1] = targaImage[k + 1];  // Green.
      m_targaData[index + 2] = targaImage[k + 0];  // Blue
      m_targaData[index + 3] = targaImage[k + 3];  // Alpha

      // Increment the indexes into the targa data.
      k += 4;
      index += 4;
    }

    // Set the targa image data index back to the preceding row at the beginning
    // of the column since its reading it in upside down.
    k -= (width * 8);
  }

  // Release the targa image data now that it was copied into the destination
  // array.
  delete[] targaImage;
  targaImage = 0;

  return true;
}