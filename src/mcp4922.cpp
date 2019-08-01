#include <mbed.h>
#include "mcp4922.h"

MCP4922::MCP4922(SPI* spi, bool grantOwnership, PinName ldac) {
    /* Save the SPI port */
    this->m_spi = spi;
    this->m_spiOwnership = grantOwnership;
    this->m_spiFrequency = 1000000;
    this->m_ldac = new DigitalOut(ldac, 1);

    /* Set format */
    this->m_spi->format(16, 0);
    /* Set frequency */
    this->m_spi->frequency(this->m_spiFrequency);

}

MCP4922::MCP4922(SPI* spi, PinName ldac) 
        :MCP4922(spi, false, ldac) {}

MCP4922::MCP4922(PinName mosi, PinName miso, PinName sclk, PinName ssel, PinName ldac)
        :MCP4922(new SPI(mosi, miso, sclk, ssel), true, ldac) {}

MCP4922::MCP4922(PinName mosi, PinName miso, PinName sclk, PinName ssel, PinName ldac, mbed::use_gpio_ssel_t)
        :MCP4922(new SPI(mosi, miso, sclk, ssel, mbed::use_gpio_ssel), true, ldac) {}


MCP4922::~MCP4922() {
    /* Release SPI if ownership */
    if (this->m_spiOwnership) {
        delete this->m_spi;
    }
    delete this->m_ldac;
}


const SPI* MCP4922::getSPI() const {
    return this->m_spi;
}

uint32_t MCP4922::getFrequency() const {
    return this->m_spiFrequency;
}

void MCP4922::setFrequency(uint32_t freq) {
    this->m_spiFrequency = freq;
    this->m_spi->frequency(freq);
}


void MCP4922::write(uint8_t dac, uint16_t data, bool doublegain, bool buffered, bool shutdown) {
    /* Clamp dac */
    dac &= MCP4922_DAC_B;
    /* Compose the message */
    uint16_t command = 
        /* The DAC data */
        (data & 0xFFF)
        /* Options */
        | (shutdown ? 0 : (((uint16_t)1) << 12))
        /* The gain */
        | (doublegain ? 0 : (((uint16_t)1) << 13))
        /* The buffering */
        | (!buffered ? 0 : (((uint16_t)1) << 14))
        /* The DAC */
        | (((uint16_t)dac) << 15);
    /* Raise LDAC */
    *(this->m_ldac) = 1;
    /* Send it */
    this->m_spi->select();
    this->m_spi->write(command);
    this->m_spi->deselect();
}

void MCP4922::update() {
    /* Lower LDAC */
    *(this->m_ldac) = 0;
    wait_ns(10);
    *(this->m_ldac) = 1;
}