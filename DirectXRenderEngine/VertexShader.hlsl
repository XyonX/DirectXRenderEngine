
cbuffer constBuffer
{
    float4x4 matFinal;
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
    //Output.position = mul(matFinal ,position);
    Output.position = position;
    
    
    //Manipulate the color before passign to the other shaders 
    Output.color = color;

    //float4 a, b;

    //mul(a,b);
    //pow(a,b);
    //sin(a);
    //tan(a);
    //saturate(a);
	
    
    return Output;
}