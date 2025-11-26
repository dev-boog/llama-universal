#include <iostream>
#include <thread>
#include <chrono>
#include <makcu.h>
#include <windows.h>


#include "recoil.h"
#include "window.h"

// WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    try {
        makcu::Device device;
        device.setMouseButtonCallback(mouseButtonCallback);
        device.connect();

        if (!device.isConnected())
        {
            MessageBox(NULL, "Could not find Makcu.", "llama-makcu-universal", MB_OK);
            return 1;
        }

        std::thread recoilThread([](makcu::Device& dev) { recoil::Manager(dev); }, std::ref(device));
        recoilThread.detach();

        window::render();

        while (true)
            std::this_thread::sleep_for(std::chrono::seconds(1));
        
    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}

