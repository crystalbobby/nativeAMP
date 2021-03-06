/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "sleep.h"
#include "xil_io.h"
#include "xresetps_hw.h"

//https://www.xilinx.com/html_docs/registers/ug1087/ug1087-zynq-ultrascale-registers.html
#define RST_LPD_TOP (XRESETPS_CRL_APB_RST_LPD_TOP)
#define RPU_R50_RESET (RPU_R50_RESET_MASK)
#define RPU_R51_RESET (RPU_R51_RESET_MASK)

int main()
{
	unsigned int cnt = 0;
	unsigned int value;

	init_platform();

    while(1){
    	printf("Hello World from APU0, run %d.\n", cnt++);
    	sleep(1);
    	// take R0 to reset
    	if (cnt == 10){
    		value = Xil_In32(RST_LPD_TOP);
    		printf("To RESET: RST_LPD_TOP: 0x%x.\n", value);
    		//preserve already reseted modules: USB1_APB_RESET_MASK, USB1_HIBERRESET_MASK, USB1_CORERESET_MASK
    		value = value | RPU_R50_RESET;
    		Xil_Out32(RST_LPD_TOP, value);
    		value = Xil_In32(RST_LPD_TOP);
    		printf("To RESET: RST_LPD_TOP: 0x%x.\n", value);
    	}
    	// take R0 from reset
    	else if (cnt == 20){
    		value = Xil_In32(RST_LPD_TOP);
    		printf("From RESET: RST_LPD_TOP: 0x%x.\n", value);
    		//preserve already reseted modules: USB1_APB_RESET_MASK, USB1_HIBERRESET_MASK, USB1_CORERESET_MASK
    		value = value ^ RPU_R50_RESET;
    		Xil_Out32(RST_LPD_TOP, value);
    		value = Xil_In32(RST_LPD_TOP);
    		printf("From RESET: RST_LPD_TOP: 0x%x.\n", value);
    	}
    }

    cleanup_platform();
    return 0;
}
