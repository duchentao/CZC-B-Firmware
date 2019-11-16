/****************************************Copyright (c)**************************************************
**                               Guangzhou ZHIYUAN electronics Co.,LTD.
**                                     
**                                 http://www.zyinside.com
**
**--------------File Info-------------------------------------------------------------------------------
** File Name:          target.c
** Last modified Date: 2006-01-11 
** Last Version:       v2.0
** Descriptions:       目标板特殊的代码，包括异常处理程序和目标板初始化程序
**
**------------------------------------------------------------------------------------------------------
** Created By:         陈明计
** Created date:       2003-06-05
** Version:            v1.0
** Descriptions:       创建
**
**------------------------------------------------------------------------------------------------------
** Modified by:        甘达
** Modified date:      2006-01-06
** Version:            v2.0
** Descriptions:       升级注释
**
**------------------------------------------------------------------------------------------------------
** Modified by:      
** Modified date:     
** Version:           
** Descriptions:      
**
********************************************************************************************************/
#define IN_TARGET
#include "config.h"

/*********************************************************************************************************
** 函数名称: FIQ_Exception
** 功能描述: 快速中断异常处理程序，用户根据需要自己改变程序
**           
** 输　入: 无
**
** 输　出: 无
**         
** 全局变量: 无
** 调用模块: 无
**
** 作　者: 黄绍斌
** 日　期: 2005年11月11日
**-------------------------------------------------------------------------------------------------------
** 修改人: 
** 日　期: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
        void  FIQ_Exception(void)
{
    while(1);                   // 这一句替换为自己的代码
}
/*********************************************************************************************************
** 函数名称: Timer0_Exception
** 功能描述: 定时器0中断服务程序
** 输　入: 无
**
** 输　出: 无
**         
** 全局变量: 无
** 调用模块: OSTimeTick
**
** 作　者: 陈明计
** 日　期: 2004年2月2日
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
        void Timer0_Exception(void)
{
	rSRCPND = 1 << 10;
	rINTPND = rINTPND;
    OSTimeTick();
}

/*********************************************************************************************************
** 函数名称: Timer0Init
** 功能描述: 定时器0初始化
** 输　入: 无
**
** 输　出: 无
**         
** 全局变量: 无
** 调用模块: 无
**
** 作　者: 陈明计
** 日　期: 2002年4月4日
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
        void Timer0Init(void)
{
	// 定时器设置
    rTCFG0 = 0;	                        // 预分频器0设置为250，取得200KHz
    rTCFG1 = 1;					        // TIMER0再取1/4分频，取得50KHz
    rTCNTB0 = (PCLK / (4 * OS_TICKS_PER_SEC)) - 1;
    rTCON = (1 << 1);                   // 更新定时器数据		
    rTCON = (1 << 0) | (1 << 3);        // 启动定时器
 }

/*********************************************************************************************************
** 函数名称: VICInit
** 功能描述: 向量中断控制器初始化
** 输　入: 无
**
** 输　出: 无
**         
** 全局变量: 无
** 调用模块: 无
**
** 作　者: 陈明计
** 日　期: 2004年2月2日
**-------------------------------------------------------------------------------------------------------
** 修改人: 
** 日　期: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
        void VICInit(void)
{
	// 设置中断控制器
	rPRIORITY = 0x00000000;		// 使用默认的固定的优先级
	rINTMOD = 0x00000000;		// 所有中断均为IRQ中断
	VICVectAddr[10] = (uint32) Timer0_Exception;
	rINTMSK = ~(1<<10);			// 打开TIMER0中断允许
 //**********************************************************
// Modbus使用的中断配置	
 	VICVectAddr[12] = (uint32) T15_Exception;
 	rINTMSK &= ~(1<<12);			// 打开TIMER2中断允许
 	VICVectAddr[13] = (uint32) T35_Exception;
 	rINTMSK &= ~(1<<13);			// 打开TIMER3中断允许


 	VICVectAddr[23] = (uint32) UART1_Exception;
    // Clear Int Pending and Unmask 
    rSUBSRCPND =(BIT_SUB_RXD1|BIT_SUB_ERR1);
    rINTSUBMSK &=~(BIT_SUB_RXD1|BIT_SUB_ERR1);
    ClearPending(BIT_UART1);
    rINTMSK &=~(BIT_UART1);
 //**********************************************************

 }


// 声明外部图片资源
#if  DISP_BGPIC!=0
extern uint16 gImage_bliss[];			
#endif
/*********************************************************************************************************
** 函数名称: TargetInit
** 功能描述: 目标板初始化代码，在需要的地方调用，根据需要改变
** 输　入: 无
**
** 输　出: 无
**         
** 全局变量: 无
** 调用模块: 无
**
** 作　者: 陈明计
** 日　期: 2004年2月2日
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
        void TargetInit(void)
{
    uint32 temp;

    OS_ENTER_CRITICAL();
    srand((uint32) TargetInit);
    VICInit();
    Timer0Init();

    IniUART1(115200);
    TimersInit();
    MBSlaveIni(&SlaveDevice);  			


    rRTCCON = 0x00;                             // 禁止RTC接口(只允许RTC计数和读数)	
    temp = rBCDYEAR & 0xff;
    if(temp > 0x99)                             // 年判断
    {
        goto ChangeRTC;
    }
    if (temp == 0)
    {
        goto ChangeRTC;
    }
    temp = rBCDMON & 0x1F;
    if (temp > 0x12)            // 月判断		
    {
        goto ChangeRTC;
    }
    if (temp == 0)
    {
        goto ChangeRTC;
    }
    temp = rBCDDATE & 0x3F;
    if (temp > 0x31)            // 日判断		
    {
        goto ChangeRTC;
    }
    if (temp == 0)
    {
        goto ChangeRTC;
    }
    temp = rBCDHOUR & 0x3F;
    if (temp > 0x24)            // 时判断		
    {
        goto ChangeRTC;
    }
    temp = rBCDMIN & 0x7F;
    if (temp > 0x59)            // 分判断		
    {
        goto ChangeRTC;
    }
    temp = rBCDSEC & 0x7F;
    if (temp > 0x59)            // 秒判断
    {
        goto ChangeRTC;
    }

    OS_EXIT_CRITICAL();

    return;
    
ChangeRTC:	
	rRTCCON = 0x01;             // 标准模式，RTC接口使能(允许写)
	
   	rBCDYEAR = 0x05;
    rBCDMON  = 0x11;
    rBCDDATE = 0x28;         
    rBCDDAY  = 0x01;            // 星期，MON:1  TUE:2  WED:3  THU:4  FRI:5  SAT:6  SUN:7
    rBCDHOUR = 0x17;
    rBCDMIN  = 0x01;
    rBCDSEC  = 0x00;
    
    rRTCCON  = 0x00;            // 禁止RTC接口(只允许RTC计数和读数)	
    
}

/*********************************************************************************************************
**                  以下为一些与系统相关的库函数的实现
**                  具体作用请ads的参考编译器与库函数手册
**                  用户可以根据自己的要求修改        
********************************************************************************************************/
/*********************************************************************************************************
**                  The implementations for some library functions
**                  For more details, please refer to the ADS compiler handbook and The library 
**                  function manual
**                  User could change it as needed       
********************************************************************************************************/

