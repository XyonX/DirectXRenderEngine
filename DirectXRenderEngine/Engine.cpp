//precompiler header
#include "pch.h"
#include "Game.h"

//inbclude common namespaces 
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Core;
using namespace Windows::UI::Popups;
using namespace Windows::System;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;
using namespace Platform;

// class definition for the core frameword of this engine
ref class Engine sealed : public IFrameworkView
{
	bool didCloseWindow;
	CGame Game;

public:
	
	virtual void Initialize(CoreApplicationView^ appView)
	{
		// subscribe OnActivated function to handle the activated event code handl
		appView->Activated += ref new TypedEventHandler <CoreApplicationView^, IActivatedEventArgs^>(this, &Engine::OnActivated);
		
		//settign up the bool to false 
		didCloseWindow = false;
		
		CoreApplication::Suspending += ref new EventHandler<SuspendingEventArgs^>(this, &Engine::OnSuspending);
		CoreApplication::Resuming += ref new EventHandler<Object^>(this, &Engine::OnResuming);
	}
	virtual void SetWindow(CoreWindow^Window)
	{
		Window->PointerPressed += ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &Engine::PointerPressed);
		Window->KeyDown += ref new TypedEventHandler<CoreWindow^,KeyEventArgs^>(this, &Engine::OnKeyDown);
		Window->PointerWheelChanged += ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &Engine::OnPointerWheelChanged);
		Window->Closed += ref new Windows::Foundation::TypedEventHandler<CoreWindow^, CoreWindowEventArgs^>(this, &Engine::OnClosed);
	}
	virtual void Load(String^EntryPoint){}
	virtual void Run()
	{
		Game.Initialize();

		//obtain a pointer to the window
		CoreWindow^Window = CoreWindow::GetForCurrentThread();


		//Run the window until the bool gets triggere
		while (!didCloseWindow)
		{
			//run processEvents to dispatch
			Window->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);

			//calling update and render continously
			Game.Update();
			Game.Render();
		}

	}
	virtual void Uninitialize(){}

	// and event is called when the applcation window is ready to be activated
	void OnActivated(CoreApplicationView^sender , IActivatedEventArgs^args)
	{
		CoreWindow^ Window = CoreWindow::GetForCurrentThread();
		Window->Activate();
	}
	void PointerPressed(CoreWindow^Windw,PointerEventArgs^Args)
	{
		MessageDialog Dialog("", "");
		Dialog.Content = Args->CurrentPoint->FrameId.ToString();
		Dialog.Title = "Notice !";
		Dialog.ShowAsync();
	
	}
	void OnKeyDown(CoreWindow^ sender, KeyEventArgs^ args)
	{
		MessageDialog Dialog("", "");

		if (args->VirtualKey == VirtualKey::W)
		{
			//Dialog.Content = "Move Forward";
			//Dialog.Title = "W Pressed !";
			//Dialog.ShowAsync();

			// Load the current camPosition into a temporary variable
			XMVECTOR tempPosition = Game.camPosition;

			// Increase the X component by a desired amount (e.g., 0.1f)
			tempPosition = XMVectorAdd(tempPosition, XMVectorSet(0.0f, 0.0f, -0.1f, 0.0f));

			// Store the modified position back into camPosition
			Game.camPosition = tempPosition;
			//OutputDebugString(L"Debug message\n");

			//XMVECTOR vectorToPrint = Game.camPosition;

			//// Create a buffer to store the formatted string
			//char buffer[100]; // Adjust the size according to your needs

			//// Format the XMVECTOR and store it in the buffer
			//sprintf_s(buffer, sizeof(buffer), "Vector: X=%.2f, Y=%.2f, Z=%.2f, W=%.2f", XMVectorGetX(vectorToPrint), XMVectorGetY(vectorToPrint), XMVectorGetZ(vectorToPrint), XMVectorGetW(vectorToPrint));

			//// Output or display the formatted string
			//OutputDebugStringA(buffer); // Use OutputDebugStringA for ANSI strings
			
		}

		else if (args->VirtualKey == VirtualKey::S)
		{
			//Dialog.Content = "Move Left";
			//Dialog.Title = "S Pressed !";
			//Dialog.ShowAsync();

						// Load the current camPosition into a temporary variable
			XMVECTOR tempPosition = Game.camPosition;

			// Increase the X component by a desired amount (e.g., 0.1f)
			tempPosition = XMVectorAdd(tempPosition, XMVectorSet(0.0f, 0.0f, 0.1f, 0.0f));

			// Store the modified position back into camPosition
			Game.camPosition = tempPosition;

		}
		else if (args->VirtualKey == VirtualKey::A)
		{
			//Dialog.Content = "Move Backward";
			//Dialog.Title = "A Pressed !";
			//Dialog.ShowAsync();

			XMVECTOR tempPosition = Game.camPosition;

			// Increase the X component by a desired amount (e.g., 0.1f)
			tempPosition = XMVectorAdd(tempPosition, XMVectorSet(-0.1f, 0.0f, 0.0f, 0.0f));

			// Store the modified position back into camPosition
			Game.camPosition = tempPosition;


		}

		else if (args->VirtualKey == VirtualKey::D)
		{
			//Dialog.Content = "Right Forward";
			//Dialog.Title = "D Pressed !";
			//Dialog.ShowAsync();


			XMVECTOR tempPosition = Game.camPosition;

			// Increase the X component by a desired amount (e.g., 0.1f)
			tempPosition = XMVectorAdd(tempPosition, XMVectorSet(0.1f, 0.0f, 0.0f, 0.0f));

			// Store the modified position back into camPosition
			Game.camPosition = tempPosition;
		}
		else if (args->VirtualKey == VirtualKey::Q)
		{
			//Dialog.Content = "Right Forward";
			//Dialog.Title = "D Pressed !";
			//Dialog.ShowAsync();

			XMVECTOR tempPosition = Game.camPosition;

			// Increase the X component by a desired amount (e.g., 0.1f)
			tempPosition = XMVectorAdd(tempPosition, XMVectorSet(0.0f, 0.1f, 0.0f, 0.0f));

			// Store the modified position back into camPosition
			Game.camPosition = tempPosition;
		}
		else if (args->VirtualKey == VirtualKey::E)
		{
			//Dialog.Content = "Right Forward";
			//Dialog.Title = "D Pressed !";
			//Dialog.ShowAsync();

			XMVECTOR tempPosition = Game.camPosition;

			// Increase the X component by a desired amount (e.g., 0.1f)
			tempPosition = XMVectorAdd(tempPosition, XMVectorSet(0.0f, -0.1f, 0.0f, 0.0f));

			// Store the modified position back into camPosition
			Game.camPosition = tempPosition;
		}
	}

	void OnPointerWheelChanged(CoreWindow^ sender, PointerEventArgs^ args)
	{
		int Wheel = args->CurrentPoint->Properties->MouseWheelDelta;

		MessageDialog Dialog("", "");
		Dialog.Content = Wheel.ToString();
		Dialog.Title = "MouseWheel Event";
		Dialog.ShowAsync();
	}

	void OnSuspending(Object^ sender, SuspendingEventArgs^ args)
	{

	}
	void OnResuming(Object^ sender, Object^ args)
	{

	}
	void OnClosed(CoreWindow^ sender,CoreWindowEventArgs^ args)
	{
		didCloseWindow = true;
	}
};

ref class EngineSource sealed : public IFrameworkViewSource
{
public:

	virtual IFrameworkView^ CreateView()
	{
		return ref new Engine();
	}

};

[MTAThreadAttribute] // define main () as a multitthreaded aparatment fucntion

int main(Array<String^>^ args)
{
	CoreApplication::Run(ref new EngineSource);
	return 0;
}

