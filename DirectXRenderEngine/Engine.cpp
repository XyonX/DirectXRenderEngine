//precompiler header
#include "pch.h"

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
public:

	virtual void Initialize(CoreApplicationView^ appView)
	{
		// subscribe OnActivated function to handle the activated event
		appView->Activated += ref new TypedEventHandler <CoreApplicationView^, IActivatedEventArgs^>(this, &Engine::OnActivated);
	}
	virtual void SetWindow(CoreWindow^Window)
	{
		Window->PointerPressed += ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &Engine::PointerPressed);
		Window->KeyDown += ref new TypedEventHandler<CoreWindow^,KeyEventArgs^>(this, &Engine::OnKeyDown);
	}
	virtual void Load(String^EntryPoint){}
	virtual void Run()
	{
		//obtain a pointer to the window
		CoreWindow^Window = CoreWindow::GetForCurrentThread();

		//run processEvents to dispatch
		Window->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessUntilQuit);


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
		Dialog.Content = "Hellow World";
		Dialog.Title = "Notice !";
		Dialog.ShowAsync();
	
	}
	void OnKeyDown(CoreWindow^ sender, KeyEventArgs^ args)
	{
		MessageDialog Dialog("", "");

		if (args->VirtualKey == VirtualKey::W)
		{
			Dialog.Content = "Move Forward";
			Dialog.Title = "W Pressed !";
			Dialog.ShowAsync();
		}

		else if (args->VirtualKey == VirtualKey::A)
		{
			Dialog.Content = "Move Backward";
			Dialog.Title = "A Pressed !";
			Dialog.ShowAsync();
		}

		else if (args->VirtualKey == VirtualKey::S)
		{
			Dialog.Content = "Move Left";
			Dialog.Title = "S Pressed !";
			Dialog.ShowAsync();
		}

		else if (args->VirtualKey == VirtualKey::D)
		{
			Dialog.Content = "Right Forward";
			Dialog.Title = "D Pressed !";
			Dialog.ShowAsync();
		}
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
