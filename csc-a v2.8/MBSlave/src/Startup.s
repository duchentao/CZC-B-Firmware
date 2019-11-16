;/****************************************Copyright (c)**************************************************
;**                               Guangzhou ZHIYUAN electronics Co.,LTD.
;**                                     
;**                                 http://www.zyinside.com
;**
;**--------------File Info-------------------------------------------------------------------------------
;** File Name:          startup.s
;** Last modified Date: 2006-01-06 
;** Last Version:       v1.1
;** Descriptions:       S3C2410异常向量入口及异常向量与c语言代码的接口，包括初始化堆栈、初始化PLL的代码
;**
;**------------------------------------------------------------------------------------------------------
;** Created By:         黄绍斌
;** Created date:       2005-11-11
;** Version:            v1.0
;** Descriptions:       创建
;**
;**------------------------------------------------------------------------------------------------------
;** Modified by:        甘达
;** Modified date:      2006-01-06 
;** Version:            v1.1
;** Descriptions:       
;**
;**------------------------------------------------------------------------------------------------------
;** Modified by:      
;** Modified date:     
;** Version:           
;** Descriptions:      
;**
;********************************************************************************************************/
    IMPORT __use_no_semihosting_swi

; /* 定义堆栈的大小 */
; **** 用户可根据实际需要修改 ****
SVC_STACK_LEGTH     EQU         16
FIQ_STACK_LEGTH     EQU         16
IRQ_STACK_LEGTH     EQU         9*8
ABT_STACK_LEGTH     EQU         0
UND_STACK_LEGTH     EQU         0

; /*************************************************************************/
; /* CPSR寄存器的位域                                                      */
; /*************************************************************************/
; /*                                                                       */
; /* 31  30  29   28         7   6   5   4   3   2   1   0                 */
; /*+---+---+---+---+--ss--+---+---+---+---+---+---+---+---+               */
; /*| N | Z | C | V |      | I | F | T |     M4 ~ M0       |               */
; /*+---+---+---+---+--ss--+---+---+---+---+---+---+---+---+               */
; /*                                                                       */
; /* Processor Mode and Mask                                               */
; /*                                                                       */
; /*************************************************************************/
Mode_USR        EQU     0x10
Mode_FIQ        EQU     0x11
Mode_IRQ        EQU     0x12
Mode_SVC        EQU     0x13
Mode_ABT        EQU     0x17
Mode_UND        EQU     0x1B
Mode_SYS        EQU     0x1F 
I_BIT           EQU     0x80    ; when I bit is set (1), IRQ is disabled
F_BIT           EQU     0x40    ; when F bit is set (1), FIQ is disabled


; 总线宽度控制定义(0表示8位，1表示16位，2表示32位)
DW8                 EQU         (0x0)
DW16                EQU         (0x1)
DW32                EQU         (0x2)
WAIT                EQU         (0x1<<2)
UBLB                EQU         (0x1<<3)

; **** 用户可根据实际需要修改 ****
B7_BWCON            EQU         (DW16|WAIT|UBLB) 
B6_BWCON            EQU         (DW32|UBLB)  
B5_BWCON            EQU         (DW16|WAIT|UBLB)  
B4_BWCON            EQU         (DW16|WAIT|UBLB)  
B3_BWCON            EQU         (DW16|WAIT|UBLB)  
B2_BWCON            EQU         (DW16|WAIT|UBLB)  
B1_BWCON            EQU         (DW16|WAIT|UBLB)  


; CPU时钟设置(PLLCON控制值)
; 50.00MHz (外部晶振为12MHz时)
MDIV_50             EQU     0x5C
PDIV_50             EQU     0x4
SDIV_50             EQU     0x2

; 200.00MHz (外部晶振为12MHz时) 
; 设置值为：m=100,p=6,s=0, MPLL=FCLK=12*100/6=200MHz
MDIV_200            EQU     0x5C
PDIV_200            EQU     0x4
SDIV_200            EQU     0x0     
MPLLCON_200         EQU     ((MDIV_200 << 12) | (PDIV_200 << 4) | (SDIV_200)) 

; 寄存器定义
;=================
; WATCH DOG TIMER
;=================
WTCON           EQU     0x53000000      ;Watch-dog timer mode
WTDAT           EQU     0x53000004      ;Watch-dog timer data
WTCNT           EQU     0x53000008      ;Eatch-dog timer count

