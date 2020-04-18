#include "MCP3426.h"

MCP3426::Config::Config() {
    // Default configuration
    mode = MODE_ONESHOT;
    channel = CHANNEL1;
    resolution = RESOLUTION_16BIT;
    gain = GAIN_1X;
}

MCP3426::Config::Config(unsigned char channel, unsigned char mode,
                        unsigned char resolution, unsigned char gain) {
    this->channel = channel;
    this->mode = mode;
    this->resolution = resolution;
    this->gain = gain;
}

unsigned char MCP3426::Config::getByte() {
    return channel | mode | resolution | gain | READY_BIT;
}

float MCP3426::Config::getMaxValue() {
    int maxInt = 0;

    if (resolution == RESOLUTION_12BIT) {
        maxInt = 1 << 12;
    } else if (resolution == RESOLUTION_14BIT) {
        maxInt = 1 << 14;
    } else if (resolution == RESOLUTION_16BIT) {
        maxInt = 1 << 16;
    }

    return (2 * VREF) / maxInt;
}

float MCP3426::Config::getMinValue() {
    int maxInt = 1;

    if (resolution == RESOLUTION_12BIT) {
        maxInt = 1 << 12;
    } else if (resolution == RESOLUTION_14BIT) {
        maxInt = 1 << 14;
    } else if (resolution == RESOLUTION_16BIT) {
        maxInt = 1 << 16;
    }

    return -((2 * VREF) / maxInt);
}

MCP3426::MCP3426() {
    i2c = nullptr;
    i2cConfig.address = 0x68;
    i2cConfig.frequency = 400000;
    valueUpdated = false;
}

MCP3426::MCP3426(I2C* i2c) {
    this->i2c = i2c;
    i2cConfig.address = 0x68;
    i2cConfig.frequency = 400000;
    valueUpdated = false;
}

void MCP3426::init() {
    if (i2c != nullptr) {
        i2c->init();
    }
}

int MCP3426::channelIndex(int channel) { return channel >> 5; }

void MCP3426::configure(unsigned char channel, unsigned char mode,
                        unsigned char resolution, unsigned char gain) {
    config[channelIndex(channel)] = Config(channel, mode, resolution, gain);
}

void MCP3426::configure(unsigned char channel, Config config) {
    this->config[channelIndex(channel)] = config;
}

void MCP3426::startConversion(unsigned char channel) {
    if (i2c == nullptr) {
        return;
    }

    char cmd[] = {(char)config[channelIndex(channel)].getByte()};
    i2c->send(i2cConfig, cmd, sizeof(cmd));
}

bool MCP3426::conversionFinished() {
    unsigned char data[3];

    if (!i2c->receive(i2cConfig, (char*)data, sizeof(data))) {
        return false;
    }

    // The ready bit is cleared when data is ready
    bool ready = !(data[2] & READY_BIT);

    return ready;
}

float MCP3426::getValue(unsigned char channel) {
    unsigned char data[2];
    Config conf = config[channelIndex(channel)];

    // Read the data registers
    if (!i2c->receive(i2cConfig, (char*)data, sizeof(data))) {
        return 0;
    }

    short value;
    // Data is MSB first
    value = data[0] << 8 | data[1];

    float lsb = LSB_16BIT;
    int gain = 1 << conf.gain;

    // Preserve sign on values less than 16 bits
    if (conf.resolution == RESOLUTION_14BIT) {
        lsb = LSB_14BIT;
        value <<= 2;
        value >>= 2;
    } else if (conf.resolution == RESOLUTION_12BIT) {
        lsb = LSB_12BIT;
        value <<= 4;
        value >>= 4;
    }

    float result = value * (lsb / gain);

    return result;
}
