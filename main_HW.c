#include <nds32_intrinsic.h>
#include "WT58F2C9.h"
#include "gpio.h"

void delay1(unsigned int nCount)
{
	volatile unsigned int i;
	for (i = 0; i < nCount; i++)
		;
}

extern void DRV_Printf(char *pFmt, U16 u16Val);

inline void GIE_ENABLE();

void DRV_EnableHWInt(void)
{
	/* enable SW0, HW0 and HW1 */
	__nds32__mtsr(0x10003, NDS32_SR_INT_MASK);
	/* Enable SW0 */
	//__nds32__mtsr(0x10000, NDS32_SR_INT_MASK);
	/* Enable global interrupt */
	GIE_ENABLE();
}

void DRV_BlockIntDisable(void)
{
	// Disable all interrupt
	OUTW(rINT0_IE0_ENABLE, 0x0000);
	OUTW(rINT0_IE1_ENABLE, 0x0000);
	OUTW(rINT0_IE2_ENABLE, 0x0000);
	OUTW(rINT1_IE0_ENABLE, 0x0000);
	OUTW(rINT1_IE1_ENABLE, 0x0000);
	OUTW(rINT1_IE2_ENABLE, 0x0000);
}

void DRV_IntInitial(void)
{
	// Disable all interrupt
	DRV_BlockIntDisable();

	// Enable all HW interrupt
	DRV_EnableHWInt(); // Enable global Hardware interrupt, Assembly command

	// Enable default Block interrupt
	// DRV_BlockIntEnable();			//Enable each block device interrupt
}

#define rSYS_OPTION1 (0x00200004)

void DRV_SysXtal(U8 u8XtalMode)
{

#if (EXTERNAL_XTAL == XTAL_MODE)
	//-----External Crystal
	//-----24MHz
	OUTW(rSYS_OPTION1, ((INW(rSYS_OPTION1) & 0xFFFFFF00) | 0x0012)); // Use HXTAL and divide 2
																	 //-----Crystal 12MHz
																	 // OUTW(rSYS_OPTION1, (INW(rSYS_OPTION1) | 0x000A));

	// OUTW(rSYS_OPTION1,(INW(rSYS_OPTION1) | (XTAL<<2) | (HSE_OSC_ON <<1)));
	// OUTW(rSYS_CLOCK_SELECT,(INW(rSYS_CLOCK_SELECT) | 0x0001));
#else
	//-----Internal RC
	//-----24MHz
	// OUTW(rSYS_OPTION1,(INW(rSYS_OPTION1) | 0x0012));
	//-----Crystal 12MHz
	// OUTW(rSYS_OPTION1, (INW(rSYS_OPTION1) | 0x000A));

	// OUTW(rSYS_OPTION1,(INW(rSYS_OPTION1) | (XTAL<<2)));
	// OUTW(rSYS_CLOCK_SELECT,(INW(rSYS_CLOCK_SELECT));
#endif
	//-----MCU Clock Output Test
	// OUTW(rSYS_OPTION3,0x0090);
}

void OS_PowerOnDriverInitial(void)
{
	//========================= << Typedef Initial  >>
	// SYS_TypeDefInitial();
	//========================= << System Clock Initial >>
	//-----External Crystal
	DRV_SysXtal(EXTERNAL_XTAL);
	//========================= << Interrupt Initial >>
	DRV_IntInitial();
	//========================= << GPIO Initial >>
	// DRV_GpioInitial();
	//========================= << UART Initial >>
	// DRV_UartInitial();
	//========================= << Timer Initial >>
	// DRV_TimerInitial(TIMER_0, SIMPLE_TIMER);
	// DRV_TimerInitial(TIMER_1, SIMPLE_TIMER);
	//========================= << PWM Initial >>
	// DRV_PwmInitial();
	//========================= << SPI Initial >>
	// DRV_SpiInitial(SPI_CH2);
	//========================= << Watchdog Initial >>
}

void ps(int the)
{
	int arr[8] = {0};
	int i = 0;
	while (the != 0)
	{
		int temp;
		temp = the % 10;
		arr[i] = temp;
		i++;
		the /= 10;
	}
	unsigned int index_7LED_NUM[17] = {Number_0, Number_1, Number_2, Number_3, Number_4, Number_5, Number_6, Number_7, Number_8, Number_9, Number_A, Number_b, Number_C, Number_d, Number_E, Number_F, Number_Dot};
	for (i = 0; i < 100; i++)
	{
		switch (i)
		{
		case 8:
			GPIO_PTA_GPIO = Digit_8;
			GPIO_PTD_GPIO = index_7LED_NUM[arr[7]];
			delay1(5000);
		case 7:
			GPIO_PTA_GPIO = Digit_7;
			GPIO_PTD_GPIO = index_7LED_NUM[arr[6]];
			delay1(5000);
		case 6:
			GPIO_PTA_GPIO = Digit_6;
			GPIO_PTD_GPIO = index_7LED_NUM[arr[5]];
			delay1(5000);
		case 5:
			GPIO_PTA_GPIO = Digit_5;
			GPIO_PTD_GPIO = index_7LED_NUM[arr[4]];
			delay1(5000);
		case 4:
			GPIO_PTA_GPIO = Digit_4;
			GPIO_PTD_GPIO = index_7LED_NUM[arr[3]];
			delay1(5000);
		case 3:
			GPIO_PTA_GPIO = Digit_3;
			GPIO_PTD_GPIO = index_7LED_NUM[arr[2]];
			delay1(5000);
		case 2:
			GPIO_PTA_GPIO = Digit_2;
			GPIO_PTD_GPIO = index_7LED_NUM[arr[1]];
			delay1(5000);
		case 1:
			GPIO_PTA_GPIO = Digit_1;
			GPIO_PTD_GPIO = index_7LED_NUM[arr[0]];
			delay1(5000);
		}
	}
}

