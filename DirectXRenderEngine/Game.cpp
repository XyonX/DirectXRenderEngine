#include"pch.h"
#include"Game.h"
#include <fstream>

/*
Array<byte>^ LoadShaderFile(std::string File)
{
	Array<byte>^ FileData = nullptr;

	//open the file 
	std::ifstream VertexFile(File, std::ios::in | std::ios::binary | std::ios::ate);
	//check wheater file open suicessfully

	if (VertexFile.is_open())
	{
		//find the file length
		int length = (int)VertexFile.tellg();

		//collect the data
		FileData = ref new Array<byte>(length);
		VertexFile.seekg(0, std::ios::beg);
		VertexFile.read(reinterpret_cast<char*>(FileData->Data), length);
		VertexFile.close();

	}
	return FileData;
}*/


// this function loads a file into an Array^
Array<byte>^ LoadShaderFile(std::string File)
{
	Array<byte>^ FileData = nullptr;

	// open the file
	std::ifstream VertexFile(File, std::ios::in | std::ios::binary | std::ios::ate);

	// if open was successful
	if (VertexFile.is_open())
	{
		// find the length of the file
		int Length = (int)VertexFile.tellg();

		// collect the file data
		FileData = ref new Array<byte>(Length);
		VertexFile.seekg(0, std::ios::beg);
		VertexFile.read(reinterpret_cast<char*>(FileData->Data), Length);
		VertexFile.close();
	}

	return FileData;
}


//Initializes and perform Direct3D
void CGame::Initialize()
{
	ComPtr<ID3D11Device>tempDevice;
	ComPtr<ID3D11DeviceContext>tempDeviceContext;

	//Create the device and the context object
	D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &tempDevice, nullptr, &tempDeviceContext);

	//Convert the pointers from DX11 version to DX11.2 version
	tempDevice.As(&device);
	tempDeviceContext.As(&deviceContext);

	//1. convertthe D2D11Device1 to IDXGIDevice1
	ComPtr<IDXGIDevice1> dxgiDevice;
	device.As(&dxgiDevice);

	//2.utilize the IDXGIDevice1 to get access the adapter
	ComPtr<IDXGIAdapter> dxgiAdapter;
	dxgiDevice->GetAdapter(&dxgiAdapter);

	//3.use the IDXGIAdpter interface to get access to the parent
	ComPtr<IDXGIFactory2> dxgiFactory;
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), &dxgiFactory);

	/// SETTING UP THE SWAP CHAIN

	//setup the 
	DXGI_SWAP_CHAIN_DESC1 swapChainDescription = { 0 };

	swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//How the swap chain should be utilized
	swapChainDescription.BufferCount = 2;								//a fromt buffer and a back buffer
	swapChainDescription.Format = DXGI_FORMAT_B8G8R8A8_UNORM;			//a common swa chain format
	swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;	//recommended flip mode
	swapChainDescription.SampleDesc.Count = 1;							//disable anti aliasing

	CoreWindow^ Window = CoreWindow::GetForCurrentThread();				// Obtain A pointer to the window

	/// CREATING HE SWAPCHAIN

	HRESULT hr = dxgiFactory->CreateSwapChainForCoreWindow(
		device.Get(),
		reinterpret_cast<IUnknown*>(Window),
		&swapChainDescription,
		nullptr,
		&swapChain);

	//error handling code 
	if (FAILED(hr))
	{
		wchar_t errorMsg[256];
		swprintf_s(errorMsg, L"Swap Chain Creation Failed! HRESULT: 0x%X", hr);
		OutputDebugString(errorMsg);

		exit(1); // Terminate with an error code.
	}

	///SETTING UP THE BACKBUFFER WITH SWAPCHAIN

	// get a direct poionter to the back buffer
	ComPtr<ID3D11Texture2D> backBuffer;
	swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backBuffer);

	//create a render targer that points to out back buffer
	device->CreateRenderTargetView(backBuffer.Get(), nullptr, &renderTarget);


	/// SETTING UP THE VIEWPORT 

	//set the viewport
	D3D11_VIEWPORT viewport = { 0 };
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = Window->Bounds.Width;
	viewport.Height = Window->Bounds.Height;

	deviceContext->RSSetViewports(1, &viewport);


	/// CALLING THE GRAPHIC INITIALIZATION FILE
	InitGraphics();
	InitPipeline();

	//set the time to -0
	time = 0;

}

