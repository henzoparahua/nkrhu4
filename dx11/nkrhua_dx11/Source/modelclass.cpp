#include "../Headers/modelclass.h"

ModelClass::ModelClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
}

ModelClass::ModelClass(const ModelClass& other)
{

}

ModelClass::~ModelClass()
{

}

//	The Initialize function will call the initialization functions for the Vertex and Index Buffers.
bool ModelClass::Initialize(ID3D11Device* device)
{
	bool result;

//	Initialize the Vertex and Index Buffers:
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	return true;
}

//	The Shutdown function will call the Shutdown functions for the Vertex and Index Buffers.
void ModelClass::Shutdown()
{
//	Shutdown the Vertex and Index Buffers:
	ShutdownBuffers();

	return;
}

//	This function calls RenderBuffers to put the vertex buffers 
//	on the graphics pipeline so the color shader will be able to render them:
void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
//	Put the Vertex and index Buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}

//	GetIndexCount returns the number of indexes in the model. 
//	The Color Shader will need this information to draw this Model.
int ModelClass::GetIndexCount()
{
	return m_indexCount;
}

//	The InitializeBuffers function is where we handle creating the Vertex and Index Buffers.
//	Usually, you would read in a model and create the buffers from that data file.
bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

//	First create two temporary arrays to hold the Vertex and Index Data that we will use later:
//	Set the number of vertices in the Vertex Array:
	m_vertexCount = 3;

//	Set the number of indices in the Index Array:
	m_indexCount = 3;

//	Create the vertex Array:
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

//	Create the Index Array:
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

//	Now fill both the Vertex and Index array with the three points of the triangle as well as the index
//	to each of the points. Please note that I create the points in the clockwise order of drawing them.
//	If you do this counter clockwise it will think the triangle is facing the opposite direction and not
// 	draw it due to back face culling. Always remember that the order in which you send your vertices to
// 	the GPU is very important. The color is set here as well since it is part of the Vertex Description.

//	Load the Vertex Array with data:
	vertices[0].position = XMFLOAT3(-1.0f - 1.0f, 0.0f); 
	vertices[0].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[1].position = XMFLOAT3(0.0f, -1.0f, 0.0f);;
	vertices[1].color = XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);

	vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);
	vertices[2].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

//	Load the Index Array with Data:
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

//	With the Vertex Array and Index Array filled out we can now use those to create the Vertex Buffer and Index Buffer.
//	Creating both buffers is done in the same fashion. First fill out a description of th buffer. In the description,
// 	the ByteWidth (size of the buffer) and the BindFlags (type of the buffer) are what you need to ensure are filled out
// 	correctly. Afte the description is filled out you need to also fill out a subresource pointer which will point to 
//	either your Vertex or Index Array you previously created. With the description and subresource pointer you can call 
//	CreateBuffer using the D3D device and it will return a pointer to your new buffer.

// 	Setup the description of the static vertex buffer:


}