#include "rt_sys.h"
#include "stdio.h"

#pragma import(__use_no_semihosting_swi)

        int __rt_div0(int a)
{
    a = a;
    return 0;
}

        int fputc(int ch,FILE *f)
{
    ch = ch;
    f = f;
    return 0;
}

    int fgetc(FILE *f)
{
    f = f;
    return 0;
}


        int _sys_close(FILEHANDLE fh)
{
    fh = fh;
    return 0;
}

        int _sys_write(FILEHANDLE fh, const unsigned char * buf,
                      unsigned len, int mode)
{
    fh = fh;
    buf = buf;
    len =len;
    mode = mode;
    return 0;
}
        int _sys_read(FILEHANDLE fh, unsigned char * buf,
                     unsigned len, int mode)
{
    fh = fh;
    buf = buf;
    len =len;
    mode = mode;
    
    return 0;
}

       void _ttywrch(int ch)
{
    ch = ch;
}

        int _sys_istty(FILEHANDLE fh)
{
    fh = fh;
    return 0;
}
        int _sys_seek(FILEHANDLE fh, long pos)
{
    fh = fh;
    pos = pos;
    return 0;
}
        int _sys_ensure(FILEHANDLE fh)
{
    fh = fh;
    return 0;
}

        long _sys_flen(FILEHANDLE fh)
{
    fh = fh;
    return 0;
}
       int _sys_tmpnam(char * name, int sig, unsigned maxlen)
{
    name = name;
    sig = sig;
    maxlen = maxlen;
    return 0;
}


        void _sys_exit(int returncode)
{
    returncode = returncode;
}

        char *_sys_command_string(char * cmd, int len)
{
    cmd = cmd;
    len = len;
    return 0;
}


/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
