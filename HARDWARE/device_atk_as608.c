/*
********************************************************************************
*                            �й���������������ϵͳ
*                            Thread Operating System
*
*                              ��оƬ:STM32F401re
*
********************************************************************************
*�ļ���     : device_atk_as608.c
*����       : ATK_AS608����
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1   Guolz       2019/4/5      ��ʼ�汾
********************************************************************************
*/
#include "system.h"

////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��os,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "rtthread.h"					//os ʹ��	  
#endif


char handshake_status = handshake_pre;
uint32_t AS608_Addr = 0XFFFFFFFF; 		//Ĭ��
uint16_t ValidN;
SysPara AS608Para;						//ָ��ģ��AS608����


const char *EnsureMessage(uint8_t ensure) 
{
	const char *p;
	switch(ensure)
	{
		case  0x00:
			p="OK";break;		
		case  0x01:
			p="���ݰ����մ���";break;
		case  0x02:
			p="��������û����ָ";break;
		case  0x03:
			p="¼��ָ��ͼ��ʧ��";break;
		case  0x04:
			p="ָ��ͼ��̫�ɡ�̫��������������";break;
		case  0x05:
			p="ָ��ͼ��̫ʪ��̫��������������";break;
		case  0x06:
			p="ָ��ͼ��̫�Ҷ�����������";break;
		case  0x07:
			p="ָ��ͼ����������������̫�٣������̫С��������������";break;
		case  0x08:
			p="ָ�Ʋ�ƥ��";break;
		case  0x09:
			p="û������ָ��";break;
		case  0x0a:
			p="�����ϲ�ʧ��";break;
		case  0x0b:
			p="����ָ�ƿ�ʱ��ַ��ų���ָ�ƿⷶΧ";
		case  0x10:
			p="ɾ��ģ��ʧ��";break;
		case  0x11:
			p="���ָ�ƿ�ʧ��";break;	
		case  0x15:
			p="��������û����Чԭʼͼ��������ͼ��";break;
		case  0x18:
			p="��д FLASH ����";break;
		case  0x19:
			p="δ�������";break;
		case  0x1a:
			p="��Ч�Ĵ�����";break;
		case  0x1b:
			p="�Ĵ����趨���ݴ���";break;
		case  0x1c:
			p="���±�ҳ��ָ������";break;
		case  0x1f:
			p="ָ�ƿ���";break;
		case  0x20:
			p="��ַ����";break;
		default :
			p="ģ�鷵��ȷ��������";break;
	}

	return p;	
}


//���Ͱ�ͷ
static void SendHead(void){
    System.Device.Usart2.WriteData(0xEF);
    System.Device.Usart2.WriteData(0x01);
}
//���͵�ַ
static void SendAddr(void)
{
	System.Device.Usart2.WriteData(((AS608Addr>>24)&0xff));
	System.Device.Usart2.WriteData(((AS608Addr>>16)&0xff));
 	System.Device.Usart2.WriteData(((AS608Addr>>8)&0xff));
	System.Device.Usart2.WriteData(AS608Addr&0xff);
}
//���Ͱ���ʶ,
static void SendFlag(uint8_t flag)
{
	System.Device.Usart2.WriteData(flag);
}
//���Ͱ�����
static void SendLength(int length)
{
	System.Device.Usart2.WriteData(length>>8);
	System.Device.Usart2.WriteData(length);
}
//����ָ����
static void Sendcmd(u8 cmd)
{
	System.Device.Usart2.WriteData(cmd);
}
//����У���
static void SendCheck(u16 check)
{
	System.Device.Usart2.WriteData(check>>8);
	System.Device.Usart2.WriteData(check);
}



uint8_t PS_HandShake(uint32_t *PS_Addr)
{
	SendHead();
	SendAddr();
	System.Device.Usart2.WriteData(0x01);
	System.Device.Usart2.WriteData(0x00);
	System.Device.Usart2.WriteData(0x00);	
	delay_ms(200);
    
	if(handshake_status == handshake_judging)//���յ�����
	{		
        handshake_status = handshake_done;
        *PS_Addr=(rx2Buff[2]<<24) + (rx2Buff[3]<<16) + (rx2Buff[4]<<8) + (rx2Buff[5]);
        
        return 0;
	}
	return 1;		
}

uint8_t PS_ValidTempleteNum(uint16_t *ValidN)
{
	uint16_t temp;
    uint8_t ensure;
	uint8_t *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x03);
	Sendcmd(0x1d);
	temp = 0x01+0x03+0x1d;
	SendCheck(temp);
    data=JudgeStr(2000);
	
    if(data){
		ensure = data[9];
		*ValidN = (data[10]<<8) +data[11];
	}		
	else
		ensure=0xff;
	
	if(ensure==0x00){
		printf("\r\n��Чָ�Ƹ���=%d",(data[10]<<8)+data[11]);
	}
	else
		printf("\r\n%s",EnsureMessage(ensure));
	
	return ensure;
}

