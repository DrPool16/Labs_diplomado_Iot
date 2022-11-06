
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "K32L2B31A.h"
#include "fsl_debug_console.h"
#include "fsl_adc16.h"


volatile uint32_t g_systickCounter;

void SysTick_Handler(void)
{
    if (g_systickCounter != 0U)
    {
        g_systickCounter--;
    }
}

void SysTick_DelayTicks(uint32_t n)
{
    g_systickCounter = n;
    while (g_systickCounter != 0U)
    {
    }
}



//#include "test.h"
/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */

/*
 * @brief   Application entry point.
 */
//volatile static uint8_t i = 0;
//float voltaje = 12.5f;

//uint8_t mensaje[10];

int main(void) {

    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();

    if (SysTick_Config(SystemCoreClock / 1000U))
    {
        while (1)
        {
        }
    }

#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    uint32_t dato_sensor;
    float k1 = 3.3/4095;
    float k2 = 3.29/-0.01;
    float k3 = 1/0.2;

    float voltaje;
    float corriente;
    float lux;

    /* Force the counter to be placed into memory. */
        //volatile static int i = 0 ;


   /* PRINTF("Volatje: %2.2f \r\n", voltaje); */
    /* Enter an infinite loop, just incrementing a counter. */
    while(1) {


        // Configurar canal adc
        ADC16_SetChannelConfig(ADC0_PERIPHERAL, ADC0_CH0_CONTROL_GROUP,  &ADC0_channelsConfig[0]);

        //Espera que el ADC finalice el ADC
        while (0U == (kADC16_ChannelConversionDoneFlag &
                              ADC16_GetChannelStatusFlags(ADC0_PERIPHERAL, ADC0_CH0_CONTROL_GROUP)))
                {
                }

        /* Captura dato ADC e imprime por consola */
        dato_sensor = ADC16_GetChannelConversionValue(ADC0_PERIPHERAL, ADC0_CH0_CONTROL_GROUP);
        PRINTF("ADC Value: %d\r\n", dato_sensor);

        voltaje = dato_sensor*k1;
        PRINTF("Voltaje[V]: %2.2f\r\n", voltaje);

        corriente = (voltaje/-0.01)-k2+1;
        PRINTF("Corriente[uA]: %2.2f\r\n", corriente);

        lux = corriente*k3;
        PRINTF("LUX: %2.2f\r\n", lux);


        SysTick_DelayTicks(1000U);
        //i++ ;
        //test(&i);
        /* 'Dummy' NOP to allow source level single stepping of
            tight while() loop */
        __asm volatile ("nop");
    }
    return 0 ;
}
