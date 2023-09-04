
struct VOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};
	
	
VOut main( float4 position : POSITION , float4 color : COLOR )
{
	//create the struct 
    VOut Output;
	
    Output.position = position;
    Output.color = 1-color;
	
    
    return Output;
}