int keyinin(int a)
{
	const int inputMatrix[4][4] = {{0x11, 0x12, 0x13, 0x14},
								   {0x21, 0x22, 0x24, 0x28},
								   {0x41, 0x42, 0x44, 0x48},
								   {0x81, 0x82, 0x84, 0x88}};
	const int outputMatrix[4][4] = {{3, 7, 11, 15},
									{2, 6, 10, 14},
									{1, 5, 9, 13},
									{0, 4, 8, 12}};
	int i, j, output;
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			if (a == intputMatrix[j][i])
			{
				output = outputMatrix[j][i];
				break;
			}
		}
	}
	return output;
}

int main()
{
	unsigned int tmp = 0;
	unsigned int ntmp = 0;

	OS_PowerOnDriverInitial();

	DRV_Printf("==========================================\r\n", 0);
	DRV_Printf("   ADP-WT58F2C9 Key Matrix demo program   \r\n", 0);
	DRV_Printf("==========================================\r\n", 0);

	DRV_Printf("Key Matrix testing...\r\n", 0);
	DRV_Printf("Press SW17 then SW16 to EXIT.\r\n", 0);

	GPIO_PTA_FS = 0x0000;
	GPIO_PTA_PADINSEL = 0x0000;

	// Setting for 7LED select
	GPIO_PTA_DIR = 0x0000;
	GPIO_PTA_CFG = 0x0000;
	GPIO_PTA_GPIO = Digit_8;
	// Setting for 7LED number
	GPIO_PTD_DIR = 0x0000;
	GPIO_PTD_CFG = 0x0000;
	GPIO_PTD_GPIO = Number_8 | Number_Dot;

	unsigned int col;
	unsigned int key;
	unsigned int index_7LED_NUM[17] = {Number_0, Number_1, Number_2, Number_3, Number_4, Number_5, Number_6, Number_7,
									   Number_8, Number_9, Number_A, Number_b, Number_C, Number_d, Number_E, Number_F, Number_Dot};
	unsigned int ready_exit = 0;

	unsigned int num1 = 0, num2 = 0, oper = 0;
	unsigned int flag = 0;
	unsigned int keyboard = 0;

	while (1)
	{
		key = 0xFF;
		GPIO_PTA_DIR = 0x0FF0;
		GPIO_PTA_CFG = 0x0000;

		for (col = 0; col < 4; col++)
		{
			GPIO_PTA_BS = 0x000F;
			GPIO_PTA_BR = 0x0000 | (1 << col);
			tmp = ((~GPIO_PTA_PADIN) & 0xFF0) >> 4;
			ntmp = (~GPIO_PTA_PADIN) & 0xFF;
			if (ntmp > 0)
			{
				keyboard = keyinin(ntmp);
				if (flag == 0)
				{
					if (keyboard < 10)
					{
						num1 = num1 * 10 + keyboard;
						ps(num1);
					}
					else
					{
						switch (keyboard)
						{
						case 10: // =
							ps(num1);
							num1 = 0;
							break;
						case 11: // AC
							ps(0);
							num1 = 0;
							num2 = 0;
							flag = 0;
							break;
						case 12: // /
							flag = 1;
							oper = keyboard;
							break;
						case 13: // *
							flag = 1;
							oper = keyboard;
							break;
						case 14: // -
							flag = 1;
							oper = keyboard;
							break;
						case 15: // +
							flag = 1;
							oper = keyboard;
							break;
						default:
							break;
						}
					}
				}
				else if (flag == 1)
				{
					if (keyboard < 10)
					{
						num2 = num2 * 10 + keyboard;
						ps(num2);
					}
					else
					{
						switch (keyboard)
						{
						case 11: // AC
							ps(0);
							num1 = 0;
							num2 = 0;
							flag = 0;
							break;
						case 10:
						case 12:
						case 13:
						case 14:
						case 15:
							switch (oper)
							{
							case 12: // /
								num1 = num1 / num2;
								num2 = 0;
								break;
							case 13: // *
								num1 = num1 * num2;
								num2 = 0;
								break;
							case 14: // -
								num1 = num1 - num2;
								num2 = 0;
								break;
							case 15: // +
								num1 = num1 + num2;
								num2 = 0;
								break;
							default:
								break;
							}
							ps(num1);
							switch (keyboard)
							{
							case 10:
								num1 = 0;
								flag = 0;
								break;
							case 12: // /
								flag = 1;
								oper = keyboard;
								break;
							case 13: // *
								flag = 1;
								oper = keyboard;
								break;
							case 14: // -
								flag = 1;
								oper = keyboard;
								break;
							case 15: // +
								flag = 1;
								oper = keyboard;
								break;
							default:
								break;
							}
						default:
							break;
						}
					}
				}
			}
		}
	}

	DRV_Printf("==========================================\r\n", 0);

	return 0;
}

//				if(ntmp==0x81)
//					num1 = 0;
//				else if(ntmp == 0x82)
//					num1 = 4;
//				else if(ntmp == 0x84)
//					num1 = 8;
//				else if(ntmp == 0x41)
//					num1 = 1;
//				else if(ntmp == 0x42)
//					num1 = 5;
//				else if(ntmp == 0x44)
//					num1 = 9;
//				else if(ntmp == 0x21)
//					num1 = 2;
//				else if(ntmp == 0x22)
//					num1 = 6;
//				else if(ntmp == 0x11)
//					num1 = 3;
//				else if(ntmp == 0x12)
//					num1 = 7;
//				else if (ntmp == 0x14)
//					goto head;
