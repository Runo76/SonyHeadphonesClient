#pragma once
#include <imgui.h>
#include <cstdint>
#include <iterator>

namespace MaterialYouTheme
{

    using Argb = uint32_t;

    inline ImVec4 ArgbToImVec4(Argb argb)
    {
        return ImVec4(static_cast<float>((argb >> 16) & 0xFF) / 255.0f, static_cast<float>((argb >> 8) & 0xFF) / 255.0f,
                      static_cast<float>(argb & 0xFF) / 255.0f, static_cast<float>((argb >> 24) & 0xFF) / 255.0f);
    }

    inline ImVec4 ArgbToImVec4(Argb argb, float alpha)
    {
        return ImVec4(static_cast<float>((argb >> 16) & 0xFF) / 255.0f, static_cast<float>((argb >> 8) & 0xFF) / 255.0f,
                      static_cast<float>(argb & 0xFF) / 255.0f, alpha);
    }

    inline ImU32 ArgbToImU32(Argb argb)
    {
        return ImGui::ColorConvertFloat4ToU32(ArgbToImVec4(argb));
    }

    inline ImU32 ArgbToImU32(Argb argb, float alpha)
    {
        return ImGui::ColorConvertFloat4ToU32(ArgbToImVec4(argb, alpha));
    }

    // Deep, near-black surface ramp. Each step is clearly lighter than the one
    // below it so cards, frames and headers read as elevated surfaces.
    struct FixedSurfaceColors
    {
        static constexpr Argb surface = 0xFF0B0D10; // window canvas
        static constexpr Argb surfaceContainerLow = 0xFF14171C; // cards / popups
        static constexpr Argb surfaceContainerHigh = 0xFF1C2127; // frames / menu bar
        static constexpr Argb surfaceContainerHighest = 0xFF262C34; // hovered / table headers
        static constexpr Argb onSurface = 0xFFE7ECF2;
        static constexpr Argb onSurfaceVariant = 0xFF9AA5B1;
        static constexpr Argb outline = 0xFF55606C;
        static constexpr Argb outlineVariant = 0xFF2A313A;
        static constexpr Argb inverseSurface = 0xFFE7ECF2;
        static constexpr Argb inverseOnSurface = 0xFF14171C;
        static constexpr Argb error = 0xFFFFB4AB;
        static constexpr Argb errorContainer = 0xFF93000A;
    };

    // One vivid accent used prominently across the whole UI. The per-model
    // Material You palette is intentionally overridden: a single premium accent
    // reads cleaner than matching the headphone's finish.
    struct AccentColors
    {
        static constexpr Argb primary = 0xFF22D3EE; // refined cyan/teal
        static constexpr Argb hovered = 0xFF5EE2F4;
        static constexpr Argb active = 0xFF16B8D2;
        static constexpr Argb onPrimary = 0xFF052229; // dark text/icon on the accent
    };

    struct Theme
    {
        Argb primary;
        Argb onPrimary;
        Argb primaryContainer;
        Argb onPrimaryContainer;
    };

// Precomputed Material You dark theme palettes per ModelColor.
// Regenerate: cd tooling/theme-generator && npm install && npm run generate
#include "MaterialYouThemeTable.inc"

    inline const Theme& ThemeForModelColor(uint8_t modelColor)
    {
        if (modelColor < std::size(kThemeTable))
            return kThemeTable[modelColor];
        return kThemeTable[0]; // DEFAULT
    }

