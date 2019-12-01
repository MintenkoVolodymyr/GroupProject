/*
 * SPI.c
 *
 *  Created on: 1 ����. 2019 �.
 *      Author: pidho
 */


void RF_Deactivate(void) //�������������� RC522
{
	GPIO_SetBits(Port, NSS);
}

void RF_Activate(void) ///������������ RC522
{
	GPIO_ResetBits(Port, NSS);
}

void SPI_init(void) //������������� SPI
{
	GPIO_InitTypeDef SPI_port; //��������� ���������

        //CSK � MOSI ��� �������������� ������, ��� PUSH - PULL
	SPI_port.GPIO_Pin = SCK | MOSI;
	SPI_port.GPIO_Mode = GPIO_Mode_AF_PP;
	SPI_port.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Port, &SPI_port);

        //MISO - ����, �������� � �������
	SPI_port.GPIO_Pin = MISO;
	SPI_port.GPIO_Mode = GPIO_Mode_IPU;
	SPI_port.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Port, &SPI_port);

        //����� - P/P
	SPI_port.GPIO_Pin = NSS;
	SPI_port.GPIO_Mode = GPIO_Mode_Out_PP;
	SPI_port.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Port, &SPI_port);

	SPI_InitTypeDef SPI;
        //������ �������
	SPI.SPI_Direction 		  = SPI_Direction_2Lines_FullDuplex;
        //stm - ������
	SPI.SPI_Mode 			  = SPI_Mode_Master;
        //����� ������ - 8 ���
	SPI.SPI_DataSize 		  = SPI_DataSize_8b;
	SPI.SPI_CPOL 			  = SPI_CPOL_Low;//����������
	SPI.SPI_CPHA 			  = SPI_CPHA_1Edge;//����
	SPI.SPI_NSS 			  = SPI_NSS_Soft; //���������� NSS
	SPI.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;//�������� SPI
	SPI.SPI_FirstBit          = SPI_FirstBit_MSB; //�������� ������� ������� ���

	SPI_Init(SPIx, &SPI);//������������� SPI

	SPI_Cmd(SPIx, ENABLE); //���. SPI

	SPI_NSSInternalSoftwareConfig(SPIx, SPI_NSSInternalSoft_Set);//����, ��� �� stm ����, ��� �� ������

        //Reset - �� ������ ������
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin   = RST_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(PORT_RST, &GPIO_InitStructure);

	GPIO_WriteBit(PORT_RST, RST_Pin,Bit_SET);
	RF_Deactivate();
}

