#include <Vesper.h>

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Platform/OpenGL/OpenGLShader.h"

static void DisplayVesperInfo() 
{
	ImGui::Begin("Vesper Engine");

	if (ImGui::TreeNode("About Vesper"))
	{
		ImGui::Text("Vesper Engine");
		ImGui::Text("Version: 0.1.0");
		ImGui::Text("Author: Damon Green II");
		ImGui::Text("GitHub: https://github.com/nomadiidamon/Vesper");
		ImGui::Separator();

		ImGui::Text("Status: ");
		ImGui::Text("\tEarly Development of API and 2D Renderer");
		ImGui::Separator();

		ImGui::TextWrapped("Vesper is a cross-platform game engine currently in early development. The engine is being built from the ground up with a focus on modularity, performance, and ease of use. The goal of Vesper is to provide developers with a powerful and flexible toolset for creating games and interactive applications.");
		ImGui::Separator();

		if (ImGui::TreeNode("Controls:"))
		{
			ImGui::Text("\tWASD: Move Camera");
			ImGui::Text("\tQ/E: Rotate Camera (if enabled {see settings}");
			ImGui::Text("\tScroll Wheel: Zoom Camera");
			ImGui::TreePop();
		}
		ImGui::Separator();

		if (ImGui::TreeNode("RoadMap")) {

			if (ImGui::TreeNode("Current Features:"))
			{
				ImGui::Text("\t- Cross-Platform Design");
				ImGui::Text("\t\t- Currently Windows only");
				ImGui::Text("\t- OpenGL Renderer");
				ImGui::Text("\t- Orthographic Camera");
				ImGui::Text("\t- Shader System");
				ImGui::Text("\t- Texture Loading");
				ImGui::Text("\t- ImGui Integration");
				ImGui::Text("\t\t- Current settings panel adjusts camera parameters!");

				ImGui::TreePop();
			}
			ImGui::Separator();

			if (ImGui::TreeNode("In Progress:"))
			{
				ImGui::Text("\t- 2D Rendering Features");
				ImGui::Text("\t\t- Sprites");
				ImGui::Text("\t\t- Sprite Sheets");
				ImGui::Text("\t\t- Animation");
				ImGui::TreePop();
			}
			ImGui::Separator();

			if (ImGui::TreeNode("Planned Features:"))
			{
				ImGui::Text("\t- Vulkan Renderer");
				ImGui::Text("\t- 2D Editor");
				ImGui::Text("\t- 2D Particles");
				ImGui::Text("\t- Audio");
				ImGui::Text("\t- Timelining");
				ImGui::Text("\t- Video Playback");
				ImGui::Text("\t- 3D Renderer");
				ImGui::Text("\t- 3D Particles");
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
	ImGui::End();
}

class ExampleLayer : public Vesper::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_CameraController(1280.0f / 720.0f)
	{

		m_VertexArray.reset(Vesper::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f,		0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f,		0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f,		0.8f, 0.8f, 0.2f, 1.0f,
		};

		Vesper::Ref<Vesper::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Vesper::VertexBuffer::Create(vertices, sizeof(vertices)));
		Vesper::BufferLayout layout = {
			{ Vesper::ShaderDataType::Float3, "a_Position" },
			{ Vesper::ShaderDataType::Float4, "a_Color"  }

		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		Vesper::Ref<Vesper::IndexBuffer> indexBuffer;
		indexBuffer.reset(Vesper::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);




		m_SquareVA.reset(Vesper::VertexArray::Create());
		float squareVertices[5 * 4] =
		{
			-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f,		1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f,		1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f,		0.0f, 1.0f
		};

		Vesper::Ref<Vesper::VertexBuffer> squareVB;
		squareVB.reset(Vesper::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		squareVB->SetLayout({
			{ Vesper::ShaderDataType::Float3, "a_Position"  },
			{ Vesper::ShaderDataType::Float2, "a_TexCoord"  }
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Vesper::Ref<Vesper::IndexBuffer> squareIB;
		squareIB.reset(Vesper::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

		m_Shader = Vesper::Shader::Create("VertexPosColor", vertexSrc, fragmentSrc);

		std::string blueShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string flatColorShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			uniform vec3 u_Color;

			void main()
			{
				color = vec4(u_Color, 1.0);
			}
		)";

		m_FlatColorShader = (Vesper::Shader::Create("FlatColor", blueShaderVertexSrc, flatColorShaderFragmentSrc));

		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");


		m_Texture = Vesper::Texture2D::Create("assets/textures/Checkerboard.png");
		m_Texture2 = Vesper::Texture2D::Create("assets/textures/sheets/fire_01.png");


		std::dynamic_pointer_cast<Vesper::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Vesper::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Vesper::Timestep ts) override
	{
		// Update
		m_CameraController.OnUpdate(ts);


		// Render
		Vesper::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Vesper::RenderCommand::Clear();

		Vesper::Renderer::BeginScene(m_CameraController.GetCamera());

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<Vesper::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<Vesper::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Vesper::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
			}
		}

		auto textureShader = m_ShaderLibrary.Get("Texture");

		m_Texture->Bind();
		Vesper::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		m_Texture2->Bind();
		Vesper::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		// Triangle
		//Vesper::Renderer::Submit(m_Shader, m_VertexArray);

		Vesper::Renderer::EndScene();
	}

	void OnImGuiRender() override
	{
		DisplayVesperInfo();

		ImGui::Begin("Settings");

		if (ImGui::TreeNode("Settings"))
		{
			ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
			m_CameraController.OnImGuiRender();
			ImGui::TreePop();
		}
		ImGui::End();
	}

	void OnEvent(Vesper::Event& event) override
	{
		m_CameraController.OnEvent(event);
	}


private:
	Vesper::ShaderLibrary m_ShaderLibrary;
	Vesper::Ref<Vesper::Shader> m_Shader;
	Vesper::Ref<Vesper::VertexArray> m_VertexArray;

	Vesper::Ref<Vesper::Shader> m_FlatColorShader;
	Vesper::Ref<Vesper::VertexArray> m_SquareVA;

	Vesper::Ref<Vesper::Texture2D> m_Texture, m_Texture2;

	Vesper::OrthographicCameraController m_CameraController;
	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};


class SandboxApp : public Vesper::Application
{
public:
	SandboxApp() {

		PushLayer(new ExampleLayer());
	}
	~SandboxApp() {

	}
};

Vesper::Application* Vesper::CreateApplication()
{
	return new SandboxApp();
}