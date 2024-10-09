#ifndef _COLORSHADERCLASS_H_
#define _COLORSHADERCLASS_H_

//	Includes:
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <fstream>
//	Namespaces:
using namespace DirectX;
using namespace std;

class ColorShaderClass
{
//	Here is the definition of the cBuffer type that will be used with the Vertex Shader.
//	This typedef must be exactly the same as the one in the Vertex Shader as the model data
//	needs to match the typedefs in the shader for proper rendering.
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

public:
	ColorShaderClass();
	ColorShaderClass(const ColorShaderClass&);
	~ColorShaderClass();

//	The function here handle initializing shutdown of the shader. The render function sets
//	the shader parameters and then draws the prepared model vertices using the shader.
	
	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX);

private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX);
	void RenderShader(ID3D11DeviceContext*, int);

	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
};

#endif