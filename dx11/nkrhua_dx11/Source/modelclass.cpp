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
	vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f); 
	vertices[0].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

	vertices[1].position = XMFLOAT3(-1.0f, 1.0f, 0.0f);;
	vertices[1].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[2].position = XMFLOAT3(1.0f, 1.0f, 0.0f);
	vertices[2].color = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

//	vertices[3].position = XMFLOAT3(1.0f, -1.0f, -1.5f);
//	vertices[3].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);


//	Load the Index Array with Data:
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

//	indices[3] = 3;
//	indices[4] = 0;
//	indices[5] = 2;


//	With the Vertex Array and Index Array filled out we can now use those to create the Vertex Buffer and Index Buffer.
//	Creating both buffers is done in the same fashion. First fill out a description of th buffer. In the description,
// 	the ByteWidth (size of the buffer) and the BindFlags (type of the buffer) are what you need to ensure are filled out
// 	correctly. Afte the description is filled out you need to also fill out a subresource pointer which will point to 
//	either your Vertex or Index Array you previously created. With the description and subresource pointer you can call 
//	CreateBuffer using the D3D device and it will return a pointer to your new buffer.

// 	Setup the description of the static vertex buffer:
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

//	Give the subresource structure a pointer to the Vertex Data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

//	Now create the Vertex Buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

//	Setup the description of the Static Index Buffer:
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

//	Create the subresource structure a pointer to the Index Data:
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

//	Create the Index Buffer:
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

//	After the Vertex Buffer and Index Buffer have been created you can delete the Vertex and Index arrays as
//	theyre no longer needed since the data was copied into the Buffers.
//	Release the Arrays now that the Vertex and Index Buffers have been created and loaded:
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

//	The ShutdownBuffer functions just releases the Vertex Buffer and Index 
//	Buffers that were created in the InitializeBuffers functions.

void ModelClass::ShutdownBuffers()
{
//	Release the Index Buffers:
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

//	Release the Vertex Buffer:
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}

//	Render Buffers is called from the Render function. The purpose of this function is to set the Vertex Buffer
//	and Index Buffers as active on the Input Assembler in the GPU. Once the GPU has an active Vertex Buffer
//	it can then use the Shader to Render that Buffer. This function also defines how those Buffers should be
//	drawn such as triangles, lines, fans and so forth. 
void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

//	Set Vertex Buffer Stride and Offset:
	stride = sizeof(VertexType);
	offset = 0;

//	Set the Vertex Buffer to active in the Input Assembler so it can be rendered:
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

//	Set the Index Buffer to active in the Input Assembler so it can be rendered:
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

//	Set the Type of Primitive that should be rendered from this VertexBuffer in this case triangles:
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}