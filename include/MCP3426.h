#pragma once

#include <I2C.h>

class MCP3426 {
  private:
    I2C* i2c;
    I2C::Config i2cConfig;

    int channelIndex(int channel);

  public:
    class Config {
      public:
        // Conversion mode
        unsigned char mode;
        // Input channel
        unsigned char channel;
        // Bit resolution of values
        unsigned char resolution;
        // Input gain
        unsigned char gain;

        Config();
        Config(unsigned char channel, unsigned char mode,
               unsigned char resolution, unsigned char gain);
        // Get the config byte to send via I2C
        unsigned char getByte();
        // Get the maximum possible value according to configuration
        float getMaxValue();
        // Get the minimum possible value according to configuration
        float getMinValue();
    };

    static const unsigned char I2C_ADDRESS = 0x68;
    static const unsigned char MODE_ONESHOT = 0x00;
    static const unsigned char MODE_CONTINUOUS = 0x10;
    static const unsigned char CHANNEL1 = 0x00;
    static const unsigned char CHANNEL2 = 0x20;
    static const unsigned char RESOLUTION_12BIT = 0x00;
    static const unsigned char RESOLUTION_14BIT = 0x04;
    static const unsigned char RESOLUTION_16BIT = 0x08;
    static const unsigned short SIGN_12BIT = 0x800;
    static const unsigned short SIGN_14BIT = 0x2000;
    static const unsigned short SIGN_16BIT = 0x8000;
    static const unsigned char GAIN_1X = 0x00;
    static const unsigned char GAIN_2X = 0x01;
    static const unsigned char GAIN_4X = 0x02;
    static const unsigned char GAIN_8X = 0x03;
    static const unsigned char READY_BIT = 0x80;
    static constexpr float VREF = 2.048;
    static constexpr float LSB_16BIT = 0.0000625;
    static constexpr float LSB_14BIT = 0.00025;
    static constexpr float LSB_12BIT = 0.001;

    Config config[2];
    bool valueUpdated;

    MCP3426();
    MCP3426(I2C* i2c);
    // Initialize I2C device
    void init();
    void configure(unsigned char channel, unsigned char mode,
                   unsigned char resolution, unsigned char gain);
    void configure(unsigned char channel, Config config);
    // Start analog-to-digital conversion according to calibration data
    void startConversion(unsigned char channel);
    // Indicates if data on the chip is up to date
    bool conversionFinished();
    // Retrieve the input voltage value to the chip
    float getValue(unsigned char channel);
};