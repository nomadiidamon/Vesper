#pragma once
#include "Vesper/App/Layer.h"
#include "Vesper/Renderer/Framebuffer.h"
#include "imgui/imgui.h"
#include <glm/glm.hpp>

namespace Vesper {

    class ViewportLayer : public Layer
    {
    public:
        explicit ViewportLayer(const std::string& name = "Viewport");
        ~ViewportLayer() override;

        // Layer lifecycle
        void OnAttach() override;
        void OnDetach() override;
        void OnImGuiRender() override;

        // Provide an external framebuffer (optional). If not set, layer can create one later.
        void SetFramebuffer(const Ref<Framebuffer>& framebuffer);
        Ref<Framebuffer> GetFramebuffer() const;

        // Called when the widget's content region changes size.
        void SetOnResizeCallback(std::function<void(uint32_t, uint32_t)> callback);

        // Called while the viewport window is still open and before ImGui::End().
        // Use this to place ImGuizmo/other ImGui drawing that requires the same window.
        void SetInsideRenderCallback(std::function<void()> callback);

        // Query state after OnImGuiRender()
        glm::vec2 GetViewportSize() const;
        const glm::vec2* GetViewportBounds() const; // [0] = min, [1] = max
        bool IsFocused() const;
        bool IsHovered() const;

    private:
        std::string m_WindowName;

        Ref<Framebuffer> m_Framebuffer;
        glm::vec2 m_ViewportSize{ 0.0f, 0.0f };
        glm::vec2 m_ViewportBounds[2];
        bool m_Focused = false;
        bool m_Hovered = false;

        std::function<void(uint32_t, uint32_t)> m_ResizeCallback;
        std::function<void()> m_InsideRenderCallback;
    };

}