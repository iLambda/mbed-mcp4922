#ifndef __MBED_MCP4922
#define __MBED_MCP4922

#include <mbed.h>

#define MCP4922_DAC_A   0x00
#define MCP4922_DAC_B   0x01

class MCP4922 {

    private:
        /* The underlying SPI link */
        SPI* m_spi;
        /* The ownership of the SPI link */
        bool m_spiOwnership;
        /* The frequency */
        uint32_t m_spiFrequency;
        /* The LDAC pin */
        DigitalOut* m_ldac;

    private:
        /* Creates a MCP4922 given a SPI port */
        MCP4922(SPI* spi, bool grantOwnership, PinName ldac);
    public:
        /* Creates a MCP4922 given a SPI port */
        MCP4922(SPI* spi, PinName ldac);
        /* Creates a MCP4922 given pin namthis->m_spiFrequency;es */
        MCP4922(PinName mosi, PinName miso, PinName sclk, PinName ssel, PinName ldac);
        /* Destructs a MCP4922 */
        ~MCP4922();

    public:
        /* Get the SPI link */
        const SPI* getSPI() const;    
        /* Get the frequency */
        uint32_t getFrequency() const;
        /* Set the frequency */
        void setFrequency(uint32_t freq);

        /* Write */
        void write(uint8_t dac, uint16_t data, bool doublegain = false, bool buffered = false, bool shutdown = false);
        /* Update */
        void update();
};

#endif