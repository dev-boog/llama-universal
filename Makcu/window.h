#pragma once

#include <windows.h>
#include "ImGui/ImGui.h"
#include "ImGui/imgui_impl_dx9.h"
#include "ImGui/imgui_impl_win32.h"

namespace window
{
	void render();

	namespace LoadedFonts
	{
		extern ImFont* BoldTitleFont;
		extern ImFont* WelcomeFont;
	}
}