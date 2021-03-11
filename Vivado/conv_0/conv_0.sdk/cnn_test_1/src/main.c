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

#include "xmy_filter_buffer.h"
#include "xaxidma.h"
#include "xtime_l.h"

#include <stdio.h>
#include <xparameters.h>
#include <stdint.h>

/*
 * DEFINES
 ******************************************************************************
 */

#define MEM_BASE_ADDR 0x01000000
#define TX_BUFFER_BASE (MEM_BASE_ADDR+0x00100000)
#define RX_BUFFER_BASE (MEM_BASE_ADDR+0x00300000)
#define SIZE_ARR 10000
#define IMAGE_SIZE 16

/*
 * GLOBAL VARS
 ******************************************************************************
 */

XMy_filter_buffer my_filter_buffer;
XMy_filter_buffer_Config *my_filter_buffer_cfg;
XAxiDma axiDMA;
XAxiDma_Config *axiDMA_cfg;

int PS_result[SIZE_ARR];

/*
 * FUNCTIONS
 ******************************************************************************
 */

void initPeripherals() {
	printf("initializing my_filter_buffer\n");
	my_filter_buffer_cfg = XMy_filter_buffer_LookupConfig(XPAR_MY_FILTER_BUFFER_0_DEVICE_ID);
	if (my_filter_buffer_cfg) {
		int status = XMy_filter_buffer_CfgInitialize(&my_filter_buffer, my_filter_buffer_cfg);
		if (status != XST_SUCCESS)
			printf("Error initializing doGain core\n");
	}

	printf("initializing AxiDMA\n");
	axiDMA_cfg = XAxiDma_LookupConfig(XPAR_AXIDMA_0_DEVICE_ID);
	if (axiDMA_cfg) {
		int status = XAxiDma_CfgInitialize(&axiDMA, axiDMA_cfg);
		if (status != XST_SUCCESS)
			printf("Error initializing AxiDMA core\n");
	}

	XAxiDma_IntrDisable(&axiDMA, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DEVICE_TO_DMA);
	XAxiDma_IntrDisable(&axiDMA, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DMA_TO_DEVICE);
}

void kernel_hw_set(uint32_t *kernel)
{
	XMy_filter_buffer_Set_kernel_0(&my_filter_buffer, kernel[0]);
	XMy_filter_buffer_Set_kernel_1(&my_filter_buffer, kernel[1]);
	XMy_filter_buffer_Set_kernel_2(&my_filter_buffer, kernel[2]);
	XMy_filter_buffer_Set_kernel_3(&my_filter_buffer, kernel[3]);
	XMy_filter_buffer_Set_kernel_4(&my_filter_buffer, kernel[4]);
	XMy_filter_buffer_Set_kernel_5(&my_filter_buffer, kernel[5]);
	XMy_filter_buffer_Set_kernel_6(&my_filter_buffer, kernel[6]);
	XMy_filter_buffer_Set_kernel_7(&my_filter_buffer, kernel[7]);
	XMy_filter_buffer_Set_kernel_8(&my_filter_buffer, kernel[8]);
}

void kernel_hw_get(uint32_t *kernel)
{
	kernel[0] = XMy_filter_buffer_Get_kernel_0(&my_filter_buffer);
	kernel[1] = XMy_filter_buffer_Get_kernel_1(&my_filter_buffer);
	kernel[2] = XMy_filter_buffer_Get_kernel_2(&my_filter_buffer);
	kernel[3] = XMy_filter_buffer_Get_kernel_3(&my_filter_buffer);
	kernel[4] = XMy_filter_buffer_Get_kernel_4(&my_filter_buffer);
	kernel[5] = XMy_filter_buffer_Get_kernel_5(&my_filter_buffer);
	kernel[6] = XMy_filter_buffer_Get_kernel_6(&my_filter_buffer);
	kernel[7] = XMy_filter_buffer_Get_kernel_7(&my_filter_buffer);
	kernel[8] = XMy_filter_buffer_Get_kernel_8(&my_filter_buffer);
}

