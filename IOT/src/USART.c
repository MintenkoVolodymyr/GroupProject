/*
 * USART.c
 *
 *  Created on: 1 груд. 2019 р.
 *      Author: pidho
 */

void USART_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

        //Настраиваем порты  Rx и Tx
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

        //Параметры USART(скорость, длина данных, кол.стоп битов, четность, управление)
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
        //Вкл. USART
	USART_Cmd(USART1, ENABLE);
        //Разрешаем прерывания от USART
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
        //Прерывание при приеме
	USART_ITConfig(USART_DEBUG, USART_IT_RXNE, ENABLE);
}

//Обработчик прерываний от USART
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART_DEBUG, USART_IT_RXNE))//если, что то пришло отправляем его по SPI
	{
           USART_ClearITPendingBit(USART_DEBUG, USART_IT_RXNE);
           char cc = (char)USART_ReceiveData(USART_DEBUG);
           printf("adds = 0x%x ", cc);
           char value = Read_MFRC522(cc);  //сразу же читаем, ответ по SPI
           printf("value = 0x%x \r\n", value);
 	}
}
