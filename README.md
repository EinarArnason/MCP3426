# MCP3426 library

A C++ driver library for the MCP3426 Analog-to-Digital Converter

## The What

The MCP3426, MCP3427 and MCP3428 devices are the low noise and high accuracy 16 Bit Delta-Sigma Analog-to-Digital (ΔΣ A/D) Converter family members of the MCP342X series from Microchip Technology Inc. These devices can convert analog inputs to digital codes with up to 16 bits of resolution. This library is written for the MCP3426, but can be used for the MCP342X family. However, that remains untested for now.

## The Why

The aim is to provide a driver that is not platform specific. Therefor functionality such as exceptions and direct I<sup>2</sup>C communications is not implemented and platform dependant libraries are not included. Either a core functionality or interface is provided that can be adapted to the target platform.

## The How

### Building

#### Using cmake

Content of CMakeLists.txt:

```cmake
add_subdirectory(${mcp3426Dir})
target_link_libraries(${PROJECT_NAME} MCP3426)
```

### Usage

Typical operation. The I<sup>2</sup>C object passed to the constructor depends on platform.

```c++
#include <MCP3426.h>

MCP3426::Config config(MCP3426::CHANNEL1, MCP3426::MODE_ONESHOT, MCP3426::RESOLUTION_16BIT, MCP3426::GAIN_1X);

MCP3426 adc(i2c);
adc.init();
adc.configure(config);
adc.startConversion();

while (!adc.conversionFinished()) {
    // Preferred sleep function
}

float value = adc.getValue(MCP3426::CHANNEL1);

```

## The Who

Einar Arnason  
<https://github.com/EinarArnason>  
<https://www.linkedin.com/in/einararnason/>
