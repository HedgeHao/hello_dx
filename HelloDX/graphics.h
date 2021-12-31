#pragma once
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

#include "camera.h"
#include "d3d.h"
#include "model.h"
#include "modelFloor.hpp"
#include "myShader.h"
#include <windows.h>

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

private:
  bool Render();

  D3DClass *m_Direct3D;
  Model *m_Model;
  ModelFloor *m_modelFloor;
  MyShader *m_ColorShader;
};

#endif
