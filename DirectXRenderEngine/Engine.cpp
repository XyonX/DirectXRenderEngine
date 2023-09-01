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
	virtual void SetWindow(CoreWindow^Window){}
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
