#include "ImGui/ImGui.h"
#include "ImGui/imgui_impl_dx9.h"
#include "ImGui/imgui_impl_win32.h"

#include "menu.h"
#include "recoil.h"
#include "window.h"

#include <shlobj.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

extern bool menu::active = true;
bool WindowInit = false;
bool ShowDevWindows = false;
int SelectedTab = 0;

extern bool menu::toggles::enable_recoil = false;
extern int menu::toggles::selected_recoil_mode = 0;
extern float menu::toggles::control_x = 0.0f;
extern float menu::toggles::control_y = 0.0f;
extern float menu::toggles::delay = 10;
extern bool menu::toggles::enable_cont_time = false;
extern float menu::toggles::cont_time = 10;
static char vector_patern_name[64] = "default";
const char* recoil_modes[] = { "X & Y Slider", "Custom Vector" };

static ImVec4 main_color = ImVec4(0.38f, 0.51f, 0.63f, 1.00f);


class initWindow
{
public:
    const char* window_title = "llama";

    DWORD window_flags = ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar;
    ImVec4 MenuColor = ImVec4(0.56f, 0.80f, 0.56f, 0.54f);
} iw;

void TabText(const char* label, int id)
{
    if (SelectedTab == id)
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 1)); // selected
    else
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1)); // unselected

    ImGui::Text("%s", label);
    if (ImGui::IsItemClicked())
        SelectedTab = id;

    ImGui::PopStyleColor();
}


