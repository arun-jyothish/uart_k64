/*
 * Copyright 2016-2023 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file    MK64F12_Project.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_uart.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */

/*
 * @brief   Application entry point.
 */
int main(void) {

    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();

    /*
     user created uart
     -------------------------------------*/
    uart_config_t config;
    UART_GetDefaultConfig( &config);
    UART_Init(UART0, &config , CLOCK_GetFreq(UART0_CLK_SRC));
    UART_EnableRx (UART0, 1);
    UART_EnableTx (UART0, 1);
    /*  --------------------------------  */

    uint8_t txbuf[100] , rxbuf[50];

    UART_EnableInterrupts(UART0, kUART_RxDataRegFullInterruptEnable ); // | kUART_TxDataRegEmptyInterruptEnable

    EnableIRQ(UART0_RX_TX_IRQn);
    while(1);
    return 0 ;


}

void UART0_RX_TX_IRQHandler()
{
	uint8_t buffTx[] = "\r\n Recieved a msg from UART0: ";
	uint8_t buffRx[10];

	if ( kUART_RxDataRegFullFlag & UART_GetStatusFlags(UART0) ){
		UART_ReadBlocking ( UART0 ,  buffRx , sizeof(buffRx));
		UART_WriteBlocking( UART0 , buffTx, sizeof(buffTx));
		UART_WriteBlocking( UART0 , buffRx, sizeof(buffRx));
	}
	UART_ClearStatusFlags( UART0 , kUART_RxDataRegFullInterruptEnable);
}
