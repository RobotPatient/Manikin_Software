#ifndef DESKTOP_TESTSUITE_SRC_SERIAL_H_
#define DESKTOP_TESTSUITE_SRC_SERIAL_H_
#include <boost/asio.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <boost/filesystem.hpp>

namespace Serial {
typedef boost::shared_ptr<boost::asio::serial_port> mSerialPortPtr;

typedef struct {
  mSerialPortPtr SerialPort;
  boost::asio::io_service IOService;
}BoostSerial;
std::vector<BoostSerial> SerialConnections;

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

}
#endif //DESKTOP_TESTSUITE_SRC_SERIAL_H_
