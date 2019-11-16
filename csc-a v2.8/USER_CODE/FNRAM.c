#include "FNRAM.h"

/*״̬��־*/
uint8 ack;	         /*Ӧ���־λ*/

void delayus(void)
{
    uint8 i = 5;
    while(i--);     
}

void FNRAM_init(void)
{
    //PINSEL
    LPC_GPIO0->FIODIR |= FM_WP_PIN | FM_SCL_PIN;
	LPC_GPIO1->FIODIR |= FM_SDA_PIN;    
    WRITE_FM_ENABLE;
}

/*******************************************************************
                     �����ߺ���               
����ԭ��: void  Start_I2c();  
����:����I2C����,������I2C��ʼ����. 
********************************************************************/
void Start_I2c(void)
{
  	SET_SDA;      /*������ʼ�����������ź�*/
  	delayus();
  	
  	SET_SCL;
  	delayus();    /*��ʼ��������ʱ�����4.7us,��ʱ*/
  	delayus();
 	delayus();
  	delayus();
  	delayus();   
  	 
  	CLR_SDA;      /*������ʼ�ź�*/
  	delayus();    /* ��ʼ��������ʱ�����4��s*/
  	delayus();
  	delayus();
  	delayus();
  	delayus();      
  	 
  	CLR_SCL;      /*ǯסI2C���ߣ�׼�����ͻ�������� */
  	delayus();
  	delayus();
}

/*******************************************************************
                      �������ߺ���               
����ԭ��: void  Stop_I2c();  
����:����I2C����,������I2C��������.  
********************************************************************/
void Stop_I2c(void)
{
    CLR_SDA;     /*���ͽ��������������ź�*/
    delayus();   /*���ͽ���������ʱ���ź�*/
    
    SET_SCL;     /*������������ʱ�����4��s*/
    delayus();
    delayus();
    delayus();
    delayus();
    delayus();
    
    SET_SDA;     /*����I2C���߽����ź�*/
    delayus();
    delayus();
    delayus();
    delayus();
}

/*******************************************************************
                 �ֽ����ݴ��ͺ���               
����ԭ��: void  SendByte(uint8 c);
����: ������c���ͳ�ȥ,�����ǵ�ַ,Ҳ����������,�����ȴ�Ӧ��,����
      ��״̬λ���в���.(��Ӧ����Ӧ��ʹack=0 ��)     
      ��������������ack=1; ack=0��ʾ��������Ӧ����𻵡�
********************************************************************/
void  SendByte(uint8 c)
{
    uint8 BitCnt;
 
    for(BitCnt = 0; BitCnt < 8; BitCnt ++)  /*Ҫ���͵����ݳ���Ϊ8λ*/
    {
        if((c<<BitCnt)&0x80) SET_SDA;   /*�жϷ���λ*/
        else  CLR_SDA;                
        delayus();
        SET_SCL;                 /*��ʱ����Ϊ�ߣ�֪ͨ��������ʼ��������λ*/
        delayus(); 
        delayus();               /*��֤ʱ�Ӹߵ�ƽ���ڴ���4��s*/
        delayus();
        delayus();
        delayus();         
        CLR_SCL; 
    }
    
    delayus();
    delayus();
    SET_SDA;               /*8λ��������ͷ������ߣ�׼������Ӧ��λ*/
    delayus();
    delayus();   
    SET_SCL;
    delayus();
    delayus();
    delayus();
    
    DATA_IN;  
    delayus(); 
    if(READ_IO) ack=0;     
    else ack=1;        /*�ж��Ƿ���յ�Ӧ���ź�*/   
    delayus();
    DATA_OUT;
       
    CLR_SCL;
    delayus();
    delayus();
}

/*******************************************************************
                 �ֽ����ݴ��ͺ���               
����ԭ��: uint8  RcvByte();
����:  �������մ���������������,���ж����ߴ���(����Ӧ���ź�)��
     ���������Ӧ������  
********************************************************************/	
uint8  RcvByte()
{
    uint8 retc;
    uint8 BitCnt;
  
    retc = 0; 
    SET_SDA;             /*��������Ϊ���뷽ʽ*/
    
    for(BitCnt = 0; BitCnt < 8; BitCnt++)
    {
        delayus();           
        CLR_SCL;       /*��ʱ����Ϊ�ͣ�׼����������λ*/
        delayus();
        delayus();         /*ʱ�ӵ͵�ƽ���ڴ���4.7��s*/
        delayus();
        delayus();
        delayus();
        SET_SCL;       /*��ʱ����Ϊ��ʹ��������������Ч*/
        delayus();
        delayus();
        retc=retc<<1;
        
        DATA_IN;
        delayus();
        if(READ_IO) retc=retc+1; /*������λ,���յ�����λ����retc�� */
        delayus();
        DATA_OUT;
        
        delayus();
        delayus(); 
    } 
    
    CLR_SCL;    
    delayus();
    delayus();
    return(retc);
}

