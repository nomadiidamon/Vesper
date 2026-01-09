#pragma once

/// DLL support
#ifdef VZ_PLATFORM_WINDOWS
#if VZ_DYNAMIC_LINK
#ifdef VZ_BUILD_DLL
#define VESPER_API __declspec(dllexport)
#else
#define VESPER_API __declspec(dllimport)
#endif
#else
#define VESPER_API
#endif
#else
#error Vesper only supports Windows!
#endif // End of DLL support

/// TODO: Implement
/// Default Graphics API




/// Editor Configurations
#ifdef VZ_EDITOR_USE_DEFAULT_SCENE 
#define VZ_EDITOR_DEFAULT_SCENE "../Vesper/src/Vesper/Resources/Scenes/TriColored3DCubeAndSpriteAnims.vesper"
#endif

#define VZ_DEFAULT_TEXTURE Texture2D::Create("../Vesper/src/Vesper/Resources/Textures/Checkerboard.png")

