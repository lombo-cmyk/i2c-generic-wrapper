//
// Created by Lukasz on 06.03.2022.
//
#include "I2CWrapper.hpp"
#include "Logger.hpp"

void I2CWrapper::Init(int sdaPin, int sclPin, i2c_port_t i2cInterface) {
    i2cInterface_ = i2cInterface;
    connectionConfiguration_.mode = I2C_MODE_MASTER;
    connectionConfiguration_.sda_io_num = sdaPin;
    connectionConfiguration_.scl_io_num = sclPin;
    connectionConfiguration_.sda_pullup_en = GPIO_PULLUP_ENABLE;
    connectionConfiguration_.scl_pullup_en = GPIO_PULLUP_ENABLE;
    connectionConfiguration_.master.clk_speed = 100000;
    i2c_param_config(I2C_NUM_0, &connectionConfiguration_);
    i2c_driver_install(I2C_NUM_0, connectionConfiguration_.mode, 0, 0, 0);
}

void I2CWrapper::AddDevice(const uint8_t address,
                           std::string_view friendlyName) {
    auto smbus_info = std::make_unique<smbus_info_t>();
    smbus_init(smbus_info.get(), i2cInterface_, address);
    smbus_set_timeout(smbus_info.get(), 10000 / portTICK_RATE_MS);
    devices_[friendlyName] = std::move(smbus_info);
}
void I2CWrapper::CheckCommunicationWithDevices() {
    for (auto const& [name, busInfo] : devices_) {
        isInit(busInfo.get(), name);
        pingDevice(busInfo.get(), name);
    }
}
bool I2CWrapper::isInit(const smbus_info_t* busInfo,
                        std::string_view deviceName) {
    bool ok = false;
    if (busInfo != nullptr) {
        if (busInfo->init) {
            ok = true;
        } else {
            LogError(deviceName, "Is not initialised");
        }
    } else {
        LogError(deviceName, "Is NULL");
    }
    LogInfo(deviceName, "Bus configured properly");
    return ok;
}
void I2CWrapper::pingDevice(const smbus_info_t* busInfo,
                            std::string_view deviceName) {
    esp_err_t error = smbus_quick(busInfo, I2C_MASTER_WRITE);
    if (error == ESP_OK) {
        LogInfo(deviceName, "I2C communication established");
    } else {
        LogError(deviceName,
                 "I2C communication not established, error: ",
                 error);
    }
}
