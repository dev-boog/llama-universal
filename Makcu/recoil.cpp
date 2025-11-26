#include <algorithm>
#include <iostream>
#define NOMINMAX
#include <thread>
#include <cmath>
#include <vector>
#include <chrono>
#include <random>
#include <atomic>
#include <fstream>
#include <windows.h>

#include <makcu.h>

#include "recoil.h"
#include "menu.h"

std::atomic<bool> leftPressed = false;
std::atomic<bool> rightPressed = false;

std::vector<int> recoil::table::x = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
std::vector<int> recoil::table::y = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

void mouseButtonCallback(makcu::MouseButton button, bool pressed)
{
    if (button == makcu::MouseButton::LEFT)
        leftPressed = pressed;

    if (button == makcu::MouseButton::RIGHT)
        rightPressed = pressed;
}

bool recoil::table::SavePattern(const char* filename)
{
    std::ofstream out(filename);
    if (!out.is_open())
        return false;

    out << recoil::table::x.size() << "\n";

    for (size_t i = 0; i < recoil::table::x.size(); i++)
        out << recoil::table::x[i] << " " << recoil::table::y[i] << "\n";

    MessageBox(NULL, "Custom patern has been saved.", "llama-makcu-universal", MB_OK);
    return true;
}
bool recoil::table::LoadPattern(const char* filename)
{
    std::ifstream in(filename);
    if (!in.is_open())
        return false;

    size_t size;
    in >> size;

    recoil::table::x.resize(size);
    recoil::table::y.resize(size);

    for (size_t i = 0; i < size; i++)
        in >> recoil::table::x[i] >> recoil::table::y[i];

    MessageBox(NULL, "Custom patern has been loaded.", "llama-makcu-universal", MB_OK);
    return true;
}

void AssignPattern(int& x, int& y, int shot_count)
{
    switch (menu::toggles::selected_recoil_mode)
    {
    case 0:
        x = menu::toggles::control_x;
        y = menu::toggles::control_y;
        break;
    case 1:
        x = recoil::table::x[shot_count];
        y = recoil::table::y[shot_count];
    }
}

void recoil::Manager(makcu::Device& device)
{
    int shot_count = 0;
    auto start_time = std::chrono::steady_clock::now();

    while (true)
    {
        int delay = static_cast<int>(menu::toggles::delay);
        int dx, dy;

        if (!menu::toggles::enable_recoil)
            continue;

        if (leftPressed && rightPressed)
        {
            // Calculate elapsed time in seconds
            auto current_time = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count() / 1000.0f;

            AssignPattern(dx, dy, shot_count);

            // We don't want to cap the shot count when using basic X, Y slider values for movement.
            if (menu::toggles::selected_recoil_mode == 1)
            {
                if (shot_count < recoil::table::x.size())
                    device.mouseMove(dx, dy);
            }
            else if (menu::toggles::selected_recoil_mode == 0)
            {
                // Only move mouse if within the cont_time duration
                if (elapsed < menu::toggles::cont_time && menu::toggles::enable_cont_time)
                {
                    device.mouseMove(dx, dy);
                }
                else if (!menu::toggles::enable_cont_time)
                {
                    device.mouseMove(dx, dy);
                }
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            shot_count++;
        }
        else
        {
            shot_count = 0;
            start_time = std::chrono::steady_clock::now(); // Reset timer when buttons are released
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}