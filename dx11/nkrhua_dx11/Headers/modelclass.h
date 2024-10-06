#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_

#include <d3d11.h>
#include <directxmath.h>
using namespace DirectX;

class ModelClass
{
private:
//	Here is the definition of our Vertex Type that will be used with the Vertex Buffer in this ModelClass.
//	Also take note that this typedef must match the layout in the ColorShaderClass.
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

//	The function here handles Initializing and Shutdown of the model's vertex and index buffers. The Render
//	function puts the model geometry on the video card to prepare it for drawing by the color shader.
	bool Initialize(ID3D11Device*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

//	The private variables in the ModelClass are the Vertex and Index buffers as well as two integers to keep
//	track of the size of each buffer. Note that all DirectX 11 Buffers generally use the generic ID3D11Buffer type
//	and are more clearly identified by a buffer description wen they're first created.
private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	ID3D11Buffer* m_vertexBuffer, * m_indexBuffer;
	int m_vertexCount, m_indexCount;
};

#endif 
