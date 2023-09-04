
cbuffer constBuffer
{
    float RedLevel;
    float BlueLevel;
};

struct VOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};
	
	
VOut main( float4 position : POSITION , float4 color : COLOR )
{
	//create the struct 
    VOut Output;
    

	//Manipulat the positon befpe passing to other shaders
    Output.position = position;
    
    
    //Manipulate the color before passign to the other shaders 
    Output.color = color;
    Output.color.r *= RedLevel;
    Output.color.b *= BlueLevel;
    
    //float4 a, b;

    //mul(a,b);
    //pow(a,b);
    //sin(a);
    //tan(a);
    //saturate(a);
	
    
    return Output;
}