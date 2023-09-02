#include"pch.h"
#include"Game.h"

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

	//setup the 
	DXGI_SWAP_CHAIN_DESC1 swapChainDescription = { 0 };

	swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// How the swap chain should be utilized
	swapChainDescription.BufferCount = 2;								//a fromt buffer and a back buffer
	swapChainDescription.Format = DXGI_FORMAT_B8G8R8A8_UNORM;			//a common swa chain format
	swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;	// he recommended flip mode
	swapChainDescription.SampleDesc.Count = 1;							//disable anti aliasing

	CoreWindow^ Window = CoreWindow::GetForCurrentThread();				// ONTAIN A pointer to the window

	HRESULT hr = dxgiFactory->CreateSwapChainForCoreWindow(
		device.Get(),
		reinterpret_cast<IUnknown*>(Window),
		&swapChainDescription,
		nullptr,
		&swapChain);

	/*if (FAILED(hr))
	{
		wchar_t errorMsg[256];
		swprintf_s(errorMsg, L"Swap Chain Creation Failed! HRESULT: 0x%X", hr);
		OutputDebugString(errorMsg);

		exit(1); // Terminate with an error code.
	}*/

}

//Perform Updated to the game state
void CGame::Update()
{
	
}

//Single frame Render code
void CGame::Render()
{
	//switch the back buffer and the front buffer
	swapChain->Present(1, 0);
}
