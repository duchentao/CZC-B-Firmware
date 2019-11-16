/*
 * @Descripttion: 
 * @version: 
 * @Author: 
 * @Date: 2019-07-08 23:06:56
 * @LastEditors: 
 * @LastEditTime: 2019-09-06 11:12:01
 */
#include "IO.h"

void IO_init(void)
{
	uint8 i = 0;
	
	//管脚设置为GPIO功能
	//可能不需要设置，因为复位值的PINSEL基本都是默认GPIO口，并且PINMODE都是上拉				
	//设置输出管脚方向
	//IODIR
	L1_IO_CON->FIODIR |= L1_PIN;			    
	L2_IO_CON->FIODIR |= L2_PIN;				    
	L3_IO_CON->FIODIR |= L3_PIN;				        
	                                                    
	CON_485_1_IO_CON->FIODIR |= CON_485_1_PIN;	    	
	CON_485_2_IO_CON->FIODIR |= CON_485_2_PIN;		    	
                                          	
	KA1_IO_CON->FIODIR |=  KA1_PIN;  	
	KA2_IO_CON->FIODIR |=  KA2_PIN; 	
	KA3_IO_CON->FIODIR |=  KA3_PIN; 
	KA4_IO_CON->FIODIR |=  KA4_PIN;  	
	KA5_IO_CON->FIODIR |=  KA5_PIN; 	
	KA6_IO_CON->FIODIR |=  KA6_PIN;	
	KA7_IO_CON->FIODIR |=  KA7_PIN;  	
	KA8_IO_CON->FIODIR |=  KA8_PIN; 	
	KA9_IO_CON->FIODIR |=  KA9_PIN;	
	KA10_IO_CON->FIODIR |=  KA10_PIN;  	
	KA11_IO_CON->FIODIR |=  KA11_PIN; 	
	KA12_IO_CON->FIODIR |=  KA12_PIN;	
	KA13_IO_CON->FIODIR |=  KA13_PIN;  	
	  	
    for (i = 1; i <= 18; i++)
    {
		IO_OutputCtrl(i, OFF);
    }
}
void IO_OutputCtrl(uint8 device_num, uint8 state) //输出控制，参数device_num表示设备名称，state表示设备的开或关
{                                               //.eg --- Out_Control(MOVE_LED_PIN, ON) 表示把开始登陆的灯点亮                                               
    switch(device_num)
    {
			case L1:
				if(state == ON)  L1_IO_CON->FIOCLR = L1_PIN;
				else             L1_IO_CON->FIOSET = L1_PIN; 
			break;	
			
			case L2:
				if(state == ON)  L2_IO_CON->FIOCLR = L2_PIN;
				else             L2_IO_CON->FIOSET = L2_PIN; 
			break;	
			
			case L3:
				if(state == ON)  L3_IO_CON->FIOCLR = L3_PIN;
				else             L3_IO_CON->FIOSET = L3_PIN; 
			break;	
			
			
			case KA1:
				if(state == ON)  KA12_IO_CON->FIOCLR = KA12_PIN;
				else             KA12_IO_CON->FIOSET = KA12_PIN; 
			break;	
			case KA2:
				if(state == ON)  KA11_IO_CON->FIOCLR = KA11_PIN;
				else             KA11_IO_CON->FIOSET = KA11_PIN; 
			break;	
			case KA3:
				if(state == ON)  KA10_IO_CON->FIOCLR = KA10_PIN;
				else             KA10_IO_CON->FIOSET = KA10_PIN; 
			break;	
			case KA4:
				if(state == ON)  KA9_IO_CON->FIOCLR = KA9_PIN;
				else             KA9_IO_CON->FIOSET = KA9_PIN; 
			break;	

			case KA5:
				if(state == ON)  KA2_IO_CON->FIOCLR = KA2_PIN;
				else             KA2_IO_CON->FIOSET = KA2_PIN; 
			break;	
			
			case KA6:
				if(state == ON)  KA3_IO_CON->FIOCLR = KA3_PIN;
				else             KA3_IO_CON->FIOSET = KA3_PIN; 
			break;	
			
			case KA7:
				if(state == ON)  KA4_IO_CON->FIOCLR = KA4_PIN;
				else             KA4_IO_CON->FIOSET = KA4_PIN; 
			break;	
			
			case KA8:
				if(state == ON)  KA5_IO_CON->FIOCLR = KA5_PIN;
				else             KA5_IO_CON->FIOSET = KA5_PIN; 
			break;	
			
			case KA9:
				if(state == ON)  KA6_IO_CON->FIOCLR = KA6_PIN;
				else             KA6_IO_CON->FIOSET = KA6_PIN; 
			break;	
			
			case KA10:
				if(state == ON)  KA7_IO_CON->FIOCLR = KA7_PIN;
				else             KA7_IO_CON->FIOSET = KA7_PIN; 
			break;	

			case KA11:
				if(state == ON)  KA8_IO_CON->FIOCLR = KA8_PIN;
				else             KA8_IO_CON->FIOSET = KA8_PIN; 
			break;	
			case BEAK_UP_OUT:
				if(state == ON)  KA13_IO_CON->FIOCLR = KA13_PIN;
				else             KA13_IO_CON->FIOSET = KA13_PIN; 
			break;	
			case BEEP:
				if(state == ON)  KA1_IO_CON->FIOCLR = KA1_PIN;
				else             KA1_IO_CON->FIOSET = KA1_PIN; 
			break;	


			case CON_485_1:
				if(state == ON)  CON_485_1_IO_CON->FIOCLR = CON_485_1_PIN;
				else             CON_485_1_IO_CON->FIOSET = CON_485_1_PIN; 
			break;	
			
			case CON_485_2:
				if(state == ON)  CON_485_2_IO_CON->FIOCLR = CON_485_2_PIN;
				else             CON_485_2_IO_CON->FIOSET = CON_485_2_PIN; 
			break;	
			
			default:
			break;  
    }    
}

