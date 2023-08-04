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
 * @file    MK64F12_Project_i2c_tst.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"
//#include "fsl_adapter_i2c.c"
#include "fsl_i2c.h"

/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */

/*
 * @brief   Application entry point.
 */

void stat(uint8_t st, char* str);
int main(void) {

	/* Init board hardware. */
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
	/* Init FSL debug console. */
	BOARD_InitDebugConsole();
#endif
	/* ----------------------------------------------------------------------------------------- */
	/* i2c configuration  */
	port_pin_config_t i2c_pin_config = {
		.pullSelect = kPORT_PullDisable,
		.slewRate = kPORT_FastSlewRate,
		.passiveFilterEnable = kPORT_PassiveFilterDisable,
		.openDrainEnable = kPORT_OpenDrainEnable,
		.driveStrength = kPORT_LowDriveStrength,
		.mux = kPORT_MuxAlt5,					// select i2c
		.lockRegister = kPORT_UnlockRegister,
	};

	uint8_t i2c_scl = 24;
	uint8_t i2c_sda = 25;
	CLOCK_EnableClock(kCLOCK_PortE);
	PORT_SetPinConfig ( PORTE , i2c_scl , &i2c_pin_config  );
	PORT_SetPinConfig ( PORTE , i2c_sda , &i2c_pin_config  );

	i2c_master_config_t i2cConfig;

	i2cConfig.enableMaster = 1;
	i2cConfig.enableStopHold = 0;
	i2cConfig.baudRate_Bps = 100000;
	i2cConfig.glitchFilterWidth = 0;

	//	   PRINTF("i am world \n");

	I2C_MasterInit( I2C0, & i2cConfig , 120000000u );
	I2C_Enable ( I2C0 , 1 );

	uint8_t slave_addr = 0x1d;							// Accelerometer adddr
	status_t st;

	st = I2C_MasterStart ( I2C0 , slave_addr , kI2C_Write );
	stat(st,"master start");

	uint8_t who_am_i_reg = 0x0D;						// who am i register addr

	uint8_t packet[]={
		slave_addr,
		who_am_i_reg,
	};

	st = I2C_MasterWriteBlocking ( I2C0 , packet, sizeof(packet), kI2C_TransferDefaultFlag );	
	stat(st,"write block");

	uint8_t RxBuf;
	/* I2C_MasterStop ( I2C0 ); */
	I2C_MasterRepeatedStart ( I2C0, slave_addr, kI2C_Read );
	st = I2C_MasterReadBlocking ( I2C0 , &RxBuf , sizeof(RxBuf), kI2C_TransferDefaultFlag);	// return kStatus_Success
	stat( st ,"repeat start");

	PRINTF("\r\nval: %x\n",RxBuf);

	/* ----------------------------------------------------------------------------------------- */

	PRINTF("\r\nHello World\n");
	return 0 ;
}

void stat(uint8_t st, char* str){
	switch ( st ){
		case kStatus_Success:
			PRINTF("\r\n%s: kstatus succ ",str);
			break;
		case kStatus_I2C_Timeout:
			PRINTF("\r\n%s: kstatus timeout",str);
			break;
		case  kStatus_I2C_Busy: 
			PRINTF("\r\n%s: i2c line is busy ",str);
			break;
	}
}
