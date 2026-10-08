# Vesper

Vesper is a lightweight 2D/3D engine and editor inspired by TheCherno's Game Engine Architecture series for the Hazel engine.  
It provides a renderer, scene & entity system, editor UI, input handling, and utilities to build simple games and interactive applications in C++ (C++17).

It is generally usable as an easily modifiable base line for creating graphics and software applications.

> The specific purpose is for the creation of Particle System demos, experiments, and visualizations.
>
> ***It is not intended to be a full-featured game engine,***
>
> But rather an application to create a range of visual effects and particle system simulations.

## Quick overview

- Language: C++17
- Platforms: Windows
  - Planned: Linux, macOS
- Third-party libraries:
  - entt (entity-component system)
  - Glad (OpenGL function loading)
  - GLFW (windowing and input)
  - glm (math library)
  - ImGui (editor UI)
  - spdlog (logging)
  - stb (image loading)
- Core features:
  - Scene / Entity / Component system
  - 2D renderer with orthographic and perspective cameras
  - ImGui-based editor layer
  - Input and event handling
  - Starter particle system and instrumentation


## New Project example code
```cpp
#include <Vesper.h>
#include <Vesper/Core/EntryPoint.h> // can only be included in one source file

#include "imgui/imgui.h"

class ExampleLayer : public Vesper::Layer
{
public:
	ExampleLayer()
		: Layer("ExampleLayer")
	{
	}

	~ExampleLayer() override = default;

	// Called once when the layer is attached to the layer stack
	void OnAttach() override {}

	// Called once when the layer is detached from the layer stack
	void OnDetach() override {}

	// Called every frame with the frame time
	void OnUpdate(Vesper::Timestep ts) override {}

	// Render ImGui UI for this layer (optional)
	void OnImGuiRender() override {
		ImGui::Begin("Example Layer");
		ImGui::Text("Hello from ExampleLayer!");
		ImGui::End();
	}

	// Receive events (keyboard/mouse/window/etc.)
	void OnEvent(Vesper::Event& e) override {}
};

class YourAppNameHere : public Vesper::Application
{
public:
	YourAppNameHere() {

		PushLayer(new ExampleLayer());
	}

	~YourAppNameHere() {	}
};

Vesper::Application* Vesper::CreateApplication() {
	return new YourAppNameHere();
}
```

