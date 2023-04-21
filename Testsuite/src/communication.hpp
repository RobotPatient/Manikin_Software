//
// Created by victor on 4/21/23.
//

#ifndef DESKTOP_TESTSUITE_SRC_COMMUNICATION_HPP_
#define DESKTOP_TESTSUITE_SRC_COMMUNICATION_HPP_
#include <iostream>
#include <vector>
namespace CommunicationManagerWindow {

typedef enum{
  kSerial,
  kTCP
}CommunicationMethod;

typedef struct{
  CommunicationMethod InterfaceMethod;
  std::string InterfacePath;
}CommunicationDevice;

typedef struct{
  std::vector<std::string> AvailableSerialPorts;
  std::vector<CommunicationDevice> DeviceProperties;
}CommunicationData;

void Create(CommunicationData* data);
void Destroy();
void Show();
void Hide();

}
#endif //DESKTOP_TESTSUITE_SRC_COMMUNICATION_HPP_
