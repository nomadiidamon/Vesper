#include "ViewportLayer.h"

#include "imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>

namespace Vesper {

    ViewportLayer::ViewportLayer(const std::string& name)
        : Layer(name)
        , m_WindowName(name)
    {
    }

    ViewportLayer::~ViewportLayer() = default;

    void ViewportLayer::OnAttach()
    {
        // no-op (framebuffer may be provided by caller)
    }

    void ViewportLayer::OnDetach()
    {
    }

    void ViewportLayer::SetFramebuffer(const Ref<Framebuffer>& framebuffer)
    {
        m_Framebuffer = framebuffer;
        if (m_Framebuffer)
        {
            auto spec = m_Framebuffer->GetSpecification();
            m_ViewportSize = { (float)spec.Width, (float)spec.Height };
        }
    }

    Ref<Framebuffer> ViewportLayer::GetFramebuffer() const
    {
        return m_Framebuffer;
    }

    void ViewportLayer::SetOnResizeCallback(std::function<void(uint32_t, uint32_t)> callback)
    {
        m_ResizeCallback = std::move(callback);
    }

    void ViewportLayer::SetInsideRenderCallback(std::function<void()> callback)
    {
        m_InsideRenderCallback = std::move(callback);
    }

    glm::vec2 ViewportLayer::GetViewportSize() const
    {
        return m_ViewportSize;
    }

    const glm::vec2* ViewportLayer::GetViewportBounds() const
    {
        return m_ViewportBounds;
    }

    bool ViewportLayer::IsFocused() const
    {
        return m_Focused;
    }

    bool ViewportLayer::IsHovered() const
    {
        return m_Hovered;
    }

    void ViewportLayer::OnImGuiRender()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
        ImGui::Begin(m_WindowName.c_str());

        m_Focused = ImGui::IsWindowFocused();
        m_Hovered = ImGui::IsWindowHovered();

        // Available content region determines the viewport size we display.
        ImVec2 contentAvail = ImGui::GetContentRegionAvail();
        glm::vec2 newSize = { contentAvail.x, contentAvail.y };

        if (m_ViewportSize != newSize && newSize.x > 0.0f && newSize.y > 0.0f)
        {
            m_ViewportSize = newSize;
            if (m_Framebuffer)
            {
                m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            }
            if (m_ResizeCallback)
                m_ResizeCallback((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        }

        ImVec2 viewportPos = ImGui::GetCursorScreenPos();
        m_ViewportBounds[0] = { viewportPos.x, viewportPos.y };
        m_ViewportBounds[1] = { viewportPos.x + m_ViewportSize.x, viewportPos.y + m_ViewportSize.y };

        uint32_t textureID = 0;
        if (m_Framebuffer)
            textureID = m_Framebuffer->GetColorAttachmentRendererID();

        // Draw the framebuffer (or a 0 texture if none set)
        ImGui::Image((ImTextureID)(uintptr_t)textureID, ImVec2(m_ViewportSize.x, m_ViewportSize.y), ImVec2(0, 1), ImVec2(1, 0));

        // Allow caller to render inside the same ImGui window (gizmos, overlays, debug UI)
        if (m_InsideRenderCallback)
            m_InsideRenderCallback();

        ImGui::End();
        ImGui::PopStyleVar();
    }

} // namespace Vesper