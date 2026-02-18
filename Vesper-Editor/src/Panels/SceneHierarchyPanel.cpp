#include <Vesper/Utils/PlatformUtils.h>
#include "SceneHierarchyPanel.h"
#include "Vesper/Scene/Components.h"

#include <ImGui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/misc/cpp/imgui_stdlib.h>

#include <glm/gtc/type_ptr.hpp>

#include <cstring>


namespace Vesper {

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
		m_SelectionContext = {};
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");
		if (m_Context) {

			auto view = m_Context->m_Registry.view<NameComponent>();

			for (auto entity : view) {
				Entity e{ entity, m_Context.get() };
				DrawEntityNode(e);
			}

			if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
				m_SelectionContext = {};

			// Right-click on blank space
			if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_NoOpenOverItems | ImGuiPopupFlags_MouseButtonRight))
			{
				if (ImGui::MenuItem("Create Empty Entity"))
					m_SelectionContext = m_Context->CreateEntity("Empty Entity");

				ImGui::EndPopup();
			}
		}
		ImGui::End();

		ImGui::Begin("Properties");
		if (m_SelectionContext)
		{
			DrawComponents(m_SelectionContext);
		}

		ImGui::End();
	}

	void SceneHierarchyPanel::SetSelectedEntity(Entity entity)
	{
		m_SelectionContext = entity;
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& name = entity.GetComponent<NameComponent>().Name;
		void* nodeID = (void*)(uint64_t)(uint32_t)entity;

		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx(nodeID, flags, name.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				entityDeleted = true;

			/// @todo extract this into a separate function and add support for more components
			if (ImGui::MenuItem("Duplicate Entity"))
			{
				Entity newEntity = m_Context->CreateEntity(name);
				// Copy components
				if (entity.HasComponent<NameComponent>()) {
					auto& src = entity.GetComponent<NameComponent>();
					auto& newEntName = newEntity.GetComponent<NameComponent>();
					/// @todo Improve name duplication logic
					if (src.Name.capacity() > 0 && isdigit(src.Name.back()))
					{
						// Increment trailing number
						size_t i = src.Name.size() - 1;
						while (i > 0 && isdigit(src.Name[i - 1]))
							--i;
						std::string baseName = src.Name.substr(0, i);
						std::string numberStr = src.Name.substr(i);
						int number = std::stoi(numberStr);
						newEntName.Name = baseName + std::to_string(number + 1);
					}
					else
						newEntName.Name = src.Name + "1";
				}
				if (entity.HasComponent<SpriteRendererComponent>())
				{
					auto& src = entity.GetComponent<SpriteRendererComponent>();
					newEntity.AddComponent<SpriteRendererComponent>(src);
				}
				if (entity.HasComponent<CameraComponent>())
				{
					auto& cc = entity.GetComponent<CameraComponent>();
					newEntity.AddComponent<CameraComponent>(cc);
				}
				if (entity.HasComponent<NativeScriptComponent>())
				{
					auto& nsc = entity.GetComponent<NativeScriptComponent>();
					newEntity.AddComponent<NativeScriptComponent>(nsc);
				}
				if (entity.HasComponent<TextureAnimationComponent>())
				{
					auto& tac = entity.GetComponent<TextureAnimationComponent>();
					newEntity.AddComponent<TextureAnimationComponent>(tac);
				}
				// Add more components as needed
				m_SelectionContext = newEntity;
			}

			ImGui::EndPopup();
		}

		if (opened)
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
			bool opened = ImGui::TreeNodeEx((void*)9817239, flags, name.c_str());
			if (opened)
			{
				/// @todo Draw child entities here in the future
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}

		if (entityDeleted)
		{
			m_Context->DestroyEntity(entity);
			if (m_SelectionContext == entity)
				m_SelectionContext = {};
		}
	}

	static bool DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f, float v_speed = 0.1f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.2f")
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		// Compute available width for the three float controls in the right column
		float availableWidth = ImGui::GetContentRegionAvail().x;
		float itemSpacing = ImGui::GetStyle().ItemSpacing.x;
		float totalButtonWidth = buttonSize.x * 3.0f;
		// Account for SameLine() spacings between button+control pairs (conservative estimate)
		float totalSpacing = itemSpacing * 6.0f;
		float itemWidth = (availableWidth - totalButtonWidth - totalSpacing) / 3.0f;
		if (itemWidth <= 0.0f)
			itemWidth = ImGui::CalcItemWidth();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::PushItemWidth(itemWidth);
		ImGui::DragFloat("##X", &values.x, v_speed, v_min, v_max, format);
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::PushItemWidth(itemWidth);
		ImGui::DragFloat("##Y", &values.y, v_speed, v_min, v_max, format);
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::PushItemWidth(itemWidth);
		ImGui::DragFloat("##Z", &values.z, v_speed, v_min, v_max, format);
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();

		return true;
	}

	static bool DrawVec2Control(const std::string& label, glm::vec2& values, float resetValue = 0.0f, float columnWidth = 100.0f, float v_speed = 0.1f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.2f")
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		// Compute available width for the three float controls in the right column
		float availableWidth = ImGui::GetContentRegionAvail().x;
		float itemSpacing = ImGui::GetStyle().ItemSpacing.x;
		float totalButtonWidth = buttonSize.x * 2.0f;
		// Account for SameLine() spacings between button+control pairs (conservative estimate)
		float totalSpacing = itemSpacing * 4.0f;
		float itemWidth = (availableWidth - totalButtonWidth - totalSpacing) / 2.0f;
		if (itemWidth <= 0.0f)
			itemWidth = ImGui::CalcItemWidth();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::PushItemWidth(itemWidth);
		ImGui::DragFloat("##X", &values.x, v_speed, v_min, v_max, format);
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::PushItemWidth(itemWidth);
		ImGui::DragFloat("##Y", &values.y, v_speed, v_min, v_max, format);
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();

		return true;

	}

	static bool DrawColorControl(const std::string& label, glm::vec4& color)
	{
		ImGui::ColorEdit4(label.c_str(), glm::value_ptr(color));
		return true;
	}

	static bool DrawTextureControl(const std::string& label, Ref<Texture2D>& texture, bool& textureEnabled, float tilingFactor)
	{
		// Separate checkbox for enabling/disabling texture usage
		if (ImGui::Checkbox("Texture Enabled", &textureEnabled)) {

		}

		// Display current texture name (if any) and buttons to Set / Change / Clear the texture
		if (texture && textureEnabled)
		{
			ImGui::TextUnformatted(texture->GetName().c_str());
			ImGui::SameLine();
			if (ImGui::Button("Change Texture"))
			{
				std::string filePath = FileDialogs::OpenFile("Image Files (*.png;*.jpg;*.jpeg;*.bmp;*.tga)\0*.png;*.jpg;*.jpeg;*.bmp;*.tga\0All Files (*.*)\0*.*\0");
				if (!filePath.empty())
				{
					auto tex = Texture2D::Create(filePath);
					if (tex)
					{
						texture = tex;
						textureEnabled = true;
					}
					else
					{
						VZ_WARN("Could not load texture {0}", filePath);
					}
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("Clear Texture"))
			{
				texture = nullptr;
				textureEnabled = false;
			}
		}
		else
		{
			ImGui::SameLine();
			if (ImGui::Button("Set Texture"))
			{
				std::string filePath = FileDialogs::OpenFile("Image Files (*.png;*.jpg;*.jpeg;*.bmp;*.tga)\0*.png;*.jpg;*.jpeg;*.bmp;*.tga\0All Files (*.*)\0*.*\0");
				if (!filePath.empty())
				{
					auto tex = Texture2D::Create(filePath);
					if (tex)
					{
						texture = tex;
						textureEnabled = true;
					}
					else
					{
						VZ_WARN("Could not load texture {0}", filePath);
					}
				}
			}
		}

		ImGui::DragFloat("Tiling Factor", &tilingFactor, 0.01f);

		return true;
	}

	static bool SubTextureEdit(const std::string& label, const Ref<Texture2D>& texture, Ref<SubTexture2D>& subTexture, glm::vec2& tilingFactor, glm::vec2& offset)
	{
		ImGui::Text(label.c_str());


		if (texture) {
			if (!subTexture) {
				subTexture = SubTexture2D::CreateFromCoords(
					texture, offset,
					glm::vec2(static_cast<float>(texture->GetWidth()) * tilingFactor.x,
						static_cast<float>(texture->GetHeight()) * tilingFactor.y));
			}
			if (subTexture->GetTexture() && subTexture->GetTexture() == texture) {

				DrawVec2Control("Offset", offset, 0.0f, 100.0f, 0.01f);
				DrawVec2Control("Scale", tilingFactor, 1.0f, 100.0f, 0.01f);

				subTexture = SubTexture2D::CreateFromCoords(
					texture, offset,
					glm::vec2(static_cast<float>(texture->GetWidth()) * tilingFactor.x,
						static_cast<float>(texture->GetHeight()) * tilingFactor.y));

			}
			else {
				if (subTexture->GetTexture()) {
					ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "SubTexture does not match the assigned texture!");
				}
				else {
					ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "SubTexture is not valid!");
				}
			}
		}
		else {
			ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "No texture assigned!");
		}

		return true;
	}

	static bool DrawSubTextureControl(const std::string& label, Ref<Texture2D>& texture, Ref<SubTexture2D>& subTexture, glm::vec2& tilingFactor, glm::vec2& offset)
	{
		if (texture) {
			ImGui::TextUnformatted(texture->GetName().c_str());
			ImGui::SameLine();
			SubTextureEdit(label, texture, subTexture, tilingFactor, offset);

		}
		else {
			ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "No texture assigned!");
		}

		return true;
	}

	static bool TextureAnimationEdit(const std::string& label, const Ref<Texture2D>& texture, Ref<SubTexture2D>& subTexture, TextureAnimationComponent& tac)
	{
		ImGui::Text(label.c_str());
		static bool changed = false;

		ImGui::Text("Current Frame: {%d}", tac.CurrentFrame);
		if (ImGui::DragInt("Frame Count", &tac.FrameCount, 1, 1, 1000)) {
			changed = true;
		}
		if (ImGui::DragFloat("Frame Time", &tac.FrameTime, 0.01f, 0.01f, 10.0f)) {
			changed = true;
		}
		if (DrawVec2Control("Coords", tac.Coords)) {
			changed = true;
		}
		if (DrawVec2Control("Cell Size", tac.CellSize)) {
			changed = true;
		}
		if (DrawVec2Control("Sprite Size", tac.SpriteSize)) {
			changed = true;
		}
		if (changed) {
			if (ImGui::Button("Reset Animation")) {
				tac.Reset(tac.Texture, tac.FrameCount, tac.Coords, tac.CellSize, tac.SpriteSize);
			}
			if (ImGui::Button("Add Repeat Frames")) {
				tac.AddFrames(tac.GetSubTextures());
				changed = true;
			}
			if (ImGui::Button("Add Reversed Frames")) {
				tac.AddReveresedFrames(tac.GetSubTextures());
				changed = true;
			}
			if (ImGui::Button("Sync Textures")) {
				tac.SynchronizeTextures(texture, subTexture, tac.GetSubTextures());
				changed = true;
			}
		}

		return changed;
	}

	static bool DrawSpriteRendererComponent(SpriteRendererComponent& src)
	{
		DrawColorControl("Color", src.Color);
		ImGui::Separator();
		DrawTextureControl("Texture", src.Texture, src.TextureEnabled, src.TilingFactor);
	}

	static void DrawParticeSystemComponent(ParticleSystemComponent& particleSystem)
	{
		ParticleSystem& ps = particleSystem.ParticleSystem;
		ParticleProps& psp = ps.m_Props;
		ImGui::DragFloat("Lifetime", &psp.Lifetime, 0.1f, 0.0f, 10000.0f);
		ImGui::DragFloat("Lifetime Variation", &psp.LifetimeVariation, 0.1f, 0.0f, 10000.0f);
		ImGui::Separator();

		DrawVec2Control("Start Size", psp.SizeBegin, 0.75f);
		DrawVec2Control("Size Variation", psp.SizeVariation, 0.5f);
		DrawVec2Control("End Size", psp.SizeEnd, 0.0f);
		ImGui::Separator();

		DrawColorControl("Start Color", psp.ColorBegin);
		DrawColorControl("End Color", psp.ColorEnd);
		ImGui::Separator();

		DrawVec3Control("Velocity", psp.Velocity, -0.1f);
		DrawVec3Control("Velocity Variation", psp.VelocityVariation, 0.5f);
		DrawVec3Control("Position Variation", psp.PositionVariation, 0.5f);
		ImGui::Separator();

		ImGui::DragFloat("Rotation Speed", &psp.Rotation, 0.1f, -360.0f, 360.0f);
		ImGui::DragFloat("Rotation Speed Variation", &psp.RotationVariation, 0.1f, 0.0f, 360.0f);
		ImGui::Separator();

		static bool textureAssigned = false;
		DrawTextureControl("Particle Texture", psp.Texture, textureAssigned, psp.TilingFactor);
		DrawSubTextureControl("Particle SubTexture", psp.Texture, psp.SubTexture, psp.TextureScale, psp.TextureOffset);
		if (psp.Texture) {
			for (int i = 0; i < ps.ActiveParticleCount(); i++) {
				Particle& particle = ps.m_ParticlePool[i];
				particle.Texture = psp.Texture;
			}
		}
		ImGui::Separator();

		if (ImGui::Button("Reset System")) {
			ps.ResetSystem();
		}
		ImGui::SameLine();
		ImGui::Text("Active Particles: ");
		ImGui::SameLine();
		ImGui::Text(std::to_string(ps.ActiveParticleCount()).c_str());
		int lastVal = ps.m_EmitRate;
		ImGui::DragInt("Particle Emission Count", &ps.m_EmitRate, 1, 1, 1500);
		if (lastVal != ps.m_EmitRate) {
			ps.ResizePool(ps.m_EmitRate);
		}
		std::string count = std::to_string(ps.m_EmitRate);
		std::string button = "Emit " + count + " particles";
		if (ImGui::Button(button.c_str())) {
			ps.Emit(ps.m_Props, ps.m_EmitRate);
		}
		ImGui::SameLine();
		if (ImGui::Checkbox("Loop", &ps.m_Loop)) {

		}
	}



	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
		if (entity.HasComponent<T>())
		{
			auto& component = entity.GetComponent<T>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
			ImGui::PopStyleVar(
			);
			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
			if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
			{
				ImGui::OpenPopup("ComponentSettings");
			}

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove component"))
					removeComponent = true;

				ImGui::EndPopup();
			}

			if (open)
			{
				uiFunction(component);
				ImGui::TreePop();
			}

			if (removeComponent)
				entity.RemoveComponent<T>();
		}
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<NameComponent>())
		{
			auto& name = entity.GetComponent<NameComponent>().Name;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strncpy_s(buffer, sizeof(buffer), name.c_str(), sizeof(buffer));
			if (ImGui::InputText("##Name", buffer, sizeof(buffer)))
			{
				name = std::string(buffer);
			}
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");

		if (ImGui::BeginPopup("AddComponent"))
		{
			DisplayAddComponentEntry<CameraComponent>("Camera");
			//DisplayAddComponentEntry<ScriptComponent>("Script");
			DisplayAddComponentEntry<SpriteRendererComponent>("Sprite Renderer");
			DisplayAddComponentEntry<SubTextureComponent>("SubTexture");
			DisplayAddComponentEntry<TextureAnimationComponent>("Texture Animation");
			DisplayAddComponentEntry<ParticleSystemComponent>("Particle System");

			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();

		DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
			{
				DrawVec3Control("Translation", component.Translation);
				glm::vec3 rotation = glm::degrees(component.Rotation);
				DrawVec3Control("Rotation", rotation);
				component.Rotation = glm::radians(rotation);
				DrawVec3Control("Scale", component.Scale, 1.0f);
			});

		DrawComponent<CameraComponent>("Camera", entity, [](auto& component)
			{
				auto& camera = component.Camera;

				ImGui::Checkbox("Primary", &component.Primary);

				const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
				const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];
				if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
						if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
						{
							currentProjectionTypeString = projectionTypeStrings[i];
							camera.SetProjectionType((SceneCamera::ProjectionType)i);
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
				{
					float perspectiveVerticalFov = glm::degrees(camera.GetPerspectiveVerticalFOV());
					if (ImGui::DragFloat("Vertical FOV", &perspectiveVerticalFov))
						camera.SetPerspectiveVerticalFOV(glm::radians(perspectiveVerticalFov));

					float perspectiveNear = camera.GetPerspectiveNearClip();
					if (ImGui::DragFloat("Near", &perspectiveNear))
						camera.SetPerspectiveNearClip(perspectiveNear);

					float perspectiveFar = camera.GetPerspectiveFarClip();
					if (ImGui::DragFloat("Far", &perspectiveFar))
						camera.SetPerspectiveFarClip(perspectiveFar);
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
				{
					float orthoSize = camera.GetOrthographicSize();
					if (ImGui::DragFloat("Size", &orthoSize))
						camera.SetOrthographicSize(orthoSize);

					float orthoNear = camera.GetOrthographicNearClip();
					if (ImGui::DragFloat("Near", &orthoNear))
						camera.SetOrthographicNearClip(orthoNear);

					float orthoFar = camera.GetOrthographicFarClip();
					if (ImGui::DragFloat("Far", &orthoFar))
						camera.SetOrthographicFarClip(orthoFar);

					ImGui::Checkbox("Fixed Aspect Ratio", &component.FixedAspectRatio);
				}
			});

		DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](auto& component)
			{
				DrawSpriteRendererComponent(component);
			});

		DrawComponent<SubTextureComponent>("SubTexture", entity, [](auto& component)
			{
				SubTextureEdit(component.SubTexture->GetTexture()->GetName(), component.SubTexture->GetTexture(), component.SubTexture, component.TilingFactor, component.Offset);
			});

		DrawComponent<TextureAnimationComponent>("Texture Animation", entity, [](auto& component)
			{
				TextureAnimationEdit("Texture Animation", component.Texture, component.SubTextures[0], component);
			});

		DrawComponent<ParticleSystemComponent>("Particle System", entity, [](auto& component)
			{
				DrawParticeSystemComponent(component);
			});



	}


	template<typename T>
	void SceneHierarchyPanel::DisplayAddComponentEntry(const std::string& entryName) {
		if (!m_SelectionContext.HasComponent<T>())
		{
			if (ImGui::MenuItem(entryName.c_str()))
			{
				m_SelectionContext.AddComponent<T>();
				ImGui::CloseCurrentPopup();
			}
		}
	}

}