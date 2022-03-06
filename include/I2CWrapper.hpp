//
// Created by Lukasz on 06.03.2022.
//

#ifndef AIRCONTROLLER_I2CWRAPPER_HPP
#define AIRCONTROLLER_I2CWRAPPER_HPP

#include <unordered_map>

#include "driver/i2c.h"
#include "smbus.h"
#include "Singleton.h"

class I2CWrapper final : public Singleton<I2CWrapper> {
public:
    friend Singleton<I2CWrapper>;

    void ConfigureConnectionPins(int sdaPin,
                                 int sclPin,
                                 i2c_port_t i2cInterface = I2C_NUM_0);
    void AddDevice(uint8_t address, std::string_view friendlyName);
    void CheckCommunicationWithDevices();

    auto GetInfo(std::string_view friendlyName) -> smbus_info_t* {
        return devices_.find(friendlyName)->second.get();
    }

private:
    i2c_port_t i2cInterface_ = {};
    i2c_config_t connectionConfiguration_ = {};
    std::unordered_map<std::string_view, std::unique_ptr<smbus_info_t>>
        devices_;

    static bool isInit(const smbus_info_t* busInfo,
                       std::string_view deviceName);
    static void pingDevice(const smbus_info_t* busInfo,
                           std::string_view device);
};

#endif // AIRCONTROLLER_I2CWRAPPER_HPP
