#pragma once
// For use by Vesper applications

#include "Vesper/Core/Base.h"

#include "Vesper/App/Application.h"
#include "Vesper/App/Layer.h"

#include "Vesper/Debug/Instrumentor.h"

#include "Vesper/Core/Log.h"
#include "Vesper/Core/Random.h"
#include "Vesper/Core/Color.h"
#include "Vesper/Core/Timestep.h"
#include "Vesper/Core/Timer.h"		

#include "Vesper/Input/Input.h"							
//#include "Vesper/Input/InputContext.h"				/// TODO: Input Context class
//#include "Vesper/Input/InputAction.h"					/// TODO: Input Action class
#include "Vesper/Input/KeyCodes.h"
#include "Vesper/Input/MouseButtonCodes.h"

/// GUI
#include "Vesper/ImGui/ImGuiLayer.h"					/// TODO: Abstract this to OpenGL/DirectX/Vulkan etc ImGui layers

// -- Particle System (Temporary) -------------------	/// Simple particle system for stress testing renderer
#include "Vesper/ParticleSystem/ParticleSystem.h"		/// Temporary starter particle system

// -- Scene - Entity - Component - System -------------------
#include "Vesper/Scene/Entity.h"
#include "Vesper/Scene/ScriptableEntity.h"
#include "Vesper/Scene/Components.h"
#include "Vesper/Scene/Scene.h"							/// TODO: Give scene a System variable
// #include "Vesper/Scene/Systems.h"					/// TODO: Systems class
// #include "Vesper/Scene/SystemsManager.h"				/// TODO: Static SystemsManager class


// -- Renderer-------------------
#include "Vesper/Renderer/Renderer.h"
#include "Vesper/Renderer/Renderer2D.h"
#include "Vesper/Renderer/RenderCommand.h"

#include "Vesper/Renderer/Buffer.h"
#include "Vesper/Renderer/Framebuffer.h"
#include "Vesper/Renderer/Shader.h"
#include "Vesper/Renderer/Texture.h"
#include "Vesper/Renderer/SubTexture2D.h"
#include "Vesper/Renderer/VertexArray.h"

#include "Vesper/Renderer/OrthographicCamera.h"
#include "Vesper/Renderer/OrthographicCameraController.h"

