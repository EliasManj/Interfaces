/*
 * RGB.h
 *
 *  Created on: Mar 30, 2019
 *      Author: Miguel Elias
 */

#ifndef RGB_H_
#define RGB_H_

#include "derivative.h" /* include peripheral declarations */

#define RGB_RED			0
#define RGB_BLUE 		1
#define RGB_GREEN		2
#define RGB_YELLOW		3
#define RGB_PURPLE		4
#define RGB_CYAN		5

void RGB_init(void);
void RGB(int r, int g, int b);
void RGB_Color(int color);

#endif /* RGB_H_ */
