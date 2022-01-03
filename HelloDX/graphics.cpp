#include "graphics.h"

GraphicsClass::GraphicsClass() {
	m_Direct3D = 0;
	m_Camera = 0;
	m_Model = 0;
	m_ColorShader = 0;
}

GraphicsClass::GraphicsClass(const GraphicsClass& other) {}

GraphicsClass::~GraphicsClass() {}

bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd) {
	bool result;

	// Create the Direct3D object.
	m_Direct3D = new D3DClass;
	if (!m_Direct3D) {
		return false;
	}

	// Initialize the Direct3D object.
	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED,
		hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new Camera;
	if (!m_Camera) {
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -2.0f);

	// Set the initial rotation of the camera.
	m_Camera->SetRotation(0.0f, 0.0f, 0.0f);

	// Create the model object.
	m_Model = new Model;
	result = m_Model->Initialize(m_Direct3D->GetDevice());
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error",
			MB_OK);
		return false;
	}

	m_modelFloor = new ModelFloor();
	m_modelFloor->Initialize(m_Direct3D->GetDevice());

	m_modelAxis = new ModelAxis();
	m_modelAxis->Initialize(m_Direct3D->GetDevice());

	m_modelSkeleton = new ModelSkeleton();
	m_modelSkeleton->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext());

	m_modelPointsCloud = new ModelPointCloud();
	m_modelPointsCloud->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext());

	// Create the color shader object.
	m_ColorShader = new MyShader;
	if (!m_ColorShader) {
		return false;
	}

	// Initialize the color shader object.
	result = m_ColorShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error",
			MB_OK);
		return false;
	}

	return true;
}

void GraphicsClass::Shutdown() {
	// Release the color shader object.
	if (m_ColorShader) {
		m_ColorShader->Shutdown();
		delete m_ColorShader;
		m_ColorShader = 0;
	}

	// Release the model object.
	if (m_Model) {
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	if (m_modelFloor) {
		m_modelFloor->Shutdown();
		delete m_modelFloor;
		m_modelFloor = 0;
	}

	if (m_modelAxis) {
		m_modelAxis->Shutdown();
		delete m_modelAxis;
		m_modelAxis = 0;
	}

	if (m_modelSkeleton) {
		m_modelSkeleton->Shutdown();
		delete m_modelSkeleton;
		m_modelSkeleton = 0;
	}

	if (m_modelPointsCloud) {
		m_modelPointsCloud->Shutdown();
		delete m_modelPointsCloud;
		m_modelPointsCloud = 0;
	}

	// Release the camera object.
	if (m_Camera) {
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the Direct3D object.
	if (m_Direct3D) {
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}

	return;
}

bool GraphicsClass::Frame() {
	bool result;

	// Render the graphics scene.
	return Render();
}

/*
  [主要render流程在這]
*/
bool GraphicsClass::Render() {
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	// Clear the buffers to begin the scene.
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d
	// objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// Put the model vertex and index buffers on the graphics pipeline to prepare
	// them for drawing. Render the model using the color shader.
	m_Model->Render(m_Direct3D->GetDeviceContext());
	result = m_ColorShader->Render(m_Direct3D->GetDeviceContext(),
		m_Model->GetIndexCount(), worldMatrix,
		viewMatrix, projectionMatrix);

	m_modelFloor->Render(m_Direct3D->GetDeviceContext());
	result = m_ColorShader->Render(m_Direct3D->GetDeviceContext(),
		m_modelFloor->GetIndexCount(), worldMatrix,
		viewMatrix, projectionMatrix);

	m_modelAxis->Render(m_Direct3D->GetDeviceContext());
	result = m_ColorShader->Render(m_Direct3D->GetDeviceContext(),
		m_modelAxis->GetIndexCount(), worldMatrix,
		viewMatrix, projectionMatrix);

#ifdef LIPS_SKELETON
	m_modelSkeleton->Render(m_Direct3D->GetDeviceContext());
	result = m_ColorShader->Render(m_Direct3D->GetDeviceContext(),
		m_modelSkeleton->GetIndexCount(), worldMatrix,
		viewMatrix, projectionMatrix);
#endif

#ifdef REALSENSE
	m_modelPointsCloud->Render(m_Direct3D->GetDeviceContext());
	result = m_ColorShader->Render(m_Direct3D->GetDeviceContext(),
		m_modelPointsCloud->GetIndexCount(), worldMatrix,
		viewMatrix, projectionMatrix);
#endif

	if (!result) {
		return false;
	}

	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return true;
}
