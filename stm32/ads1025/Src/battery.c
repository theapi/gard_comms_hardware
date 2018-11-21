/**
 * battery.c
 */

#include "ads1015.h"
#include "battery.h"

extern I2C_HandleTypeDef hi2c1;

uint16_t BATTERY_vcc() {
    /*
     * 2x gain   +/- 2.048V  1 bit = 1mV
     * 1x gain   +/- 4.096V  1 bit = 2mv
     * Battery is measured via a voltage divider = 620 / 100
     *
     * 1x gain:
     *   1v  = 71 bits = 142mV
     *   2v  = 141 bits = 282mV
     *   3v  = 213 bits = 426mV
     *   15V = 1065 bits = 2130mV
     *
     *   1000 / 71 = 14.084507042
     *   so 1 bit = 14.084507042 mV on the battery.
     *   .. 1065 bits = 1065 * 14.084507042 = 15V
     */
    uint16_t val = ADS1015_SingleEnded(&hi2c1, ADS1015_ADDRESS, 3, ADS1015_GAIN_ONE);
    /*
     * Multiply val by 14.084507042 to get the real mV before the voltage divider & gain.
     */
    float mV = (float)val * 14.084507042;
    return (int) mV;
}

uint16_t BATTERY_ChargeMv() {
    /*
     * Voltage divider = 300 / 100
     */
    uint16_t val = ADS1015_SingleEnded(&hi2c1, ADS1015_ADDRESS, 2, ADS1015_GAIN_TWO);
    return val * 4;
}

int16_t BATTERY_ChargeMa() {
    int16_t val = ADS1015_Differential_0_1(&hi2c1, ADS1015_ADDRESS, ADS1015_GAIN_TWO);
    /*
     * Invert as its a negative value referenced to ground when charging.
     */
    return val *-1;
}
