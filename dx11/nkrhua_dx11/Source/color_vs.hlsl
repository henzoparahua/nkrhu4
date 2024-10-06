//  In the shader programs you begin with the global variables.
//  These globals can be modified externally from your C++ code.
//  Generally, you will put most global variables in buffer object types called "cbuffer"
//  even if is just a single global variable. Logically, organizing these buffers is important
//  for efficient execution of the shaders as well as how the graphics card will store the buffers.

// Globals:
cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

//  We will use different types such as float4 that are avaliable to HLSL, which make programming
//  shaders easier and readable. In this example we are creating types that have x y z w position 
//  vestors and red green blue alpha colors. the POSITION COLOR and SV_POSITION are semantics that
//  convey to the GPU the use of the variable. There are two different structures here since the
//  semantics are different for vertex and pixel shaders even though the structure are the same.
//  POSIION works for pixel shaders while COLOR works for both.

//  Typedefs:
struct VertexInputType
{
    float4 position : POSITION;
    float4 color : COLOR;  
};

struct PixelInputType
{
    float4 position : SV_Position;
    float4 color : COLOR;
};

//  The vertex shader is called by the GPU when it is processing data from the vertex buffers
//  that have been sent to it. This vertex shader which is named ColorVertexShader will be 
//  called for every single vertex in the Vertex Buffer. The input to the vertex shader must 
//  match the data format in the vertex buffer as well as the type definition in the shader 
//  source file which in this case is VertexInputType. The output of the vertex shader will
//  be sent to the pixel __XB_GetShaderUserData. In this case the output type is called 
//  PixelInputType which is defined above as well.
//  WIth that in mind you see that the Vertex Shader creates an output variable that is called
//  PixelInputType type. It then takes the position of the Input Vertex and multiplies it by
//  the worldMatrix, view and then Projection Matrices. This will place the vertex in the correct
//  This will place the vertex in the correct location for rendering in 3D space according to our
//  view and then onto the 2D screen. After that the output variable takes a copy of the Input Color
//  and then returns the output which will be used as input to the Pixel Shader. 

//  Vertex Shader:
PixelInputType ColorVertexShader(VertexInputType input)
{
    PixelInputType output;
    
//  Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;
    
//  Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
//  Store the input color for the pixel shader to use.
    output.color = input.color;
    
    return output;
};