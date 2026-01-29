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
- Primary IDE: Visual Studio 2022
- Platforms: Windows (Visual Studio solution & VCXPROJ files provided)
  - Planned: Linux, macOS
- Build system: Premake5
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

## Repository layout (high level)

- `Vesper/` � Engine source (core, renderer, scene, editor integration)
  - `src/Vesper` � engine code
  - `vendor/` � third-party libs (GLFW, Glad, ImGui)
- `Vesper-Editor/` � Editor build that hosts engine in an editor UI
- `Sandbox/` � Example application(s) that use the engine
- `README.md`, `LICENSE-Hazel_Apache2.txt` � repo metadata and licensing

Notable engine files:
- `Vesper/src/Vesper.h` � single include for engine public API
- `Vesper/src/Vesper/Scene/*` � Scene, Entity, Components, Camera
- `Vesper-Editor/src/EditorLayer.*` � Editor integration layer and UI panels

## Getting started (clone + third-party libs)

```bash
git clone --recursive https://github.com/nomadiidamon/Vesper <desiredLocation>
```

## Build (Visual Studio 2022)

1. Run the 'Win-GenProjects.bat' script from the repository root 'Vesper/scripts/Win-GenProjects.bat' to generate the Visual Studio solution file (`.sln`).
	- This runs Premake5 which is located in 'Vesper/Vendor/bin/premake'.
2. Open the solution in Visual Studio 2022.
3. Select the desired configuration (e.g. `Debug` or `Release`) and platform (`x64`).
4. Right-click the project you want to run (`Vesper-Editor` or `Sandbox`) and choose __Set as Startup Project__.
5. Build and run (F5).

Troubleshooting:
- Ensure the Windows SDK and Visual C++ toolset for VS2022 are installed.
- If include or linker errors appear, confirm vendor project builds (GLFW, Glad, ImGui, etc.) and that project dependencies are set correctly.
- For unresolved externals, confirm platform (x86/x64) consistency across projects.

## Run the editor or sandbox

- Start the editor by launching the `Vesper-Editor` startup project.
- Use the `Sandbox` project to iterate small demos that use the engine API.
  - Modify or add new demo source files in the `Sandbox/src` folder.
- Create new projects that link against the Vesper engine.

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

## Continuous Integration

This repository includes a GitHub Actions CI workflow that automatically builds the project on Windows using Visual Studio. The workflow:

- Triggers on pushes to `master`, `develop`, and `copilot/**` branches
- Triggers on pull requests to `master` and `develop` branches
- Can be manually triggered via GitHub Actions UI
- Builds in both Debug and Release configurations
- Uploads Release build artifacts for download

The workflow uses Premake5 to generate Visual Studio solutions and MSBuild to compile the project.

## Contributing

Please open issues or pull requests. 

## Notes / TODO
- Pin third-party dependency versions and document them.
- Add a `CONTRIBUTING.md` with coding standards, branch workflow, and required checks (formatting, tests).

## Licensing
> **License status:** Proprietary (subject to change)

Vesper itself is not **currently** released under any specific license and is provided as-is.
Copyright � 2025 by Damon S. Green II (nomad_ii_damon).
All rights reserved unless otherwise stated.

This project contains code derived from the Hazel Engine
by TheCherno, licensed under the Apache License 2.0,
obtained through public tutorialization and source code access.

Original Hazel code and derivative works thereof are licensed
under the Apache License 2.0. Other original code in this repository
is Copyright � 2025 Damon S. Green II (nomad_ii_damon).

See the LICENSE file for details.


#### Fully-Modified File Header for Licensing
Copyright � TheCherno
Modifications Copyright � 2025 Damon S. Green II (nomad_ii_damon)

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    [Apache](http://www.apache.org/licenses/LICENSE-2.0)


#### Modified File Header for Licensing
Based on Hazel Engine (Apache 2.0)
Modified by Damon S. Green II, 2025