uint8_t PS_ReadSysPara(SysPara *p)
{
	uint16_t temp;
  	uint8_t ensure;
	uint8_t  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);		//�������ʶ
	SendLength(0x03);
	Sendcmd(0x0F);
	temp = 0x01+0x03+0x0F;
	SendCheck(temp);
	data=JudgeStr(1000);
	if(data)
	{
		ensure = data[9];
		p->PS_max = (data[14]<<8)+data[15];
		p->PS_level = data[17];
		p->PS_addr=(data[18]<<24)+(data[19]<<16)+(data[20]<<8)+data[21];
		p->PS_size = data[23];
		p->PS_N = data[25];
	}		
	else
		ensure=0xff;
	
	if(ensure==0x00)
	{
		printf("\r\nģ�����ָ������=%d",p->PS_max);
		printf("\r\n�Աȵȼ�=%d",p->PS_level);
		printf("\r\n��ַ=%x",p->PS_addr);
		printf("\r\n������=%d",p->PS_N*9600);
	}
	else 
		printf("\r\n%s",EnsureMessage(ensure));
	
	return ensure;
}

static uint8_t FingerDetect(void){
	uint8_t ret = 0;
	
	if(HAL_GPIO_ReadPin(StaPORT,GPIO_PIN_10) == GPIO_PIN_SET){
		LEDOn();
		ret = 1;
	}else{
		LEDOff();
		ret = 0;
	}
	
	return ret;
}

//¼��ͼ�� PS_GetImage
//����:̽����ָ��̽�⵽��¼��ָ��ͼ�����ImageBuffer�� 
//ģ�鷵��ȷ����
uint8_t PS_GetImage(void)
{
  	uint16_t temp;
  	uint8_t  ensure;
	uint8_t  *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);		//�������ʶ
	SendLength(0x03);
	Sendcmd(0x01);
 	temp = 0x01+0x03+0x01;
	SendCheck(temp);
	data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	
	return ensure;
}


//�������� PS_GenChar
//����:��ImageBuffer�е�ԭʼͼ������ָ�������ļ�����CharBuffer1��CharBuffer2			 
//����:BufferID --> charBuffer1:0x01	charBuffer1:0x02												
//ģ�鷵��ȷ����
uint8_t PS_GenChar(uint8_t BufferID)
{
	uint16_t temp;
  	uint8_t ensure;
	uint8_t *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x04);
	Sendcmd(0x02);
	System.Device.Usart2.WriteData(BufferID);
	temp = 0x01+0x04+0x02+BufferID;
	SendCheck(temp);
	data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	
	return ensure;
}


//��������PS_HighSpeedSearch
//���ܣ��� CharBuffer1��CharBuffer2�е������ļ��������������򲿷�ָ�ƿ⡣
//		  �����������򷵻�ҳ��,��ָ����ڵ�ȷ������ָ�ƿ��� ���ҵ�¼ʱ����
//		  �ܺõ�ָ�ƣ���ܿ�������������
//����:  BufferID�� StartPage(��ʼҳ)��PageNum��ҳ����
//˵��:  ģ�鷵��ȷ����+ҳ�루����ָ��ģ�壩
uint8_t PS_HighSpeedSearch(uint8_t BufferID,uint16_t StartPage,uint16_t PageNum,SearchResult *p)
{
	uint16_t temp;
  	uint8_t ensure;
	uint8_t *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x08);
	Sendcmd(0x1b);
	System.Device.Usart2.WriteData(BufferID);
	System.Device.Usart2.WriteData(StartPage>>8);
	System.Device.Usart2.WriteData(StartPage);
	System.Device.Usart2.WriteData(PageNum>>8);
	System.Device.Usart2.WriteData(PageNum);
	temp = 0x01+0x08+0x1b+BufferID
	+(StartPage>>8)+(uint8_t)StartPage
	+(PageNum>>8)+(uint8_t)PageNum;
	SendCheck(temp);
	data=JudgeStr(2000);
 	if(data)
	{
		ensure=data[9];
		p->pageID 	=(data[10]<<8) +data[11];
		p->mathscore=(data[12]<<8) +data[13];
	}
	else
		ensure=0xff;
	
	return ensure;
}


