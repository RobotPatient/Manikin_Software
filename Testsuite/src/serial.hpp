#ifndef DESKTOP_TESTSUITE_SRC_SERIAL_HPP_
#define DESKTOP_TESTSUITE_SRC_SERIAL_HPP_
#include <boost/asio.hpp>
#include <iostream>
#include <vector>
#ifdef __WIN32__
#include <windows.h>
#else
#include <boost/filesystem.hpp>
#include <algorithm>
#endif

namespace Serial {
typedef boost::shared_ptr<boost::asio::serial_port> mSerialPortPtr;

typedef struct {
  mSerialPortPtr SerialPort;
  boost::asio::io_service IOService;
}BoostSerial;

std::vector<BoostSerial> SerialConnections;

namespace fs = boost::filesystem;

#ifdef __linux__
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


#elifdef __WIN32__
std::vector<std::string> get_available_ports() {
  std::vector<std::string> port_names;
  for(int i =1; i < 256; i++){
     //Form the Raw device name
    ATL::CAtlString sPort;
    sPort.Format(_T("\\\\.\\COM%u"), i);

    //Try to open the port
    bool bSuccess{false};
    ATL::CHandle port{CreateFile(sPort, GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr)};
    if (port == INVALID_HANDLE_VALUE)
    {
      const DWORD dwError{GetLastError()};

      //Check to see if the error was because some other app had the port open or a general failure
      if ((dwError == ERROR_ACCESS_DENIED) || (dwError == ERROR_GEN_FAILURE) || (dwError == ERROR_SHARING_VIOLATION) || (dwError == ERROR_SEM_TIMEOUT))
        bSuccess = true;
    }
    else
    {
      //The port was opened successfully
      bSuccess = true;
    }

    //Add the port number to the array which will be returned
    if (bSuccess)
#pragma warning(suppress: 26489)
      port_names.push_back(i);
  }
  return port_names;
}
#elifdef __APPLE__
std::vector<std::string> get_available_ports() {
  std::vector<std::string> port_names;
  fs::path p("/dev/tty");
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
#endif
}
#endif //DESKTOP_TESTSUITE_SRC_SERIAL_HPP_
