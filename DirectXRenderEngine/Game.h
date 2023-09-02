#pragma once
#include <wrl.h>
#include "windows.graphics.directx.h" 

using namespace Microsoft::WRL;
using namespace Windows::UI::Core;

class CGame
{

public:
	ComPtr<ID3D11Device1>device;					// the device interface - virtual reoresentation of the video adapter
	ComPtr<ID3D11DeviceContext1>deviceContext;		// the control panel for the gpu
	ComPtr<IDXGISwapChain1> swapChain;				// the swap chain interface

	void Initialize();	//starting up code
	void Update();		//Code that manipulate game
	void Render();		//Draw graphics
};
