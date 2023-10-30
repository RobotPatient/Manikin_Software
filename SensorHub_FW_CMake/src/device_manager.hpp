//
// Created by victor on 10/30/23.
//

#ifndef ATMELSAMD21_DEVICE_MANAGER_HPP
#define ATMELSAMD21_DEVICE_MANAGER_HPP
#include <FreeRTOS.h>
#include <queue.h>

class DeviceManager {
public:
    DeviceManager() {

    }

    void Init(xSTATIC_QUEUE incoming_messages);

    void Poll();

};

#endif //ATMELSAMD21_DEVICE_MANAGER_HPP