;=================
; INTERRUPT
;=================
SRCPND          EQU     0x4a000000      ;Interrupt request status
INTMOD          EQU     0x4a000004      ;Interrupt mode control
INTMSK          EQU     0x4a000008      ;Interrupt mask control
PRIORITY        EQU     0x4a00000c      ;IRQ priority control  
INTPND          EQU     0x4a000010      ;Interrupt request status
INTOFFSET       EQU     0x4a000014      ;Interruot request source offset
SUSSRCPND       EQU     0x4a000018      ;Sub source pending
INTSUBMSK       EQU     0x4a00001c      ;Interrupt sub mask

;=================
; Memory control 
;=================
BWSCON          EQU     0x48000000     ;Bus width & wait status
BANKCON0        EQU     0x48000004     ;Boot ROM control
BANKCON1        EQU     0x48000008     ;BANK1 control
BANKCON2        EQU     0x4800000c     ;BANK2 cControl
BANKCON3        EQU     0x48000010     ;BANK3 control
BANKCON4        EQU     0x48000014     ;BANK4 control
BANKCON5        EQU     0x48000018     ;BANK5 control
BANKCON6        EQU     0x4800001c     ;BANK6 control
BANKCON7        EQU     0x48000020     ;BANK7 control
REFRESH         EQU     0x48000024     ;DRAM/SDRAM refresh
BANKSIZE        EQU     0x48000028     ;Flexible Bank Size
MRSRB6          EQU     0x4800002c     ;Mode register set for SDRAM
MRSRB7          EQU     0x48000030     ;Mode register set for SDRAM

;==========================
; CLOCK & POWER MANAGEMENT
;==========================
LOCKTIME        EQU     0x4c000000     ;PLL lock time counter
MPLLCON         EQU     0x4c000004     ;MPLL Control
UPLLCON         EQU     0x4c000008     ;UPLL Control
CLKCON          EQU     0x4c00000c     ;Clock generator control
CLKSLOW         EQU     0x4c000010     ;Slow clock control
CLKDIVN         EQU     0x4c000014     ;Clock divider control


; /************************************************************************/

; 引入的外部标号在这声明
        IMPORT  __main                          ;C语言主程序入口 
        IMPORT  SoftwareInterrupt
    

; 给外部使用的标号在这声明
        EXPORT  Reset
        EXPORT  VICVectAddr
        EXPORT  bottom_of_heap
        EXPORT  StackUsr

        EXPORT  __user_initial_stackheap    
    

; /************************************************************************/
        CODE32
        AREA    vectors,CODE,READONLY
; 异常向量表
Reset
        LDR     PC, ResetAddr
        LDR     PC, UndefinedAddr
        LDR     PC, SWI_Addr
        LDR     PC, PrefetchAddr
        LDR     PC, DataAbortAddr
        DCD     IRQ_Addr
        LDR     PC, IRQ_Addr
        LDR     PC, FIQ_Addr

ResetAddr           DCD     ResetInit
UndefinedAddr       DCD     Undefined
SWI_Addr            DCD     SoftwareInterrupt
PrefetchAddr        DCD     PrefetchAbort
DataAbortAddr       DCD     DataAbort
Nouse               DCD     0
IRQ_Addr            DCD     IRQ_Handler
FIQ_Addr            DCD     FIQ_Handler


; 未定义指令
Undefined
        B       Undefined

SwiFunction
        DCD     IRQDisable       ;0
        DCD     IRQEnable        ;1
        DCD     FIQDisable       ;2
        DCD     FIQEnable        ;3

IRQDisable
        ;关IRQ中断
        MRS     R0, SPSR
        ORR     R0, R0, #I_BIT
        MSR     SPSR_c, R0
        MOVS    PC, LR

IRQEnable
        ;开IRQ中断
        MRS     R0, SPSR
        BIC     R0, R0, #I_BIT
        MSR     SPSR_c, R0
        MOVS    PC, LR
        
FIQDisable
        ;关FIQ中断
        MRS     R0, SPSR
        ORR     R0, R0, #F_BIT
        MSR     SPSR_c, R0
        MOVS    PC, LR