void bias_hw_set(uint32_t *bias)
{
	XMy_filter_buffer_Set_bias_0(&my_filter_buffer, bias[0]);
	XMy_filter_buffer_Set_bias_1(&my_filter_buffer, bias[1]);
	XMy_filter_buffer_Set_bias_2(&my_filter_buffer, bias[2]);
	XMy_filter_buffer_Set_bias_3(&my_filter_buffer, bias[3]);
	XMy_filter_buffer_Set_bias_4(&my_filter_buffer, bias[4]);
	XMy_filter_buffer_Set_bias_5(&my_filter_buffer, bias[5]);
	XMy_filter_buffer_Set_bias_6(&my_filter_buffer, bias[6]);
	XMy_filter_buffer_Set_bias_7(&my_filter_buffer, bias[7]);
	XMy_filter_buffer_Set_bias_8(&my_filter_buffer, bias[8]);
}

void bias_hw_get(uint32_t *bias)
{
	bias[0] = XMy_filter_buffer_Get_bias_0(&my_filter_buffer);
	bias[1] = XMy_filter_buffer_Get_bias_1(&my_filter_buffer);
	bias[2] = XMy_filter_buffer_Get_bias_2(&my_filter_buffer);
	bias[3] = XMy_filter_buffer_Get_bias_3(&my_filter_buffer);
	bias[4] = XMy_filter_buffer_Get_bias_4(&my_filter_buffer);
	bias[5] = XMy_filter_buffer_Get_bias_5(&my_filter_buffer);
	bias[6] = XMy_filter_buffer_Get_bias_6(&my_filter_buffer);
	bias[7] = XMy_filter_buffer_Get_bias_7(&my_filter_buffer);
	bias[8] = XMy_filter_buffer_Get_bias_8(&my_filter_buffer);
}

int main()
{
	//int *m_dma_buffer_TX = (int*) TX_BUFFER_BASE;
	int *m_dma_buffer_RX = (int*) RX_BUFFER_BASE;

	// timing vars
	XTime tStartPL, tEndPL;
	//XTime tStartPS, tEndPS;

	initPeripherals();

	uint32_t image_input[IMAGE_SIZE] = {0  ,  1  ,  2  ,  3  ,
										4  ,  5  ,  6  ,  7  ,
										8  ,  9  ,  0xa,  0xb,
										0xc,  0xd,  0xe,  0xf };

	//uint32_t output[4] = {0};

	uint32_t kernel[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
	uint32_t bias[9] = {0};

	XMy_filter_buffer_Set_ctrl(&my_filter_buffer, 0);
	kernel_hw_set(kernel);
	bias_hw_set(bias);

	XMy_filter_buffer_Start(&my_filter_buffer);

	Xil_DCacheFlushRange((u32) image_input, SIZE_ARR * sizeof(int));
	Xil_DCacheFlushRange((u32) m_dma_buffer_RX, SIZE_ARR * sizeof(int));

	do {

		Xil_DCacheFlushRange((u32) image_input, IMAGE_SIZE * sizeof(int));
		Xil_DCacheFlushRange((u32) m_dma_buffer_RX, IMAGE_SIZE * sizeof(int));

		printf("Sending data to IP core slave\n");
		XTime_GetTime(&tStartPL);
		XAxiDma_SimpleTransfer(&axiDMA, (u32) image_input, 16 * sizeof(int), XAXIDMA_DMA_TO_DEVICE);
		//printf("Receive data from IP core\n");
		XAxiDma_SimpleTransfer(&axiDMA, (u32) m_dma_buffer_RX, 4 * sizeof(int), XAXIDMA_DEVICE_TO_DMA);
		while (XAxiDma_Busy(&axiDMA, XAXIDMA_DEVICE_TO_DMA));
		Xil_DCacheInvalidateRange((u32) m_dma_buffer_RX, 4 * sizeof(int));
		while (!XMy_filter_buffer_IsDone(&my_filter_buffer));
		XTime_GetTime(&tEndPL);

		printf("OutputPL took %llu clock cycles. \n", 2*(tEndPL-tStartPL));

		//printf("Calculation complete\n");
		//XTime_GetTime(&tStartPS);
		for (int idx = 0; idx < 4; idx++) {
		//	PS_result[idx] = idx * gain;
			printf("result[%d]=%d\n", idx, m_dma_buffer_RX[idx]);
		}
		//XTime_GetTime(&tEndPS);

		//printf("OutputPS took %llu clock cycles. \n", 2*(tEndPS-tStartPS));

	} while (0);

	printf("DONE!\n");

	return 0;
}