    inline void Apply(const Theme& theme)
    {
        (void)theme; // Premium accent overrides the per-model palette below.
        auto& style = ImGui::GetStyle();
        ImVec4* c = style.Colors;

        // Deep near-black canvas with clearly separated, elevated surface steps.
        c[ImGuiCol_WindowBg] = ArgbToImVec4(FixedSurfaceColors::surface);
        c[ImGuiCol_ChildBg] = ArgbToImVec4(FixedSurfaceColors::surface, 0.0f);
        c[ImGuiCol_PopupBg] = ArgbToImVec4(FixedSurfaceColors::surfaceContainerLow);
        c[ImGuiCol_MenuBarBg] = ArgbToImVec4(FixedSurfaceColors::surfaceContainerHigh);
        c[ImGuiCol_ScrollbarBg] = ArgbToImVec4(FixedSurfaceColors::surface, 0.5f);
        c[ImGuiCol_TableRowBg] = ArgbToImVec4(FixedSurfaceColors::surface, 0.0f);
        c[ImGuiCol_TableRowBgAlt] = ArgbToImVec4(FixedSurfaceColors::surfaceContainerLow, 0.5f);

        // Title bar
        c[ImGuiCol_TitleBg] = ArgbToImVec4(FixedSurfaceColors::surfaceContainerLow);
        c[ImGuiCol_TitleBgActive] = ArgbToImVec4(FixedSurfaceColors::surfaceContainerHighest);
        c[ImGuiCol_TitleBgCollapsed] = ArgbToImVec4(FixedSurfaceColors::surface);

        // Text
        c[ImGuiCol_Text] = ArgbToImVec4(FixedSurfaceColors::onSurface);
        c[ImGuiCol_TextDisabled] = ArgbToImVec4(FixedSurfaceColors::onSurfaceVariant);

        // Hairline borders so elevated surfaces read against the deep background.
        c[ImGuiCol_Border] = ArgbToImVec4(FixedSurfaceColors::outlineVariant);
        c[ImGuiCol_BorderShadow] = ImVec4(0, 0, 0, 0);
        c[ImGuiCol_Separator] = ArgbToImVec4(FixedSurfaceColors::outline, 0.10f);
        c[ImGuiCol_SeparatorHovered] = ArgbToImVec4(FixedSurfaceColors::outline, 0.28f);
        c[ImGuiCol_SeparatorActive] = ArgbToImVec4(AccentColors::primary, 0.80f);
        c[ImGuiCol_TableHeaderBg] = ArgbToImVec4(FixedSurfaceColors::surfaceContainerHighest);
        c[ImGuiCol_TableBorderStrong] = ArgbToImVec4(FixedSurfaceColors::outline);
        c[ImGuiCol_TableBorderLight] = ArgbToImVec4(FixedSurfaceColors::outlineVariant);

        // Input frames: elevated surfaces, accent ring only while active.
        c[ImGuiCol_FrameBg] = ArgbToImVec4(FixedSurfaceColors::surfaceContainerHigh);
        c[ImGuiCol_FrameBgHovered] = ArgbToImVec4(FixedSurfaceColors::surfaceContainerHighest);
        c[ImGuiCol_FrameBgActive] = ArgbToImVec4(AccentColors::primary, 0.22f);

        // Scrollbar
        c[ImGuiCol_ScrollbarGrab] = ArgbToImVec4(FixedSurfaceColors::outline);
        c[ImGuiCol_ScrollbarGrabHovered] = ArgbToImVec4(FixedSurfaceColors::onSurfaceVariant);
        c[ImGuiCol_ScrollbarGrabActive] = ArgbToImVec4(AccentColors::primary);

        // The single accent: selection, sliders, focus and links.
        c[ImGuiCol_CheckMark] = ArgbToImVec4(AccentColors::primary);
        c[ImGuiCol_SliderGrab] = ArgbToImVec4(AccentColors::primary);
        c[ImGuiCol_SliderGrabActive] = ArgbToImVec4(AccentColors::hovered);

        // Buttons: accent-filled primaries with clear hover/press states. Button
        // text is pushed to onPrimary at the call sites for contrast.
        c[ImGuiCol_Button] = ArgbToImVec4(AccentColors::primary);
        c[ImGuiCol_ButtonHovered] = ArgbToImVec4(AccentColors::hovered);
        c[ImGuiCol_ButtonActive] = ArgbToImVec4(AccentColors::active);

        // Headers (combo/list selection): accent tint on hover/active.
        c[ImGuiCol_Header] = ArgbToImVec4(FixedSurfaceColors::surfaceContainerLow);
        c[ImGuiCol_HeaderHovered] = ArgbToImVec4(AccentColors::primary, 0.16f);
        c[ImGuiCol_HeaderActive] = ArgbToImVec4(AccentColors::primary, 0.28f);

        // Tabs: the active tab carries the accent (overline + tint).
        c[ImGuiCol_Tab] = ArgbToImVec4(FixedSurfaceColors::surfaceContainerLow);
        c[ImGuiCol_TabSelected] = ArgbToImVec4(AccentColors::primary, 0.20f);
        c[ImGuiCol_TabSelectedOverline] = ArgbToImVec4(AccentColors::primary);
        c[ImGuiCol_TabHovered] = ArgbToImVec4(AccentColors::primary, 0.12f);
        c[ImGuiCol_TabDimmed] = ArgbToImVec4(FixedSurfaceColors::surface);
        c[ImGuiCol_TabDimmedSelected] = ArgbToImVec4(AccentColors::primary, 0.12f);
        c[ImGuiCol_TabDimmedSelectedOverline] = ArgbToImVec4(FixedSurfaceColors::outline);

        // Misc
        c[ImGuiCol_TextLink] = ArgbToImVec4(AccentColors::primary);
        c[ImGuiCol_TextSelectedBg] = ArgbToImVec4(AccentColors::primary, 0.40f);
        c[ImGuiCol_NavCursor] = ArgbToImVec4(AccentColors::primary);
        c[ImGuiCol_DragDropTarget] = ArgbToImVec4(AccentColors::primary);

        // Resize grip
        c[ImGuiCol_ResizeGrip] = ArgbToImVec4(FixedSurfaceColors::outline, 0.5f);
        c[ImGuiCol_ResizeGripHovered] = ArgbToImVec4(FixedSurfaceColors::onSurfaceVariant);
        c[ImGuiCol_ResizeGripActive] = ArgbToImVec4(AccentColors::primary);

        // Plot
        c[ImGuiCol_PlotLines] = ArgbToImVec4(AccentColors::primary);
        c[ImGuiCol_PlotLinesHovered] = ArgbToImVec4(AccentColors::hovered);
        c[ImGuiCol_PlotHistogram] = ArgbToImVec4(AccentColors::primary);
        c[ImGuiCol_PlotHistogramHovered] = ArgbToImVec4(AccentColors::hovered);

        // Modal dim
        c[ImGuiCol_ModalWindowDimBg] = ImVec4(0, 0, 0, 0.6f);
        c[ImGuiCol_NavWindowingHighlight] = ArgbToImVec4(AccentColors::primary, 0.7f);
        c[ImGuiCol_NavWindowingDimBg] = ImVec4(0, 0, 0, 0.2f);

        // Input cursor
        c[ImGuiCol_InputTextCursor] = ArgbToImVec4(AccentColors::primary);
    }

    inline void ApplyDefault()
    {
        Apply(kThemeTable[0]);
    }

    inline void ApplyForModelColor(uint8_t modelColor)
    {
        Apply(ThemeForModelColor(modelColor));
    }

} // namespace MaterialYouTheme
