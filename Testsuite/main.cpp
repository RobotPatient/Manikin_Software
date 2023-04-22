#include <communication.hpp>
#include <serial.hpp>
#include "gui_setup.h"
#include <stdio.h>

CommunicationManagerWindow::CommunicationData CommData;


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
  CommData.AvailableSerialPorts = Serial::get_available_ports();
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
    // Create communication manager window
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
  // Print settings from the Communication Manager Window
  int counter = 0;
  for (auto &device : CommData.DeviceProperties) {
    printf("Device: %d\n", counter++);
    if (device.InterfaceMethod == CommunicationManagerWindow::kSerial) {
      printf("Interfacemethod: Serial\n");
    } else {
      printf("Interfacemethod: TCP\n");
    }
    printf("InterfacePath: %s\n", device.InterfacePath.c_str());
  }
  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
  SDL_GL_DeleteContext(gl_context);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
