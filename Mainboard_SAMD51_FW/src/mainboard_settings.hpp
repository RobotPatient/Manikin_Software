#ifndef MAINBOARD_SETTINGS_HPP
#define MAINBOARD_SETTINGS_HPP
#include <SPI.h>
#include <variant.h>

inline constexpr int kSpiClockSpeed = 25000000;
inline constexpr uint8_t kSpiRpiClkPin = 25; //PA17
inline constexpr uint8_t kSpiRpiMosiPin = 24; //PB23
inline constexpr uint8_t kSpiRpiMisoPin = 23; //PB22

inline constexpr uint8_t kSpiFramClkPin = PIN_QSPI_SCK;
inline constexpr uint8_t kSpiFramData0Pin = PIN_QSPI_IO0;
inline constexpr uint8_t kSpiFramData1Pin = PIN_QSPI_IO1;
inline constexpr uint8_t kSpiFramData2Pin = PIN_QSPI_IO2;
inline constexpr uint8_t kSpiFramData3Pin = PIN_QSPI_IO3;
inline constexpr uint8_t kSpiFramCsPin = PIN_QSPI_CS;

SPIClass spi_bus (&sercom1, kSpiRpiMisoPin, kSpiRpiClkPin, kSpiRpiMosiPin, SPI_PAD_3_SCK_1, SERCOM_RX_PAD_2);
const SPISettings kspi_bus_settings(kSpiClockSpeed, MSBFIRST, SPI_MODE0);


#endif