void CGame::InitGraphics()
{
	/// SETTING UP THE VERTEX DATA 

	VERTEX Vertices[] =
	{
		{0.0f,0.5f,0.0f		, 1.0f,0.0f,0.0f},
		{0.5f,-0.5f,0.0f	,0.0f,1.0f,0.0f },
		{-0.5f,-0.5f,0.0f	,0.0f,0.0f,1.0f},
	};

	///SETTING UP THE VERTEX  BUFFER

	// creating the buffer description
	D3D11_BUFFER_DESC bufferDesc = { 0 };

	// the size of the buffer we want to make 
	bufferDesc.ByteWidth = sizeof(VERTEX) * ARRAYSIZE(Vertices);

	// what kind of buffer we want 
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	//
	D3D11_SUBRESOURCE_DATA subResourceData = { Vertices , 0 , 0 };

	//creating the buffer and storing in the vertex bufffer com object
	device->CreateBuffer(&bufferDesc, &subResourceData, &vertexBuffer);

}

//initializes the gpu and prepares direct3d for rendering
void CGame::InitPipeline()
{

	///CREATING SHADERS

	//load shader file
	Array<byte>^ VSFile = LoadShaderFile("VertexShader.cso");

	//load shader file
	Array<byte>^ PSFile = LoadShaderFile("PixelShader.cso");

	//create the shader object
	device->CreateVertexShader(VSFile->Data, VSFile->Length, nullptr, &vertexShader);
	device->CreatePixelShader(PSFile->Data, PSFile->Length, nullptr, &pixelShader);

	//set the shader as active shader
	deviceContext->VSSetShader(vertexShader.Get(), nullptr, 0);
	deviceContext->PSSetShader(pixelShader.Get(), nullptr, 0);

	/// CREATING INPUT LAYOUT

	//initialize input layout
	D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},

	};
	//creating the input layout 
	device->CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), VSFile->Data, VSFile->Length, &inputLayout);

	//set the input layout 
	deviceContext->IASetInputLayout(inputLayout.Get());

	///setup const buffers

	D3D11_BUFFER_DESC bufferDesc = { 0 };
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = 64;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	
	device->CreateBuffer(&bufferDesc, nullptr, &constBuffer);
	deviceContext->VSSetConstantBuffers(0, 1, constBuffer.GetAddressOf());


}

//Perform Updated to the game state
void CGame::Update()
{
	time += 0.05f;
}

//Single frame Render code
void CGame::Render()
{
	//set the render target as active rendeer target
	deviceContext->OMSetRenderTargets(1, renderTarget.GetAddressOf(), nullptr);

	///SETTING UP A COLOR FOR THE BACKGROUND

	//clear the back buffer to some color
	float color[4] = { 0.219f,0.290f,0.431f,0.8f };
	deviceContext->ClearRenderTargetView(renderTarget.Get(), color);

	/// SETTING UP THE VERTEX BUFFER 
	//seting up vertex buffer
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);



	//settin up the premitive topology
	deviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	///	Setup the new value for the Const Buffer

	//COLORMOD Colors;
	//Colors.RedLevel = 0.5f;
	//Colors.BlueLevel = 0.5f;
	////set the new value for the const buffer
	//deviceContext->UpdateSubresource(constBuffer.Get(), 0, 0, &Colors, 0, 0);

	OFFSET Offset;
	//Offset.X = .25;
	//Offset.Y = .3;
	//Offset.Z = .4;
	

	///MAKING THE MESH DATA

	//calculate the world transformation
	XMMATRIX matWorld = XMMatrixRotationY(time);
	
	//calculate the view transformation
	XMVECTOR camPosition = XMVectorSet(1.5f, 0.5f, 1.5f, 0.0f);
	XMVECTOR camLookAt = XMVectorSet(0.0f, 0.0f, 0.0f,0.0f);
	XMVECTOR camUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMMATRIX matView = XMMatrixLookAtLH(camPosition, camLookAt, camUp);

	//calculate the projection transformation
	CoreWindow^Window = CoreWindow::GetForCurrentThread();

	XMMATRIX matProjection = XMMatrixPerspectiveLH(XMConvertToRadians(45),
		(FLOAT)Window->Bounds.Width / (FLOAT)Window->Bounds.Height,
		1.0f,
		100.0f);

	//calculate the final matrix
	XMMATRIX matFinal = matWorld * matView * matProjection;


	///send the data to the const buffers

	deviceContext->UpdateSubresource(constBuffer.Get(), 0, 0, &matFinal, 0, 0);


	deviceContext->Draw(3, 0);




	//switch the back buffer and the front buffer
	swapChain->Present(1, 0);
}
