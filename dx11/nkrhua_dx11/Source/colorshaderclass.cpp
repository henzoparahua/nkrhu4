#include "../Headers/colorshaderclass.h"

ColorShaderClass::ColorShaderClass()
{
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_matrixBuffer = 0;
}

ColorShaderClass::ColorShaderClass(const ColorShaderClass& other)
{

}

ColorShaderClass::~ColorShaderClass()
{

}

//	The initialize function will call the initialization function for the shaders.
//	We pass in the name of the HLSL shader files.
bool ColorShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;
	wchar_t vsFilename[128];
	wchar_t psFilename[128];
	int error;

//	Set the filename of the Vertex Shader:
	error = wcscpy_s(vsFilename, 128, L"./Source/color.vs");
	if (error != 0)
	{
		return false;
	}

//	Set the filename of the Pixel Shader:
	error = wcscpy_s(psFilename, 128, L"./Source/color.ps");
	if (error != 0)
	{
		return false;
	}

//	Initialize the Vertex and Pixel Shaders:
	result = InitializeShader(device, hwnd, vsFilename, psFilename);
	if (!result)
	{
		return false;
	}

	return true;
}

//	The Shutdown function will call the shutdown of the shader.
void ColorShaderClass::Shutdown()
{
//	Shutdown the Vertex and Pixel Shader as well as the related objects:
	ShutdownShader();

	return;
}

//	Render will first set the parameters inside the Shader using the SetShaderParameters function.
//	Once the parameters are set it then calls RenderShader to draw the green triangle using the HLSL Shader.
bool ColorShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix,
	XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	bool result;

//	Set the shader parameters that will be used for rendering:
	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

//	Now render the prepared buffer with the shader:
	RenderShader(deviceContext, indexCount);

	return true;
}

//	Now we will start with one of the more important functions called InitializeShader.
//	The function is what actually loads the shader files and makes it usable to DirectX and GPU.
bool ColorShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;

//	Initialize the pointers this function will use to null:
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

//	Here is where we compile the shader programs into buffers. We give it the name of the Shader file,
//	the name of the shader, the shader version (5.0 in DirectX 11), and the Buffer to compile the Shader
// 	into. If it fails compiling the Shader it will put an error message inside the errorMessage string which
// 	we send to another function to write out the error. If it stills fails and there is no errorMessage string
// 	then it means it could not find the Shader file in which case we pop up a dialog box saying so.

//	Compile the Vertex Shader color:
	result = D3DCompileFromFile(vsFilename, NULL, NULL, "ColorVertexShader", 
		"vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
//	If the Shader failed to compile it should have writen something to the Error Message.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		}
//	If there was nothing in the error message then it simply could not find the Shader itself.
		else
		{
			MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

//	Compile the Pixel Shader Code:
	result = D3DCompileFromFile(psFilename, NULL, NULL, "ColorPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
	//	If the Shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
		}
	//	If there was nothing in the error message then it simply could not find the file itself.
		else
		{
			MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

//	Once the Vertex Shader and Pixel Shader code has successfully compiled into buffers,
//	we then use those Buffers to create the Shader objects themselves. We will use these
// 	pointers to interface with the Vertex and Pixel Shader from this point forward.

//	Create the Vertex Shader from the Buffer:
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(result))
	{
		return false;
	}

//	Create the Pixel Shader from the Buffer:
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(result))
	{
		return false;
	}

//	The next step is to create the Layout of the Vertex Data that will be processed by the Shader.
//	As this shader uses a position and color vector, we need to create both in the layout specifying
// 	the size of both. The semantic name is the first thing to fill out in the layout, this allows the
// 	shader to determine the usage of this element of the layout. As we have two different elements, we
// 	use the POSITION for the first one and COLOR for the second. 
// 	The next important part is the Format. For the Position Vector we use DXGI_FORMAT_R32G32B32_FLOAT and
// 	for the color we use DXGI_FORMAT_R32G32B32A32_FLOAT. The final thing is the AlignedByteOffset which
// 	indicates how the data is spaced in the Buffer. For this layout we are telling it the first 12 bytes
// 	are POSITION and the next 16 will be COLOR. You can also use the D3D11_APPEND_ALIGNED_ELEMENT instead
// 	of placing your own values in AlignedByteOffset and it will figure out spacing for you.

//	Create the Vertex Input Layout description:
//	This Setup needs to match the VertexType structure in the ModelClass and in the Shader:
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

//	Once the layout description has been setup, we can get the size of it and then create
//	the input layout using the D3D device. Also release the Vertex and Pixel Shader Buffers
// 	since they're not longer needed once the layout has been created.

//	Get a count of the elements in the Layout:
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

//	Create the Vertex Input Layout:
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_layout);
	if (FAILED(result))
	{
		return false;
	}

