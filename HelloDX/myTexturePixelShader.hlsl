/////////////
// GLOBALS //
/////////////
Texture2D shaderTexture : TEXTURE : register(t0);
SamplerState SampleType : SAMPLER : register(s0);

//////////////
// TYPEDEFS //
//////////////
struct PixelInputType {
  float4 position : SV_POSITION;
  float2 tex : TEXCOORD;
};

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 TexturePixelShader(PixelInputType input) : SV_TARGET {
  float4 textureColor;

  // Sample the pixel color from the texture using the sampler at this texture
  // coordinate location.
  textureColor = shaderTexture.Sample(SampleType, input.tex);

  return textureColor;
}