/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   ����һ��[Ұ��]-STM32F103�Ե�LiteOS�������ʵ�飡
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� F103-�Ե� STM32 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 
 /* LiteOS ͷ�ļ� */
#include "los_sys.h"
#include "los_typedef.h"
#include "los_task.ph"
 /* �弶����ͷ�ļ� */
#include "stm32f10x.h"
#include "bsp_usart.h"
#include "bsp_led.h"
#include "bsp_key.h" 

/* ���������� */
UINT32 LED_Task_Handle;
UINT32 Key_Task_Handle;

/* �������� */
static UINT32 Creat_LED_Task(void);
static UINT32 Creat_Key_Task(void);
static void LED_Task(void);
static void Key_Task(void);
static void BSP_Init(void);


/***************************************************************************
  * @brief  ������
  * @param  ��
  * @retval ��
  * @note   ��һ����������Ӳ����ʼ�� 
			�ڶ���������APPӦ������
			������������LiteOS����ʼ��������ȣ�����ʧ�������������Ϣ
  **************************************************************************/
int main(void)
{	
	UINT32 uwRet = LOS_OK;  //����һ�����񴴽��ķ���ֵ��Ĭ��Ϊ�����ɹ�
	/* ������س�ʼ�� */
  BSP_Init();
	/* ����һ���ַ��� */
	printf("����һ��[Ұ��]-STM32F103�Ե�LiteOS�������ʵ�飡\n\n");
  printf("����K1�������񣬰���K2�ָ�����\n");
	/* LiteOS ���ĳ�ʼ�� */
	uwRet = LOS_KernelInit();
  if (uwRet != LOS_OK)
   {
	printf("LiteOS ���ĳ�ʼ��ʧ�ܣ�\n");
	return LOS_NOK;
   }
	/* ����LED_Task���񴴽�ʧ�� */
	uwRet = Creat_LED_Task();
  if (uwRet != LOS_OK)
   {
	printf("LED_Task���񴴽�ʧ�ܣ�\n");
       return LOS_NOK;
   }
	/* ����Key_Task���񴴽�ʧ�� */
	uwRet = Creat_Key_Task();
  if (uwRet != LOS_OK)
   {
	printf("Key_Task���񴴽�ʧ�ܣ�\n");
       return LOS_NOK;
   }
   /* ����LiteOS������� */
   LOS_Start();
}
/*********************************************************************************
  * @ ������  �� LED_Task
  * @ ����˵���� LED_Task����ʵ��
  * @ ����    ��   
  * @ ����ֵ  �� ��
  ********************************************************************************/
static void LED_Task(void)
{
  /* ������һ������ѭ�������ܷ��� */
	while(1)
	{
		LED2_TOGGLE;      //LED2��ת
		printf("LED_Task�����������У�\n");
		LOS_TaskDelay(1000);			
	}
}
/*********************************************************************************
  * @ ������  �� Key_Task
  * @ ����˵���� Key_Task����ʵ��
  * @ ����    ��   
  * @ ����ֵ  �� ��
  ********************************************************************************/
static void Key_Task(void)
{
	UINT32 uwRet = LOS_OK;				/* ����һ����������ķ������ͣ���ʼ��Ϊ�����ɹ��ķ���ֵ */
	/* ������һ������ѭ�������ܷ��� */
	while(1)
	{
		if( Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON )      /* K1 ������ */
		{
			printf("����LED1����\n");
			uwRet = LOS_TaskSuspend(LED_Task_Handle);/* ����LED1���� */
			if(LOS_OK == uwRet)
			{
				printf("����LED1����ɹ���\n");
			}
		}
		else if( Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN) == KEY_ON ) /* K2 ������ */
		{
			printf("�ָ�LED1����!\n");
			uwRet = LOS_TaskResume(LED_Task_Handle); /* �ָ�LED1���� */
			if(LOS_OK == uwRet)
			{
				printf("�ָ�LED1����ɹ���\n");
			}
			
		}
		
		LOS_TaskDelay(20);                                            /* 20msɨ��һ�� */
	}
}

/*********************************************************************************
  * @ ������  �� Creat_LED_Task
  * @ ����˵���� ����LED_Task����
  * @ ����    ��   
  * @ ����ֵ  �� ��
  ********************************************************************************/
static UINT32 Creat_LED_Task()
{
    UINT32 uwRet = LOS_OK;				/* ����һ����������ķ������ͣ���ʼ��Ϊ�����ɹ��ķ���ֵ */
    TSK_INIT_PARAM_S task_init_param;

    task_init_param.usTaskPrio = 4;								/* ���ȼ�����ֵԽС�����ȼ�Խ�� */
    task_init_param.pcName = "LED_Task";						/* ���������ַ�����ʽ��������� */
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)LED_Task;	/* �������� */
    task_init_param.uwStackSize = 0x1000;						/* ջ��С����λΪ�֣���4���ֽ� */

    uwRet = LOS_TaskCreate(&LED_Task_Handle, &task_init_param);/* �������� */
    return uwRet;
}
/*********************************************************************************
  * @ ������  �� Creat_Key_Task
  * @ ����˵���� ����Key_Task����
  * @ ����    ��   
  * @ ����ֵ  �� ��
  ********************************************************************************/
static UINT32 Creat_Key_Task()
{
    UINT32 uwRet = LOS_OK;				/* ����һ����������ķ������ͣ���ʼ��Ϊ�����ɹ��ķ���ֵ */
    TSK_INIT_PARAM_S task_init_param;

    task_init_param.usTaskPrio = 5;								/* ���ȼ�����ֵԽС�����ȼ�Խ�� */
    task_init_param.pcName = "Key_Task";						/* ���������ַ�����ʽ��������� */
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)Key_Task;	/* �������� */
    task_init_param.uwStackSize = 0x1000;						/* ջ��С����λΪ�֣���4���ֽ� */

    uwRet = LOS_TaskCreate(&Key_Task_Handle, &task_init_param);/* �������� */

    return uwRet;
}
/*********************************************************************************
  * @ ������  �� BSP_Init
  * @ ����˵���� �弶�����ʼ�������а����ϵĳ�ʼ�����ɷ��������������
  * @ ����    ��   
  * @ ����ֵ  �� ��
  ********************************************************************************/
static void BSP_Init(void)
{
	/*
	 * STM32�ж����ȼ�����Ϊ4����4bit��������ʾ��ռ���ȼ�����ΧΪ��0~15
	 * ���ȼ�����ֻ��Ҫ����һ�μ��ɣ��Ժ������������������Ҫ�õ��жϣ�
	 * ��ͳһ��������ȼ����飬ǧ��Ҫ�ٷ��飬�мɡ�
	 */
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
	
	/* LED ��ʼ�� */
	LED_GPIO_Config();
  
  /* ������ʼ�� */
	Key_GPIO_Config();
  
	/* ���ڳ�ʼ��	*/
	USART_Config();
  
	

}


/*********************************************END OF FILE**********************/