//	Release the Vertex Shader Buffer and Pixel Shader Buffer since theyre no longer needed:
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

//	The final thing that needs to be setup to utilize the Shader is the Constant Buffer. As you
//	saw in the Vertex Shader, we currently have just one constant buffer so we only need to setup
//	one here so we can interface the shader. The Buffer usage needs to be set to dynamic since we
//	will be updating it each frame. The bind flags indicate that this buffer will be a Constant Buffer.
//	The CPU access flags need to match up with the usage so it is set to D3D11_CPU_ACCESS_WRITE.
//	Once we fill out the description, we can then create the Constant Buffer Interface and then use that
//	to access the internal variables in the Shader using the function SetShaderParameters.

//	Setup the description of the Dynamic Matrix Constant Buffer that is in the Vertex Shader:
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

//	Create the Constant Buffer Pointer so we can access the	Vertex Shader 
//	Constant buffer from within this class:
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void ColorShaderClass::ShutdownShader()
{
//	Release the Matrix Buffer Constant:
	if (m_matrixBuffer)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
	}
//	Release the Layout
	if (m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}
//	Release the Pixel Shader:
	if (m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = 0;
	}
//	Release the Vertex Shader:
	if (m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = 0;
	}

	return;
}

//	The OutputShaderErrorMessage writes out error messages that are
//	generating when compiling either Vertex Shaders or Pixel Shaders:
void ColorShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long long bufferSize, i;
	ofstream fout;

//	Get a pointer to the error message text buffer:
	compileErrors = (char*)(errorMessage->GetBufferPointer());

//	Get the length of the Message:
	bufferSize = errorMessage->GetBufferSize();

//	Open a file to write the error message to:
	fout.open("shader-error.txt");

//	Write out the error message:
	for (i = 0; i < bufferSize; i++)
	{
		fout << compileErrors[i];
	}

//	Close the file:
	fout.close();

//	Release the Error Message:
	errorMessage->Release();
	errorMessage = 0;

//	Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, L"Error compiling shader. Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;
}

//	The SetShaderVariables function exists to make setting the global variables in the shader easier.
//	The matrices used in this function are created inside the ApplicationClass, after which this function
//	is called to send them from there into the Vertex Shader during the Render Function call.

bool ColorShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, 
	XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPTR;
	unsigned int bufferNumber;

//	Make sure to transpose matrices before sending them into the shader, this is a requirement for DirectX 11.
//	Transpose the matrices to prepare them for the Shader:
	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);

//	Lock the m_matrixBuffer, set the new Matrices inside it, and then unlock it.
//	Lock the Constant Buffer so it can be written to:
	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

//	Get a pointer to the data in the Constant Buffer:
	dataPTR = (MatrixBufferType*)mappedResource.pData;

//	Copy the matrices into the Constant Buffer:
	dataPTR->world = worldMatrix;
	dataPTR->view = viewMatrix;
	dataPTR->projection = projectionMatrix;

//	Unlock the Constant Buffer:
	deviceContext->Unmap(m_matrixBuffer, 0);

//	Now set the Update Matrix Buffer in the HLSL Vertex Shader:
	bufferNumber = 0;

//	Finaly set the Constant Buffer in the Vertex Shade with the Updated Values:
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

	return true;
}

//	RenderShader is the second function called in the Render Function. 
//	SetShaderParameters is called before this to ensure the Shader Parameters are setup correctly.
//	The first step in this function is to set our Input Layout to active in the Input Assembler.
//	This lets the GPU know the format of the Vertex Buffer. The second step is to set the Vertex
//	Shader and Pixel Shader we will be using to render this Vertex Buffer. Once the Shaders are set
//	we render the triangle by calling the DrawIndexed DirectX 11 function using the D3D Device Context.
//	Once this function is called it will render the green triangle.
void ColorShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
//	Set the Vertex Input Layout:
	deviceContext->IASetInputLayout(m_layout);

//	Set the Vertex and Pixel Shaders that will be used to render this triangle.
	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);

//	Render the triangle:
	deviceContext->DrawIndexed(indexCount, 0, 0);

	return;
}