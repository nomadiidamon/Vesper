#pragma once
// For use by Vesper applications

#include "Vesper/Core/Base.h"

#include "Vesper/Core/Application.h"
#include "Vesper/Core/Layer.h"
#include "Vesper/Core/Log.h"
#include "Vesper/Core/Random.h"
#include "Vesper/Core/Color.h"
#include "Vesper/Core/Timestep.h"

#include "Vesper/Core/Input.h"
#include "Vesper/Core/KeyCodes.h"
#include "Vesper/Core/MouseButtonCodes.h"

#include "Vesper/ImGui/ImGuiLayer.h"

#include "Vesper/Debug/Instrumentor.h"
//#include "Vesper/Time/Timer.h"		/// TODO: Finish timer class


#include "Vesper/ParticleSystem/ParticleSystem.h" /// Temporary starter particle system

#include "Vesper/Scene/Scene.h"
#include "Vesper/Scene/Entity.h"
#include "Vesper/Scene/Components.h"


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

