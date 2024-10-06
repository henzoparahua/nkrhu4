//  The pixel shader draws each pixel on the polygons that will be rendered
//  to the screen. In this Pixel Shader it uses PixelInputType as input and
//  returns a float4 as output which represents the final pixel color. This
//  pixel shader program is very simple as we just tell it to color the pixel
//  the same as the input value of the color.

//  Typedefs:
struct PixelInputType
{
    float4 position : SV_Position;
    float4 color : COLOR;
};

//  Pixel Shader:
float4 ColorPixelShader(PixelInputType input) : SV_Target
{
    return input.color;
}