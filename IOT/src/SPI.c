/*
 * SPI.c
 *
 *  Created on: 1 груд. 2019 р.
 *      Author: pidho
 */


void RF_Deactivate(void) //Деактивировать RC522
{
	GPIO_SetBits(Port, NSS);
}

void RF_Activate(void) ///Активировать RC522
{
	GPIO_ResetBits(Port, NSS);
}

void SPI_init(void) //Инициализация SPI
{
	GPIO_InitTypeDef SPI_port; //объявляем структуру

        //CSK и MOSI как альтернативная фунция, тип PUSH - PULL
	SPI_port.GPIO_Pin = SCK | MOSI;
	SPI_port.GPIO_Mode = GPIO_Mode_AF_PP;
	SPI_port.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Port, &SPI_port);

        //MISO - вход, подтяжка к питанию
	SPI_port.GPIO_Pin = MISO;
	SPI_port.GPIO_Mode = GPIO_Mode_IPU;
	SPI_port.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Port, &SPI_port);

        //Выход - P/P
	SPI_port.GPIO_Pin = NSS;
	SPI_port.GPIO_Mode = GPIO_Mode_Out_PP;
	SPI_port.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Port, &SPI_port);

	SPI_InitTypeDef SPI;
        //полный дуплекс
	SPI.SPI_Direction 		  = SPI_Direction_2Lines_FullDuplex;
        //stm - мастер
	SPI.SPI_Mode 			  = SPI_Mode_Master;
        //длина пакета - 8 бит
	SPI.SPI_DataSize 		  = SPI_DataSize_8b;
	SPI.SPI_CPOL 			  = SPI_CPOL_Low;//Полярность
	SPI.SPI_CPHA 			  = SPI_CPHA_1Edge;//Фаза
	SPI.SPI_NSS 			  = SPI_NSS_Soft; //Управление NSS
	SPI.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;//Скорость SPI
	SPI.SPI_FirstBit          = SPI_FirstBit_MSB; //Передаем сначала старший бит

	SPI_Init(SPIx, &SPI);//инициализация SPI

	SPI_Cmd(SPIx, ENABLE); //Вкл. SPI

	SPI_NSSInternalSoftwareConfig(SPIx, SPI_NSSInternalSoft_Set);//надо, что бы stm знал, что он мастер

        //Reset - на всякий случай
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin   = RST_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(PORT_RST, &GPIO_InitStructure);

	GPIO_WriteBit(PORT_RST, RST_Pin,Bit_SET);
	RF_Deactivate();
}