void Init()
{
    ImVec4* colors = ImGui::GetStyle().Colors;
    {
        colors[ImGuiCol_WindowBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
        colors[ImGuiCol_Border] = ImVec4(0.25f, 0.25f, 0.25f, 0.50f);
        colors[ImGuiCol_CheckMark] = ImVec4(0.38f, 0.51f, 0.63f, 1.00f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.38f, 0.51f, 0.63f, 1.00f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.38f, 0.51f, 0.63f, 1.00f);
        colors[ImGuiCol_Button] = ImVec4(0.38f, 0.51f, 0.63f, 1.00f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.38f, 0.51f, 0.63f, 1.00f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.38f, 0.51f, 0.63f, 1.00f);
        colors[ImGuiCol_Header] = ImVec4(0.38f, 0.51f, 0.63f, 1.00f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.38f, 0.51f, 0.63f, 1.00f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.11f, 0.11f, 0.11f, 0.54f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    }
    ImGuiStyle* style = &ImGui::GetStyle();
    {
		style->FrameBorderSize = 1.0f;

        style->ChildRounding = 2.f;
        style->FrameRounding = 2.f;
    }

    WindowInit = true;
}

bool ShowWelcome = true;
void menu::render()
{
   
    if (!WindowInit)
        Init();

    // Menu Loop
    if (menu::active)
    {
        // Colours that user can change
        ImVec4* colors = ImGui::GetStyle().Colors;
        {
            colors[ImGuiCol_CheckMark] = main_color;
            colors[ImGuiCol_SliderGrab] = main_color;
            colors[ImGuiCol_SliderGrabActive] = main_color;
            colors[ImGuiCol_Button] = main_color;
            colors[ImGuiCol_ButtonHovered] = main_color;
            colors[ImGuiCol_ButtonActive] = main_color;
        }

        if (ShowDevWindows)
            ImGui::ShowStyleEditor();

        if (ShowWelcome)
        {
            ImGui::SetNextWindowSize(ImVec2(235, 125));
            ImGui::Begin("Welcome", &menu::active, iw.window_flags);
            {
                ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.16f, 0.16f, 0.16f, 1.00f));
                ImGui::BeginChild("SidebarContent", ImVec2(218, 110), true, 0);
                {
                    ImGui::PushFont(window::LoadedFonts::WelcomeFont);
                    {
                        ImGui::SetCursorPos(ImVec2(55, 25));
                        ImGui::Text("llama-makcu");
                    }
                    ImGui::PopFont();
                    ImGui::SetCursorPos(ImVec2(40, 45));
                    ImGui::TextDisabled("Your Makcu has been found.");

                    if (ImGui::Button("Continue", ImVec2(130, 26))) {
                        ShowWelcome = false;
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Discord", ImVec2(60, 26))) {
                        std::string url = "https://discord.gg/HmaP9MW5NN";
                        std::string command = "start " + url;

                        system(command.c_str());
                    }
                }
                ImGui::EndChild();
                ImGui::PopStyleColor();
            }
            ImGui::End();
        }
        else
        {
            ImGui::Begin(iw.window_title, &menu::active, iw.window_flags);
            {
                ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.16f, 0.16f, 0.16f, 1.00f));
                ImGui::BeginChild("SidebarContent", ImVec2(101, 300), true, 0);
                {
                    ImGui::PushFont(window::LoadedFonts::BoldTitleFont);
                    {
                        ImGui::Spacing();
                        ImGui::Text("llama-makcu");
                    }
                    ImGui::PopFont();
                    ImGui::SetCursorPos(ImVec2(31, 24));
                    ImGui::TextDisabled("universal");

                    TabText("Recoil", 0);
                    TabText("Vector Generator", 1);
                    TabText("Settings", 2);
                    ImGui::TextDisabled("Exit");
                    if (ImGui::IsItemClicked())
                        menu::active = false;
                }
                ImGui::EndChild();
                ImGui::PopStyleColor();

                ImGui::SameLine();

                ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.16f, 0.16f, 0.16f, 1.00f));
                ImGui::BeginChild("RecoilContent", ImVec2(400, 300), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
                {
                    switch (SelectedTab)
                    {
                    case 0: { // Recoil
                        ImGui::TextDisabled(" Llama Makcu"); ImGui::SameLine(); ImGui::TextDisabled(">"); ImGui::SameLine(); ImGui::Text("Recoil");
                        ImGui::Spacing();

                        // Recoil toggles
                        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.12f, 0.12f, 1.00f));
                        ImGui::BeginChild("RecoilToggles", ImVec2(385, 124), true, 0);
                        {
                            ImGui::Checkbox("Enable Recoil Control", &menu::toggles::enable_recoil);
                            ImGui::Combo("Recoil Mode", &menu::toggles::selected_recoil_mode, recoil_modes, IM_ARRAYSIZE(recoil_modes));
                            if (ImGui::IsItemHovered())
                                ImGui::SetTooltip("Generate, save & load custom vectors in the vector generator tab.");
                            ImGui::SliderFloat("Delay", &menu::toggles::delay, 0, 100, "%.2f");
                            ImGui::Checkbox("Enable Control Time", &menu::toggles::enable_cont_time);
                            ImGui::SliderFloat("Control Time (Seconds)", &menu::toggles::cont_time, 1, 100, "%.2f");
                        }
                        ImGui::EndChild();
                        ImGui::PopStyleColor();

                        // X, Y Sliders
                        if (menu::toggles::selected_recoil_mode == 0)
                        {
                            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.12f, 0.12f, 1.00f));
                            ImGui::BeginChild("RecoilSliders", ImVec2(385, 57), true, 0);
                            {
                                ImGui::SliderFloat("X", &menu::toggles::control_x, -10, 10, "%.1f");
                                ImGui::SliderFloat("Y", &menu::toggles::control_y, -10, 10, "%.1f");
                            }
                            ImGui::EndChild();
                            ImGui::PopStyleColor();
                        }
                    }
                          break;
                    case 1: { // Vector Generator
                        ImGui::TextDisabled(" Llama Makcu"); ImGui::SameLine(); ImGui::TextDisabled(">"); ImGui::SameLine(); ImGui::Text("Vector Generator");
                        ImGui::Spacing();

                        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.12f, 0.12f, 1.00f));
                        ImGui::BeginChild("VectorResize", ImVec2(385, 75), true, 0);
                        {
                            ImGui::TextDisabled("Vector Size");

                            static int newSize = recoil::table::x.size();
                            ImGui::PushItemWidth(370);
                            {
                                ImGui::InputInt("##VectorSize", &newSize);
                            }
                            ImGui::PopItemWidth();
                            if (ImGui::Button("Resize", ImVec2(370, 18)))
                            {
                                if (newSize > 0)
                                {
                                    recoil::table::x.resize(newSize);
                                    recoil::table::y.resize(newSize);
                                }
                            }
                        }
                        ImGui::EndChild();
                        ImGui::PopStyleColor();

                        // Edit Values
                        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.12f, 0.12f, 1.00f));
                        ImGui::BeginChild("VectorValues", ImVec2(385, 123), true, 0);
                        {
                            ImGui::TextDisabled("Edit X / Y values:");
                            for (int i = 0; i < recoil::table::x.size(); i++)
                            {
                                ImGui::PushID(i);
                                ImGui::PushItemWidth(135);
                                {
                                    ImGui::InputInt("X", &recoil::table::x[i]);
                                    ImGui::SameLine();
                                    ImGui::InputInt("Y", &recoil::table::y[i]);
                                    ImGui::SameLine();
                                    ImGui::TextDisabled("%d", i + 1);
                                }
                                ImGui::PopItemWidth();
                                ImGui::PopID();
                            }
                        }
                        ImGui::EndChild();
                        ImGui::PopStyleColor();

                        // Save/Load Values
                        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.12f, 0.12f, 1.00f));
                        ImGui::BeginChild("VectorSaveLoad", ImVec2(385, 57), true, 0);
                        {
                            ImGui::SetNextItemWidth(370);
                            ImGui::InputText("##ConfigName", vector_patern_name, IM_ARRAYSIZE(vector_patern_name));

                            if (ImGui::Button("Save", ImVec2(180, 18)))
                                recoil::table::SavePattern(vector_patern_name);
                            ImGui::SameLine();
                            if (ImGui::Button("Load", ImVec2(180, 18)))
                                recoil::table::LoadPattern(vector_patern_name);

                        }
                        ImGui::EndChild();
                        ImGui::PopStyleColor();
                    }
                          break;
                    case 2: {
                        ImGui::TextDisabled(" Llama Makcu"); ImGui::SameLine(); ImGui::TextDisabled(">"); ImGui::SameLine(); ImGui::Text("Settings");
                        ImGui::Spacing();

                        // Menu Settings
                        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.12f, 0.12f, 1.00f));
                        ImGui::BeginChild("MenuSettings", ImVec2(385, 35), true, 0);
                        {
                            ImGui::ColorEdit3("Color (RGB)", (float*)&main_color);
                        }
                        ImGui::EndChild();
                        ImGui::PopStyleColor();

                        break;
                    }
                    }
                }
                ImGui::EndChild();
                ImGui::PopStyleColor();
            }
            ImGui::End();
        }
    }
    else
    {
        exit(0);
    }
}