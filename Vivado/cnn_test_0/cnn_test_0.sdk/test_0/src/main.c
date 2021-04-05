/**
 * Project 20-1-1-2187
 * CNN accelerator
 *
 * Chaim Gruda
 * Shay Tsabar
 *
 */

/*
 * INCLUDES
 ******************************************************************************
 */

#include <stdio.h>
#include "conv_0.h"
#include "pool_0.h"
#include "cnn_hw.h"
#include "dbg.h"
#include "fixed_point.h"

/*
 * MAIN
 ******************************************************************************
 */

#define MEM_BASE_ADDR 0x01000000
#define TX_BUFFER_BASE (MEM_BASE_ADDR + 0x00100000)
#define RX_BUFFER_BASE (MEM_BASE_ADDR + 0x00300000)


#define INPUT_DATA_LEN (94 * 94)
#define OUTPUT_DATA_LEN (46 * 46)
#define KERNEL_DATA_LEN 9


int main()
{
	int status = XST_SUCCESS;
	struct cnn_hw cnn_hw = {0};

	// DBG_LOG_PRINT(TRACE, "hello world\n");


	/******************** temp ***************************/
	u32 *p_dma_buffer_TX = (u32*) TX_BUFFER_BASE;
	u32 *p_dma_buffer_RX = (u32*) RX_BUFFER_BASE;

	u32 kernel_data[KERNEL_DATA_LEN];
	u32 input_data[INPUT_DATA_LEN];
	u32 conv_0_ctrl, pool_0_ctrl;

	// DBG_LOG_PRINT(TRACE, "pre loop\n");
	printf("hello world\n");

	for (int i = 0; i < INPUT_DATA_LEN; i++) {
		input_data[i] = i;
	}
	//DBG_LOG_PRINT(TRACE, "post loop\n");


	for (int i = 0; i < KERNEL_DATA_LEN; i++) {
		kernel_data[i] = 1;
	}
	conv_0_ctrl = 0; // no relu
	pool_0_ctrl = 0; // max pool
	/*******************************************************/

	//DBG_LOG_PRINT(TRACE, "pre init\n");

	printf("before init\n");

	status = cnn_hw_init(&cnn_hw);
	if (status != XST_SUCCESS) {
		return status;
	}
	printf("after init\n");

	//DBG_LOG_PRINT(TRACE, "post init\n");

	/******************** temp ***************************/
	do {
		conv_0_kernel_set(&cnn_hw.conv_0_hw, kernel_data);
		conv_0_ctrl_set(&cnn_hw.conv_0_hw, conv_0_ctrl);
		pool_0_ctrl_set(&cnn_hw.pool_0_hw, pool_0_ctrl);
		for (int i = 0; i < INPUT_DATA_LEN; i++) {
			p_dma_buffer_TX[i] = fixed_point_div(input_data[i], FLOAT_2_FIXED(255));
		}
		pool_0_start(&cnn_hw.pool_0_hw);
		conv_0_start(&cnn_hw.conv_0_hw);

		Xil_DCacheFlushRange((u32) p_dma_buffer_TX, INPUT_DATA_LEN * sizeof(u32));
		Xil_DCacheFlushRange((u32) p_dma_buffer_RX, OUTPUT_DATA_LEN * sizeof(u32));

		printf("Sending data to IP core slave\n");
		XAxiDma_SimpleTransfer(&cnn_hw.axi_dma, (u32) p_dma_buffer_TX, INPUT_DATA_LEN * sizeof(u32), XAXIDMA_DMA_TO_DEVICE);

		printf("Receive data from IP core\n");
		XAxiDma_SimpleTransfer(&cnn_hw.axi_dma, (u32) p_dma_buffer_RX, OUTPUT_DATA_LEN * sizeof(u32), XAXIDMA_DEVICE_TO_DMA);
		while (XAxiDma_Busy(&cnn_hw.axi_dma, XAXIDMA_DEVICE_TO_DMA))
			/* wait */ ;

		Xil_DCacheInvalidateRange((u32) p_dma_buffer_RX, OUTPUT_DATA_LEN * sizeof(u32));

		while (!pool_0_is_done(&cnn_hw.pool_0_hw))
			/* wait */ ;
		printf("Calculation complete\n");

		for (int i = 0; i < OUTPUT_DATA_LEN; i++) {
			printf("Recv[%d] ", i);
			fixed_point_print(p_dma_buffer_RX[i]);
		}
	} while (0);
	/*******************************************************/




	// cnn sw init

	// simulation init

	// cnn hw set

	// dbg init

	// print menu
	// main loop with user options:
	// 0. exit
	// 1. run HW and get results
	// 2. run SW and get results
	// 3. run programmed simulation
	// 4. ?

	// free resources

	return 0;
}