uint32 ReadInput(void)                             
{
	uint32 state = 0;
																		
	if(!(IN12_IO_CON->FIOPIN & IN12_PIN))	    state |= (1<<0);	//后4	
	if(!(IN11_IO_CON->FIOPIN & IN11_PIN))	    state |= (1<<1);	//后3	
	if(!(IN10_IO_CON->FIOPIN & IN10_PIN))	    state |= (1<<2);	//后2	
	if(!(IN9_IO_CON->FIOPIN & IN9_PIN))	    	state |= (1<<3);	//后1	
	if(!(IN8_IO_CON->FIOPIN & IN8_PIN))	    	state |= (1<<4);	//完车	
	if(!(IN7_IO_CON->FIOPIN & IN7_PIN))	    	state |= (1<<5);  	//停车	
	if(!(IN6_IO_CON->FIOPIN & IN6_PIN))	    	state |= (1<<6);	//备车	
	if(!(IN5_IO_CON->FIOPIN & IN5_PIN))	    	state |= (1<<7);  	//前1 	
	if(!(IN4_IO_CON->FIOPIN & IN4_PIN))	    	state |= (1<<8);  	//前2	
	if(!(IN3_IO_CON->FIOPIN & IN3_PIN))	    	state |= (1<<9);	//前3	
	if(!(IN2_IO_CON->FIOPIN & IN2_PIN))	    	state |= (1<<10);	//前4	
	if(!(IN13_IO_CON->FIOPIN & IN13_PIN))	    state |= (1<<11);   //备用     
	//	if(!(IN1_IO_CON->FIOPIN & IN1_PIN))	    	state |= (1<<11); 		//消音
	return state;
}

uint32 IO_InputScan(void)
{
	uint8 j = 0;
	static uint8 cnt_p[IO_OUTPUT_NUM] = {0};
	static uint8 cnt_n[IO_OUTPUT_NUM] = {0};
	
	uint32 flag = 0;
	uint32 IO_STATE1 = 0;
	IO_STATE1 = ReadInput();
	for(j=0;j<13;j++)
	{
		if(IO_STATE1 & (0x0001 << j))
		{
			cnt_p[j]++;
			if(cnt_p[j] >= 5)
			{
				cnt_p[j]=5;
				cnt_n[j]=0;
				flag |= (0x0001<<j);
			}
		}
		else
		{
			cnt_n[j]++;
			if(cnt_n[j] >= 5)
			{
				cnt_n[j]=5;
				cnt_p[j]=0;
				flag &= ~(0x0001<<j);
			}
		}
		
	}
	return flag;
}


uint8 mute_flag;
uint8 IO_ReadMute(void)
{
	static uint8 cnt_p = 0;
	static uint8 flag = 0;
	if(READ_MUTE_PIN)
	{
		cnt_p++;
		if(cnt_p >= 5)
		{
			cnt_p=5;
			flag = 1;
		}
	}
	else if(flag == 1)
	{
		cnt_p = 0;
		flag = 0;
		mute_flag = 1;
	}
	
	return mute_flag;
}

uint8 IO_ReadSwitch(void)
{
	static int8 cnt[4] = {0};
	uint8 flag;
	uint8 temp=0;

	if((D0_IO_CON->FIOPIN & D0_PIN))	    temp |= (1<<0);  
	if((D1_IO_CON->FIOPIN & D1_PIN))	    temp |= (1<<1);  
	if((D2_IO_CON->FIOPIN & D2_PIN))	    temp |= (1<<2);  
	if((D3_IO_CON->FIOPIN & D3_PIN))	    temp |= (1<<3);

	return temp;
}

