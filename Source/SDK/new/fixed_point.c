/*
 * fixed_point.c
 *
 *  Created on: Apr 3, 2021
 *      Author: cgrud
 */


/*
 * INCLUDES
 ******************************************************************************
 */

#include "fixed_point.h"
#include <stdint.h>
#include <stdio.h>

/*
 * FUNCTIONS
 ******************************************************************************
 */

void fixed_point_print(ufixp32_t num)
{
	if (num & SIGN_BIT) {
		printf("-%.6f ", FIXED_2_FLOAT(-num));
	} else {
		printf("%.6f ", FIXED_2_FLOAT(num));
	}
}

ufixp32_t fixed_point_mul(ufixp32_t a, ufixp32_t b)
{
	ufixp32_t result = 0;
	int sign = !SIGN_EQ(a, b);

	a = (a & SIGN_BIT) ? -a : a;
	b = (b & SIGN_BIT) ? -b : b;

	result = ((a * b) >> FIXP_FRACTION_WIDTH);

	return sign ? -result : result;
}

ufixp32_t fixed_point_div(ufixp32_t a, ufixp32_t b)
{
	ufixp32_t result = 0;
	int sign = !SIGN_EQ(a, b);

	a = (a & SIGN_BIT) ? -a : a;
	b = (b & SIGN_BIT) ? -b : b;

	result = ((a << FIXP_FRACTION_WIDTH) / b);

	return sign ? -result : result;
}