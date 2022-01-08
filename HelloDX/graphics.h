#pragma once
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

#include <windows.h>

#include "camera.h"
#include "d3d.h"
#include "model.h"
#include "modelAxis.hpp"
#include "modelFloor.hpp"
#include "modelPointCloud.hpp"
#include "modelSkeleton.hpp"
#include "myShader.h"
#include "modelTest.hpp"

#include "textureShader.h"
#include "modelWithTexture.h"

//#define LIPS_SKELETON
#define REALSENSE
#define DEV


const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class GraphicsClass {
 public:
  GraphicsClass();
  GraphicsClass(const GraphicsClass &);
  ~GraphicsClass();

  bool Initialize(int, int, HWND);
  void Shutdown();
  bool Frame();

  Camera *m_Camera;
  ModelSkeleton *m_modelSkeleton;
  ModelPointCloud *m_modelPointsCloud;
  ModelTest *m_modelTest;

 private:
  bool Render();

  D3DClass *m_Direct3D;
  Model *m_Model;
  ModelWithTexture *m_ModelWithTexture; 
  ModelFloor *m_modelFloor;
  ModelAxis *m_modelAxis;
  MyShader *m_ColorShader;
  TextureShaderClass *m_TextureShader;
};

#endif