FIQEnable
        ;开FIQ中断
        MRS     R0, SPSR
        BIC     R0, R0, #F_BIT
        MSR     SPSR_c, R0
        MOVS    PC, LR

        
; 取指中止
PrefetchAbort
        B       PrefetchAbort

; 取数据中止
DataAbort
        B       DataAbort
        
; IRQ中断
NoInt       EQU 0x80

USR32Mode   EQU 0x10
SVC32Mode   EQU 0x13
SYS32Mode   EQU 0x1f
IRQ32Mode   EQU 0x12
FIQ32Mode   EQU 0x11


;引入的外部标号在这声明
        IMPORT  OSIntCtxSw                      ;任务切换函数
        IMPORT  OSIntExit                       ;中断退出函数
        IMPORT  OSTCBCur
        IMPORT  OSTCBHighRdy
        IMPORT  OSIntNesting                    ;中断嵌套计数器
        IMPORT  OsEnterSum


IRQ_Handler
        SUB     LR, LR, #4                      ; 计算返回地址
        STMFD   SP!, {R0-R3, R12, LR}           ; 保存任务环境
        MRS     R3, SPSR                        ; 保存状态
        STMFD   SP, {R3, SP, LR}^               ; 保存用户状态的R3,SP,LR,注意不能回写
                                                ; 如果回写的是用户的SP，所以后面要调整SP
        LDR     R2,  =OSIntNesting              ; OSIntNesting++
        LDRB    R1, [R2]
        ADD     R1, R1, #1
        STRB    R1, [R2]

        SUB     SP, SP, #4*3
        
        MSR     CPSR_c, #(NoInt | SYS32Mode)    ; 切换到系统模式
        CMP     R1, #1
        LDREQ   SP, =StackUsr
        

        LDR     R0, =INTPND
        LDR     R1, [R0]                        ; 读取INTPND的值

        ; 找出当前中断号(INTPND)
        MOV     R0, #0       
FIND_NO 
        MOVS    R1, R1, LSR #1
        ADDNE   R0, R0, #1
        BNE     FIND_NO