//ˢָ��
void press_FR(void)
{
	SearchResult seach;
	uint8_t ensure;
	char *str;
	ensure=PS_GetImage();
	if(ensure==0x00)//��ȡͼ��ɹ� 
	{	
		//�򿪷�����	
		ensure=PS_GenChar(CharBuffer1);
		if(ensure==0x00) //���������ɹ�
		{		
//			BEEP=0;//�رշ�����	
			ensure=PS_HighSpeedSearch(CharBuffer1,0,AS608Para.PS_max,&seach);
			if(ensure==0x00)//�����ɹ�
			{				
				printf("ˢָ�Ƴɹ�\r\n");				
				printf("ȷ�д���,ID:%d  ƥ��÷�:%d",seach.pageID,seach.mathscore);
			}
			else 
				printf("\r\n%s\r\n",EnsureMessage(ensure));			
	  }else{
			printf("\r\n%s\r\n",EnsureMessage(ensure));	
	  }

	  //�رշ�����
	 delay_ms(600);
	}
}

//��ȷ�ȶ���öָ������ PS_Match
//����:��ȷ�ȶ�CharBuffer1 ��CharBuffer2 �е������ļ� 
//ģ�鷵��ȷ����
uint8_t PS_Match(void)
{
	uint16_t temp;
   	uint8_t ensure;
	uint8_t *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);			//�������ʶ
	SendLength(0x03);
	Sendcmd(0x03);
	temp = 0x01+0x03+0x03;
	SendCheck(temp);
	data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;
}

//�ϲ�����������ģ�壩PS_RegModel
//����:��CharBuffer1��CharBuffer2�е������ļ��ϲ����� ģ��,�������CharBuffer1��CharBuffer2	
//˵��:  ģ�鷵��ȷ����
uint8_t PS_RegModel(void)
{
	uint16_t temp;
    uint8_t ensure;
	uint8_t *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x03);
	Sendcmd(0x05);
	temp = 0x01+0x03+0x05;
	SendCheck(temp);
	data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;		
}

//����ģ�� PS_StoreChar
//����:�� CharBuffer1 �� CharBuffer2 �е�ģ���ļ��浽 PageID ��flash���ݿ�λ�á�			
//����:  BufferID @ref charBuffer1:0x01	charBuffer1:0x02
//       PageID��ָ�ƿ�λ�úţ�
//˵��:  ģ�鷵��ȷ����
uint8_t PS_StoreChar(u8 BufferID,u16 PageID)
{
	uint16_t temp;
  	uint8_t ensure;
	uint8_t *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x06);
	Sendcmd(0x06);
	System.Device.Usart2.WriteData(BufferID);
	System.Device.Usart2.WriteData(PageID>>8);
	System.Device.Usart2.WriteData(PageID);
	temp = 0x01+0x06+0x06+BufferID
	+(PageID>>8)+(u8)PageID;
	SendCheck(temp);
	data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;	
}


//¼ָ��
void Add_FR(void)
{
	uint8_t i,ensure ,processnum=0;
	uint16_t ID;
	while(1)
	{
		switch (processnum)
		{
			case 0:
				i++;
				printf("�밴ָ��\r\n");
				ensure=PS_GetImage();
				if(ensure==0x00) 
				{
					//��������
					ensure=PS_GenChar(CharBuffer1);//��������
					//������ͣ
					if(ensure==0x00)
					{
						printf("ָ������\r\n");
						i=0;
						processnum=1;//�����ڶ���						
					}else printf("\r\n%s\r\n",EnsureMessage(ensure));					
				}else printf("\r\n%s\r\n",EnsureMessage(ensure));						
				break;
			
			case 1:
				i++;
				printf("�밴�ٰ�һ��ָ��\r\n");
				ensure=PS_GetImage();
				if(ensure==0x00) 
				{
					//��������
					ensure=PS_GenChar(CharBuffer2);//��������
					//������ͣ
					if(ensure==0x00)
					{
						printf("ָ������\r\n");
						i=0;
						processnum=2;//����������
					}else printf("\r\n%s\r\n",EnsureMessage(ensure));
				}else printf("\r\n%s\r\n",EnsureMessage(ensure));	
				break;

			case 2:
				printf("�Ա�����ָ��\r\n");
				ensure=PS_Match();
				if(ensure==0x00) 
				{
					printf("�Աȳɹ�,����ָ��һ��\r\n");
					processnum=3;//�������Ĳ�
				}
				else 
				{
					printf("�Ա�ʧ�ܣ�������¼��ָ��\r\n");
					printf("\r\n%s\r\n",EnsureMessage(ensure));	
					i=0;
					processnum=0;//���ص�һ��		
				}
				delay_ms(1200);
				break;

			case 3:
				printf("����ָ��ģ��\r\n");
				ensure=PS_RegModel();
				if(ensure==0x00) 
				{
					printf("����ָ��ģ��ɹ�\r\n");
					processnum=4;//�������岽
				}else {processnum=0;printf("\r\n%s\r\n",EnsureMessage(ensure));}
				delay_ms(1200);
				break;
				
			case 4:	
				printf("�����봢��ID,��Enter����\r\n");
				printf("0=< ID <=299\r\n");
//				do
//					ID=GET_NUM();
//				while(!(ID<AS608Para.PS_max));//����ID����С��ģ������������ֵ
				ensure=PS_StoreChar(CharBuffer2,ID);//����ģ��
				if(ensure==0x00) 
				{			
					printf("¼��ָ�Ƴɹ�\r\n");
					PS_ValidTempleteNum(&ValidN);//����ָ�Ƹ���
					printf("ʣ������%d\r\n",AS608Para.PS_max-ValidN);
					delay_ms(1500);
					return ;
				}else {processnum=0;printf("\r\n%s\r\n",EnsureMessage(ensure));}					
				break;				
		}
		delay_ms(400);
		if(i==5)//����5��û�а���ָ���˳�
		{
			printf("�˳�\r\n");
			break;	
		}				
	}
}

