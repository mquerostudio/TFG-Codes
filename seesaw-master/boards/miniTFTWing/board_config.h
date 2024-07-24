#ifndef SEESAW_DEVICE_CONFIG_H
#define SEESAW_DEVICE_CONFIG_H

#define PRODUCT_CODE 0000

//override default I2C addr
#define CONFIG_I2C_SLAVE_ADDR 0x5E

#define CONFIG_NO_ACTIVITY_LED
#define CONFIG_NO_EEPROM

//* ============== POOL SIZES =================== *//
#define	EVT_SIZE_SMALL 16
#define EVT_SIZE_MEDIUM 32
#define	EVT_SIZE_LARGE 64
#define	EVT_COUNT_SMALL 16
#define	EVT_COUNT_MEDIUM 8
#define	EVT_COUNT_LARGE 1

//* ============== ADC =================== *//
#define CONFIG_ADC 0
#define CONFIG_ADC_INPUT_0 1

#define CONFIG_ADC_INPUT_1 1

#define CONFIG_ADC_INPUT_2 0

#define CONFIG_ADC_INPUT_3 0

//* ============== DAC =================== *//
#define CONFIG_DAC 0

//* ============== TIMER =================== *//
#define CONFIG_TIMER 1

//override default PWM0 pin
#define CONFIG_TIMER_PWM_OUT0 1

#define CONFIG_TIMER_PWM_OUT0_TC TC1
#define CONFIG_TIMER_PWM_OUT0_WO 1
#define CONFIG_TIMER_PWM_OUT0_PIN 5

#define CONFIG_TIMER_PWM_OUT1 0

#define CONFIG_TIMER_PWM_OUT2 0

#define CONFIG_TIMER_PWM_OUT3 0

//* ============== INTERRUPT =================== *//
#define CONFIG_INTERRUPT 0
#define CONFIG_INTERRUPT_PIN 8

//* ============== I2C SLAVE =================== *//
#define CONFIG_I2C_SLAVE 1

//* ============== SERCOM =================== *//
#define CONFIG_SERCOM0 0
#define CONFIG_SERCOM1 0
#define CONFIG_SERCOM2 0

//These are only available on samd21
#define CONFIG_SERCOM3 0
#define CONFIG_SERCOM4 0
#define CONFIG_SERCOM5 0

//* ============== DAP =================== *//
#define CONFIG_DAP 0

//* =========== NEOPIXEL ================ *//
#define CONFIG_NEOPIXEL 0

#define CONFIG_NEOPIXEL_BUF_MAX 1024

#endif