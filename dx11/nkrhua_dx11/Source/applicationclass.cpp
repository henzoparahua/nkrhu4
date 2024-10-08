#include "../Headers/applicationclass.h"

ApplicationClass::ApplicationClass()
{
	m_Direct3D = 0;
	m_Camera = 0;
	m_Model = 0;
	m_ColorShader = 0;
}

ApplicationClass::ApplicationClass(const ApplicationClass& other)
{

}

ApplicationClass::~ApplicationClass()
{

}

bool ApplicationClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	m_Direct3D = new D3DClass;
	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

//	Create the Camera Object:
	m_Camera = new CameraClass;

//	Set the Initial Position of the Camera:
	m_Camera->SetPosition(0.0f, 0.0f, -5.0f);

//	Create and Initialize the Model Class:
	m_Model = new ModelClass;
	
	result = m_Model->Initialize(m_Direct3D->GetDevice());
	if (!result)
	{
		MessageBox(hwnd, L"Could not open the initializer of model object", L"Error", MB_OK);
		return false;
	}

//	Create and Initialize the Color Shader Object:
	m_ColorShader = new ColorShaderClass;
	
	result = m_ColorShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the Color Shader Objects", L"Error", MB_OK);
		return false;
	}

	return true;
}

void ApplicationClass::Shutdown()
{
	if (m_ColorShader)
	{
		m_ColorShader->Shutdown();
		delete m_ColorShader;
		m_ColorShader = 0;
	}

	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}
			
	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}

	return;
}

bool ApplicationClass::Frame()
{
	bool result;

	result = Render();
	if (!result)
	{
		return false;
	}

	return true;
}

//	It still b egins with clearing the scene except that it is cleared to black. After that it calls the
//	Render function for the Cameraobject to create a View Matrix based on the Camera's location that was set
//	in the Initialize function. Once the view matrix is created, we get a copy of it from the Camera Class
//	We also get copies of the world and projection matrix from the D3DClass object. We then call the 
//	ModelClass::Render function to put the green triangle model geometry on the graphics pipeline. With the
//	vertices now prepared we call the Color Shader to draw the Vertices using hte Model Information and the
//	three matrices for positioning each vertex. The green triangle is now drawn to the Back Buffer. With that
//	the scene is complete and we call EndScene to display it to the green.2
bool ApplicationClass::Render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

//	Clear the buffers to begin the scene:
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

//	Generate the View Matrix based on the Camera's Position:
	m_Camera->Render();

//	Get the world, view and projection matrices from the camera and d3d objects:
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

//	Put the Model Vertex and Index Buffers on the Graphics Pilepine to prepare them for drawing:
	m_Model->Render(m_Direct3D->GetDeviceContext());

//	Render the model using the Color Shader:
	result = m_ColorShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	m_Direct3D->EndScene();
	
	return true;
}