//���ָ�ƿ� PS_Empty
//����:  ɾ��flash���ݿ�������ָ��ģ��
//����:  ��
//˵��:  ģ�鷵��ȷ����
uint8_t PS_Empty(void)
{
	uint16_t temp;
  	uint8_t ensure;
	uint8_t *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x03);
	Sendcmd(0x0D);
	temp = 0x01+0x03+0x0D;
	SendCheck(temp);
	data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	return ensure;
}

//ɾ��ģ�� PS_DeletChar
//����:  ɾ��flash���ݿ���ָ��ID�ſ�ʼ��N��ָ��ģ��
//����:  PageID(ָ�ƿ�ģ���)��Nɾ����ģ�������
//˵��:  ģ�鷵��ȷ����
uint8_t PS_DeletChar(u16 PageID,u16 N)
{
	uint16_t temp;
  	uint8_t ensure;
	uint8_t *data;
	SendHead();
	SendAddr();
	SendFlag(0x01);//�������ʶ
	SendLength(0x07);
	Sendcmd(0x0C);
	System.Device.Usart2.WriteData(PageID>>8);
	System.Device.Usart2.WriteData(PageID);
	System.Device.Usart2.WriteData(N>>8);
	System.Device.Usart2.WriteData(N);
	temp = 0x01+0x07+0x0C
	+(PageID>>8)+(u8)PageID
	+(N>>8)+(u8)N;
	SendCheck(temp);
	data=JudgeStr(2000);
	if(data)
		ensure=data[9];
	else
		ensure=0xff;
	
	return ensure;
}


void Del_FR(void)
{
	uint8_t ensure;
	uint16_t num;
	printf("ɾ��ָ��\r\n");
	printf("������ָ��ID��Enter����\r\n");
	printf("0=< ID <=299\r\n");
	delay_ms(50);
//	num=GET_NUM();//��ȡ���ص���ֵ
	if(num==0xFFFF)
		goto MENU ; //������ҳ��
	else if(num==0xFF00)
		ensure=PS_Empty();//���ָ�ƿ�
	else 
		ensure=PS_DeletChar(num,1);//ɾ������ָ��
	if(ensure==0){
		printf("ɾ��ָ�Ƴɹ�\r\n");
	}else
		printf("\r\n%s\r\n",EnsureMessage(ensure));
	delay_ms(1200);
	PS_ValidTempleteNum(&ValidN);//����ָ�Ƹ���
	printf("ʣ������%d\r\n",AS608Para.PS_max-ValidN);
MENU:	
	delay_ms(50);
}



/* �߳�thread_Fingerprint_Detect_entry����ں��� */
void thread_Fingerprint_Detect_entry(void* parameter)
{
	uint8_t ensure;
	static uint8_t det_flag = 0;
	static uint8_t last_det_flag = 0;
    while(PS_HandShake(&AS608_Addr)){
        delay_ms(1000);
    }
    LEDOn();
    delay_ms(100);
    LEDOff();
    delay_ms(100);
    LEDOn();
    delay_ms(10);
    LEDOff();

	ensure = PS_ValidTempleteNum(&ValidN);  //����ָ�Ƹ���
	ensure = PS_ReadSysPara(&AS608Para); 	//������ 
	if(ensure == 0x00){
		printf("������:%d     �Աȵȼ�: %d",AS608Para.PS_max-ValidN,AS608Para.PS_level);
	}
	
	while (1)
	{
        det_flag = FingerDetect();
		if(last_det_flag != det_flag){
			last_det_flag = det_flag;

			if(det_flag == 1){
				press_FR();
			}
		}



        rt_thread_delay(10);
	}
}



