#pragma once
#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_

#define WIN32_LEAN_AND_MEAN

//#define LIPS_SKELETON
#define REALSENSE
#define DEV

#include "graphics.h"
#include "input_class.h"
#include <windows.h>



#include "LIPSBodyPose.h"
#include "LIPSBodyPoseTypes.h"

#include <librealsense2/rs.hpp>

class SystemClass {
public:
  SystemClass();
  SystemClass(const SystemClass &);
  ~SystemClass();

  bool Initialize();
  void Shutdown();
  void Run();

  LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
  bool Frame();
  void InitializeWindows(int &, int &);
  void ShutdownWindows();

  uint8_t controlMode = 49;
  LPCWSTR m_applicationName;
  HINSTANCE m_hinstance;
  HWND m_hwnd;
  InputClass *m_Input;
  GraphicsClass *m_Graphics;
  std::shared_ptr<lips::LIPSBodyPose> mPoseEngine;

  #ifdef REALSENSE
  rs2::pipeline rsPipe;
  rs2::pointcloud rsPointCloud;
  rs2::points rsPoints;
  #endif
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static SystemClass *ApplicationHandle = 0;

#endif