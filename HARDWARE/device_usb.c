/*
********************************************************************************
*                            �й���������������ϵͳ
*                            Thread Operating System
*
*                              ��оƬ:STM32F401re
*
*                               ��ֲ/�ü�������־
********************************************************************************
*�ļ���     : device_usb.c
*����       : USBVCP�豸
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Guolz         2016/9/14        ��ʼ�汾
********************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "system.h"

#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_cdc.h"
#include "usbd_cdc_if.h"

/* USB Device Core handle declaration */
USBD_HandleTypeDef hUsbDeviceFS;

#ifdef USB_VCP

void USB_Vcp_Init(void)
{
  /* Init Device Library,Add Supported Class and Start the library*/
  USBD_Init(&hUsbDeviceFS, &FS_Desc, DEVICE_FS);

  USBD_RegisterClass(&hUsbDeviceFS, &USBD_CDC);

  USBD_CDC_RegisterInterface(&hUsbDeviceFS, &USBD_Interface_fops_FS);

  USBD_Start(&hUsbDeviceFS);
}



void UsbProcess(uint8_t data)
{
    static uint8_t m = 0 ;
    switch(data)
    {
      case 0xff:
         break;
      case 0xfe:            
         break;
      case 0xfa:           

         break;
      case 0xf0:

        break;
    }
}

#endif       //ifdef USB_VCP
