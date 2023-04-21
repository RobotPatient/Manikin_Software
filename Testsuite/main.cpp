// Dear ImGui: standalone example application for SDL2 + OpenGL
// (SDL is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#include "gui_setup.h"
#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <communication.hpp>

CommunicationManagerWindow::CommunicationData CommData;

typedef boost::shared_ptr<boost::asio::serial_port> mSerialPortPtr;
mSerialPortPtr mSerialPort;
boost::asio::io_service mIOService;

namespace fs = boost::filesystem;

std::vector<std::string> get_available_ports() {
  std::vector<std::string> port_names;

  fs::path p("/dev/serial/by-id");
  try {
    if (!exists(p)) {
      //throw std::runtime_error(p.generic_string() + " does not exist");
    } else {
      for (fs::directory_entry &de : fs::directory_iterator(p)) {
        if (is_symlink(de.symlink_status())) {
          fs::path symlink_points_at = read_symlink(de);
          fs::path canonical_path = fs::canonical(symlink_points_at, p);
          port_names.push_back(canonical_path.generic_string());
        }
      }
    }
  } catch (const fs::filesystem_error &ex) {
    std::cout << ex.what() << '\n';
  }
  std::sort(port_names.begin(), port_names.end());
  return port_names;
}

int main(int, char **) {
  setup_gui_renderer();
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void) io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  //ImGui::StyleColorsLight();

  // Setup Platform/Renderer backends
  ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
  ImGui_ImplOpenGL3_Init(glsl_version);
  // Our state
  ImVec4 clear_color = ImVec4(255.00f, 255.00f, 255.00f, 255.00f);
  // Main loop
  bool done = false;
  bool show_demo_window = true;
  bool show_another_window = false;
  mSerialPort = mSerialPortPtr(new boost::asio::serial_port(mIOService));
  CommData.AvailableSerialPorts = get_available_ports();
  for(auto &p : CommData.AvailableSerialPorts)
    printf("%s\n",p.c_str());
  while (!done) {
    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
    // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      ImGui_ImplSDL2_ProcessEvent(&event);
      if (event.type == SDL_QUIT)
        done = true;
      if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE
          && event.window.windowID == SDL_GetWindowID(window))
        done = true;
    }

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    ImGui::ShowDemoWindow(&show_demo_window);


    CommunicationManagerWindow::Create(&CommData);


    // Rendering
    ImGui::Render();
    glViewport(0, 0, (int) io.DisplaySize.x, (int) io.DisplaySize.y);
    glClearColor(clear_color.x * clear_color.w,
                 clear_color.y * clear_color.w,
                 clear_color.z * clear_color.w,
                 clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window);
  }
  uint8_t counter = 0;
  for(auto &i : CommData.DeviceProperties) {
    printf("Device: %d\n",counter++);
    if(i.InterfaceMethod == CommunicationManagerWindow::kSerial){
      printf("Interfacemethod: Serial\n");
    } else {
      printf("Interfacemethod: TCP\n");
    }
    printf("InterfacePath: %s\n", i.InterfacePath.c_str());

  }

  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
  SDL_GL_DeleteContext(gl_context);
  SDL_DestroyWindow(window);
  SDL_Quit();
  mSerialPort->close();

  return 0;
}
