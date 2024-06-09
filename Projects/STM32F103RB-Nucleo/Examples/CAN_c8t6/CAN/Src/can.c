#include "can.h"

#include "stdio.h"

typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* ?????????? */
__IO uint32_t ret = 0;

volatile TestStatus TestRx;	

/*CAN RX0 ???????  */
 void CAN_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

  	/* Configure the NVIC Preemption Priority Bits */  
  	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	#ifdef  VECT_TAB_RAM  
	  /* Set the Vector Table base location at 0x20000000 */ 
	  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
	#else  /* VECT_TAB_FLASH  */
	  /* Set the Vector Table base location at 0x08000000 */ 
	  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
	#endif

	/* enabling interrupt */
  	NVIC_InitStructure.NVIC_IRQChannel=USB_LP_CAN1_RX0_IRQn;;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
}

/*CAN GPIO ????? */
 void CAN_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure; 
  /* ?????GPIOB??????*/	 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE);	                        											 

  /* CAN1 ?????? */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE); 

  /* Configure CAN pin: RX */	 // PB8
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 // ????
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  /* Configure CAN pin: TX */   // PB9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // ??????
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
	//#define GPIO_Remap_CAN    GPIO_Remap1_CAN1 ??????????I/O
  GPIO_PinRemapConfig(GPIO_Remap1_CAN1, ENABLE);
}

/*	CAN??? */
 void CAN_INIT(void)
{
  CAN_InitTypeDef        CAN_InitStructure;
  CAN_FilterInitTypeDef  CAN_FilterInitStructure;
  CanTxMsg TxMessage;  

  /* CAN register init */
  CAN_DeInit(CAN1);	//???CAN????????????
  CAN_StructInit(&CAN_InitStructure);//?CAN_InitStruct?????????????

  /* CAN cell init */
  CAN_InitStructure.CAN_TTCM=DISABLE;//??????????
  CAN_InitStructure.CAN_ABOM=DISABLE;//??????????
  CAN_InitStructure.CAN_AWUM=DISABLE;//??????????
  CAN_InitStructure.CAN_NART=DISABLE;//???????????
  CAN_InitStructure.CAN_RFLM=DISABLE;//??????FIFO????
  CAN_InitStructure.CAN_TXFP=DISABLE;//??????FIFO???
  CAN_InitStructure.CAN_Mode=CAN_Mode_Normal;//CAN???????
  CAN_InitStructure.CAN_SJW=CAN_SJW_1tq; //????????1?????
  CAN_InitStructure.CAN_BS1=CAN_BS1_3tq; //???1?3?????
  CAN_InitStructure.CAN_BS2=CAN_BS2_2tq; //???2?2?????
  CAN_InitStructure.CAN_Prescaler=60;  //???????60	
  CAN_Init(CAN1,&CAN_InitStructure);//????:72M/2/60(1+3+2)=0.1 ?100K

  /* CAN filter init */
  CAN_FilterInitStructure.CAN_FilterNumber=1;//??????1
  CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;//??????????????
  CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;//??????32?
  CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;// ????????16??
  CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;//	 ????????16??
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//??????????16??
  CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;//	??????????16??
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;// ?????????FIFO?0
  CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;// ?????
  CAN_FilterInit(&CAN_FilterInitStructure);//	????????????

  /* CAN FIFO0 message pending interrupt enable */ 
 CAN_ITConfig(CAN1,CAN_IT_FMP0, ENABLE); //??FIFO0??????
 
 }  
/* ?????????*/
void can_tx(u8 Data1,u8 Data2)
{ 
  CanTxMsg TxMessage;  

  TxMessage.StdId=0x00;	//??????0x00
  TxMessage.ExtId=0x0000; //?????0x0000
  TxMessage.IDE=CAN_ID_EXT;//???????
  TxMessage.RTR=CAN_RTR_DATA;//????
  TxMessage.DLC=2;	//	????????2???
  TxMessage.Data[0]=Data1; //???????
  TxMessage.Data[1]=Data2; //??????? 
  CAN_Transmit(CAN1,&TxMessage); //????
  
 
}

void Can_TxByte(u8 *Data,u8 len)
{ 
	int ix=0;
  CanTxMsg TxMessage;  

  TxMessage.StdId=0x00;	//??????0x00
  TxMessage.ExtId=0x0000; //?????0x0000
  TxMessage.IDE=CAN_ID_EXT;//???????
  TxMessage.RTR=CAN_RTR_DATA;//????
  TxMessage.DLC=len;	//	????????2???
//  TxMessage.Data[0]=Data1; //???????
//  TxMessage.Data[1]=Data2; //??????? 
	for(ix=0;ix<len;ix++)
	TxMessage.Data[ix]=Data[ix];
  CAN_Transmit(CAN1,&TxMessage); //????
  
 
}

/* USB???CAN????????,USB?CAN??I/O,?????CAN???? */
void USB_LP_CAN1_RX0_IRQHandler(void)
{
  int ix=0;
  CanRxMsg RxMessage;

  RxMessage.StdId=0x00;
  RxMessage.ExtId=0x00;
  RxMessage.IDE=0;
  RxMessage.DLC=0;
  RxMessage.FMI=0;
	
  RxMessage.Data[0]=0x00;
  RxMessage.Data[1]=0x00;    
  for(ix=0;ix<8;ix++)
	 RxMessage.Data[ix]=0;			  
  CAN_Receive(CAN1,CAN_FIFO0, &RxMessage); //??FIFO0????  
 
  if((RxMessage.Data[0]==0x99)&&(RxMessage.Data[1]==0xbb))   
  { LED1(0);LED2(1);}
   if((RxMessage.Data[0]==0x55)&&(RxMessage.Data[1]==0x77))   
  { LED1(1);LED2(0);}
   
  
} 