FIND_END

        LDR     R1, =VICVectAddr 
        MOV     LR, PC                          ; 保存返回地址
        LDR     PC, [R1, R0, LSL #2]            ; 跳转到相应中断服务程序        

        MSR     CPSR_c, #(NoInt | SYS32Mode)    ; 切换到系统模式
        LDR     R2, =OsEnterSum                 ; OsEnterSum,使OSIntExit退出时中断关闭
        MOV     R1, #1
        STR     R1, [R2]

        BL      OSIntExit

        LDR     R2, =OsEnterSum                 ; 因为中断服务程序要退出，所以OsEnterSum=0
        MOV     R1, #0
        STR     R1, [R2]

        MSR     CPSR_c, #(NoInt | IRQ32Mode)    ; 切换回irq模式
        LDMFD   SP, {R3, SP, LR}^               ; 恢复用户状态的R3,SP,LR,注意不能回写
                                                ; 如果回写的是用户的SP，所以后面要调整SP
        LDR     R0, =OSTCBHighRdy
        LDR     R0, [R0]
        LDR     R1, =OSTCBCur
        LDR     R1, [R1]
        CMP     R0, R1

        ADD     SP, SP, #4*3                    ; 
        MSR     SPSR_cxsf, R3
        LDMEQFD SP!, {R0-R3, R12, PC}^          ; 不进行任务切换
        LDR     PC, =OSIntCtxSw                 ; 进行任务切换


; 快速中断
FIQ_Handler
        STMFD   SP!, {R0-R3, LR}        
        ; /* FIQ中断处理 */        
        LDMFD   SP!, {R0-R3, LR}
        SUBS    PC,  LR,  #4                
                
        
        
;/*********************************************************************************************************
;** 函数名称: Reset
;** 功能描述: 复位入口
;** 输　入: 无
;** 输　出: 无
;********************************************************************************************************/
ResetInit
        BL      InitStack               ; 初始化堆栈                                              
        BL      TargetInitReset         ; 针对目标板的系统初始化        
        BL      Remap                   ; 重映射操作
        B       __main                  ; 跳转到c语言入口  


;/*********************************************************************************************************
;** 函数名称: Remap
;** 功能描述: 重映射向量表操作。
;** 输　入: 无
;** 输　出: 无
;** 说明：将向量表复制到0x0000000地址，所以要求系统为NAND Flash启动方式。
;**       占用R0--R9寄存器。
;********************************************************************************************************/ 
Remap
    IF :DEF: Release
        MOV     PC, LR
    ELSE        
        MOV     R0, #0x00000000
        LDR     R1, =Reset
        LDMIA   R1!, {R2-R9}
        STMIA   R0!, {R2-R9}
        LDMIA   R1!, {R2-R9}
        STMIA   R0!, {R2-R9}     
        MOV     PC, LR
    ENDIF       
 
;/*********************************************************************************************************
;** 函数名称: TargetInitReset
;** 功能描述: 针对目标板的系统初始化，包括WDT、中断、PLL、SDRAM控制器等等。
;** 输　入: 无
;** 输　出: 无
;** 说明：占用R0--R8寄存器
;********************************************************************************************************/ 
TargetInitReset
        LDR     R0, =WTCON                  ; 关闭WDT 
        LDR     R1, =0x0000         
        STR     R1,[R0]                     ; WTCON=0x0000

        LDR     R0, =INTMSK                 ; 禁止所有中断 (中断控制器)
        LDR     R1, =0xFFFFFFFF  
        STR     R1, [R0]                    ; INTMSK=0xFFFFFFFF

        LDR     R0, =INTSUBMSK
        LDR     R1, =0x07FF                 ; INTSUBMSK=0x07FF
        STR     R1, [R0]
        
        LDR     R0, =SRCPND                 ; 清除中断标志 (add)
        LDR     R1, =0xFFFFFFFF
        STR     R1, [R0]
        
        LDR     R0, =INTPND      
        LDR     R1, =0xFFFFFFFF  
        STR     R1, [R0]
        
    IF :DEF: Release    
        ; 系统时钟设置，启用PLL
        LDR     R0, =LOCKTIME
        LDR     R1, =0x00FFFFFF             ; 锁定时间设置U_LTIME=0xFFF，M_LTIME=0xFFF
        STR     R1, [R0]
        
        LDR     R0, =CLKDIVN     
        MOV     R1, #0x03                   ; HCLK=FCLK/2，PCLK=HCLK/2
        STR     R1, [R0]             
        
        MRC     p15, 0, R1, c1, c0, 0       ; (MMU设置) 读控制寄存器 
        ORR     R1, R1, #0xC0000000   
        MRC     p15, 0, R1, c1, c0, 0    

        LDR     R0, =MPLLCON     
        LDR     R1, =MPLLCON_200            ; 设置CPU时钟为200Mhz (FCLK)
        STR     R1, [R0]                             
            
        ; 总线设置，初始化SDRAM
        LDR     R0, =BUS_INIT
        LDR     R1, =BWSCON
        LDMIA   R0!, {R2-R8}
        STMIA   R1!, {R2-R8}
        LDMIA   R0!, {R2-R7}
        STMIA   R1!, {R2-R7}         
    ENDIF                                           
    
        MOV     PC, LR               ; 返回
        
; 总线配置数据表        
BUS_INIT    DCD (B7_BWCON<<28)|(B6_BWCON<<24)|(B5_BWCON<<20)|(B4_BWCON<<16) \
                              |(B3_BWCON<<12)|(B2_BWCON<<8)|(B1_BWCON<<4)   ; BWSCON  寄存器
            DCD (1<<13)|(1<<11)|(7<<8)|(1<<6)|(1<<4)|(1<<2)|(0<<0)          ; BANKCON0寄存器
            DCD (1<<13)|(1<<11)|(7<<8)|(1<<6)|(1<<4)|(1<<2)|(0<<0)          ; BANKCON1寄存器    
            DCD (1<<13)|(1<<11)|(7<<8)|(1<<6)|(1<<4)|(1<<2)|(0<<0)          ; BANKCON2寄存器
            DCD (1<<13)|(1<<11)|(7<<8)|(1<<6)|(1<<4)|(1<<2)|(0<<0)          ; BANKCON3寄存器
            DCD (1<<13)|(1<<11)|(7<<8)|(1<<6)|(1<<4)|(1<<2)|(0<<0)          ; BANKCON4寄存器
            DCD (1<<13)|(1<<11)|(7<<8)|(1<<6)|(1<<4)|(1<<2)|(0<<0)          ; BANKCON5寄存器
            DCD (3<<15)|(1<<2)|(1<<0)                                       ; BANKCON6寄存器(SDRAM)
            DCD (0<<15)|(1<<13)|(1<<11)|(7<<8)|(1<<6)|(1<<4)|(1<<2)|(0<<0)  ; BANKCON7寄存器(SRAM )
            DCD (1<<23)|(0<<22)|(0<<20)|(3<<18)|(1113)                      ; REFRESH 寄存器(SDRAM) ，period=15.6us, HCLK=60Mhz, (2048+1-15.6*60)
            DCD (1<<7)|(1<<5)|(1<<4)|(2<<0)                                 ; BANKSIZE寄存器(128MB)
            DCD (3<<4)                                                      ; MRSRB6  寄存器
            DCD (3<<4)                                                      ; MRSRB7  寄存器
               
               
;/*********************************************************************************************************
;** 函数名称: InitStack
;** 功能描述: 初始化堆栈。最后返回时，处理器工作在系统模式。
;** 输　入:   无
;** 输　出:   无
;** 说  明:   由本文件开头的USR_STACK_LEGTH、SVC_STACK_LEGTH等定义各工作模式的堆栈大小。
;********************************************************************************************************/
InitStack    
        MOV     R0, LR

;设置管理模式堆栈
        MSR     CPSR_c, #(Mode_SVC | I_BIT | F_BIT)     ; 0xd3
        LDR     SP, StackSvc
;设置中断模式堆栈
        MSR     CPSR_c, #(Mode_IRQ | I_BIT | F_BIT)     ; 0xd2
        LDR     SP, StackIrq
;设置快速中断模式堆栈
        MSR     CPSR_c, #(Mode_FIQ | I_BIT | F_BIT)     ; 0xd1
        LDR     SP, StackFiq
;设置中止模式堆栈
        MSR     CPSR_c, #(Mode_ABT | I_BIT | F_BIT)     ; 0xd7
        LDR     SP, StackAbt
;设置未定义模式堆栈
        MSR     CPSR_c, #(Mode_UND | I_BIT | F_BIT)     ; 0xdb
        LDR     SP, StackUnd
;设置系统模式堆栈
        MSR     CPSR_c, #(Mode_SYS | I_BIT | F_BIT)     ; 0xdf
        LDR     SP, =StackUsr

        MOV     PC, R0

StackSvc           DCD     SvcStackSpace + (SVC_STACK_LEGTH - 1)* 4
StackIrq           DCD     IrqStackSpace + (IRQ_STACK_LEGTH - 1)* 4
StackFiq           DCD     FiqStackSpace + (FIQ_STACK_LEGTH - 1)* 4
StackAbt           DCD     AbtStackSpace + (ABT_STACK_LEGTH - 1)* 4
StackUnd           DCD     UndtStackSpace + (UND_STACK_LEGTH - 1)* 4

    
;/*********************************************************************************************************
;** 函数名称: __user_initial_stackheap 
;** 功能描述: 库函数初始化堆和栈，不能删除
;** 输　入: 参考库函数手册
;** 输　出: 参考库函数手册
;********************************************************************************************************/
__user_initial_stackheap    
    LDR   r0, =bottom_of_heap
    MOV   pc, lr

; /* 分配堆栈空间 */
        AREA    MyStacks, DATA, NOINIT, ALIGN=2
SvcStackSpace      SPACE   SVC_STACK_LEGTH * 4  ;管理模式堆栈空间
IrqStackSpace      SPACE   IRQ_STACK_LEGTH * 4  ;中断模式堆栈空间
FiqStackSpace      SPACE   FIQ_STACK_LEGTH * 4  ;快速中断模式堆栈空间
AbtStackSpace      SPACE   ABT_STACK_LEGTH * 4  ;中止义模式堆栈空间
UndtStackSpace     SPACE   UND_STACK_LEGTH * 4  ;未定义模式堆栈

; /* IRQ中断向量地址表定义 */
VICVectAddr        SPACE   32*4  

        AREA    Heap, DATA, READWRITE
bottom_of_heap    SPACE   1

        AREA    Stacks, DATA, NOINIT
StackUsr
        END
;/*********************************************************************************************************
;**                            End Of File
;********************************************************************************************************/
