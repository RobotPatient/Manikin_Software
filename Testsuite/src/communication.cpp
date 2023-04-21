#include <communication.hpp>
#include <imgui.h>
#include <stdlib.h>
#include <stdint.h>

namespace CommunicationManagerWindow {
static char deviceSpecifier[20];
static char tempIPAddr[100];
float TempXPos;
void Create(CommunicationData *data) {
  ImGui::Begin("Connection manager");                          // Create a window called "Hello, world!" and append into it.
  ImGui::Text("How many devices to monitor?");               // Display some text (you can use a format strings too)
  static int amount_of_devices = 1;
  ImGui::InputInt("Devices", &amount_of_devices, 1, 100);
  if (amount_of_devices < 1) amount_of_devices = 1;
  if (data->DeviceProperties.size() < amount_of_devices) {
    for (uint8_t i = data->DeviceProperties.size(); i < amount_of_devices; i++) {
      CommunicationDevice CommDevice;
      data->DeviceProperties.push_back(CommDevice);
      data->DeviceProperties.at(i).InterfaceMethod = kSerial;
    }
  } else if (data->DeviceProperties.size() > amount_of_devices) {
    for (uint8_t i = amount_of_devices; data->DeviceProperties.size() > amount_of_devices; i++) {
      data->DeviceProperties.pop_back();
    }
  }
  int DeviceCounter = 1;
  for (auto &device : data->DeviceProperties) {
    sprintf(deviceSpecifier, "Device: %d", DeviceCounter++);
    if (ImGui::TreeNode(deviceSpecifier)) {
      ImGui::Text("Connection type:");
      ImGui::SameLine();
      TempXPos = ImGui::GetCursorPosX();
      if (ImGui::RadioButton("Serial", device.InterfaceMethod == kSerial)) { device.InterfaceMethod = kSerial; };
      ImGui::SetCursorPosX(TempXPos);
      if (ImGui::RadioButton("TCP", device.InterfaceMethod == kTCP)) { device.InterfaceMethod = kTCP; };
      ImGui::Text("Enter or select the PORT/IP:");
      if (device.InterfaceMethod == kSerial) {
        int selectedItem;
        if (selectedItem < 0 || selectedItem > data->AvailableSerialPorts.size()) selectedItem = 0;
        ImGui::Combo("Port",
                     &selectedItem,
                     (const char *const *) data->AvailableSerialPorts.data(),
                     data->AvailableSerialPorts.size(),
                     -1);
        if (data->AvailableSerialPorts.size())
          device.InterfacePath = data->AvailableSerialPorts.at(selectedItem);
      } else {
        ImGui::InputText("IP", tempIPAddr, 100, 0);
        device.InterfacePath = tempIPAddr;
      }
      ImGui::TreePop();
    }
  }
  ImGui::End();
}
void Destroy() {

}
void Show() {

}
void Hide() {

}

}