/********************************************************************
                     Ӧ���Ӻ���
ԭ��:  void Ack_I2c(bit a);
 
����:����������Ӧ���ź�,(������Ӧ����Ӧ���ź�)
********************************************************************/
void Ack_I2c(uint8 a)
{
    if(a==0)   CLR_SDA;     /*�ڴ˷���Ӧ����Ӧ���ź� */
	else   SET_SDA;
	delayus();
	delayus();
	delayus(); 
	     
	SET_SCL;
    delayus();
    delayus();              /*ʱ�ӵ͵�ƽ���ڴ���4��s*/
    delayus();
    delayus();
    delayus();  
    
 	CLR_SCL;                /*��ʱ���ߣ�ǯסI2C�����Ա��������*/
    delayus();
    delayus();    
}

/*******************************************************************
                    �����ӵ�ַ���������ֽ����ݺ���               
����ԭ��: bit  ISendByte(uint8 sla,ucahr c);  
����:     ���������ߵ����͵�ַ�����ݣ��������ߵ�ȫ����,��������ַsla.
          �������1��ʾ�����ɹ��������������
ע�⣺    ʹ��ǰ�����ѽ������ߡ�
********************************************************************/
uint8 ISendByte(uint8 sla,uint8 c)
{
    Start_I2c();               /*��������*/
    SendByte(sla);             /*����������ַ*/
    if(ack==0)return(0);
    SendByte(c);               /*��������*/
    if(ack==0)return(0);
    Stop_I2c();                /*��������*/ 
    return(1);
}

uint8 ISendStr(uint8 sla,uint8 subah, uint8 subal, uint8 *s,uint8 no)
{
   	uint8 i;

   	Start_I2c();              		/*��������*/
  
   	SendByte(sla);            		/*����������ַ*/
   	if(ack == 0)return(0);
     
   	SendByte(subah);  				/*���������ӵ�ַ*/
   	if(ack == 0)return(0);			//��λ
   	
    SendByte(subal);            	
    if(ack == 0)return(0);			//��λ


   	for(i = 0; i < no; i++)
    {   
     	SendByte(*s);          		/*��������*/
       	if(ack == 0)
       	{
       		return(0);
     	}
     	s++;
    } 
 	Stop_I2c();                 	/*��������*/ 
 	
  	return(1);
}

/*******************************************************************
                    �����ӵ�ַ�������ֽ����ݺ���               
����ԭ��: bit  IRcvByte(uint8 sla,ucahr *c);  
����:     ���������ߵ����͵�ַ�������ݣ��������ߵ�ȫ����,��������
          ַsla������ֵ��c.
           �������1��ʾ�����ɹ��������������
ע�⣺    ʹ��ǰ�����ѽ������ߡ�
********************************************************************/
uint8 IRcvByte(uint8 sla,uint8 *c)
{
	Start_I2c();               /*��������*/
	SendByte(sla+1);           /*����������ַ*/
	if(ack==0)return(0);
	*c=RcvByte();              /*��ȡ����*/
	Ack_I2c(1);                /*���ͷǾʹ�λ*/
	Stop_I2c();                /*��������*/ 
	return(1);
}

uint8 IRcvStr(uint8 sla,uint8 subah, uint8 subal, uint8 *s,uint8 no)
{
   	uint8 i;

   	Start_I2c();               	/*��������*/
   	SendByte(sla);            	/*����������ַ*/
 	if(ack==0)return(0);
     
     
	SendByte(subah);            	/*����������ַ*/
    if(ack == 0) return(0);		//��λ
    
    SendByte(subal);            
    if(ack == 0) return(0);		//��λ
     
     
	Start_I2c();
	SendByte(sla + 1);
    if(ack == 0) return(0);

   	for(i = 0; i < (no - 1); i++)
   	{   
	 	*s = RcvByte();          /*��������*/
	   	Ack_I2c(0);            /*���;ʹ�λ*/  
	   	s++;
   	} 
    
   	*s=RcvByte();
    Ack_I2c(1);                /*���ͷ�Ӧλ*/
 	Stop_I2c();                /*��������*/ 
 	
  	return(1);
}







