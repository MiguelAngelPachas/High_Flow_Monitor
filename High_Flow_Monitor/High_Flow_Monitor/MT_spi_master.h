/*
 * spi_master.h
 *
 *  Created on: Apr 13, 2018
 *      Author: mtorres
 */

#ifndef MT_SPI_MASTER_H_
#define MT_SPI_MASTER_H_

/* Inclusiones */
#include <avr/io.h>
#include <stdint.h>
#include "project_defines.h"


/* Constantes y macros */

// Definici�n de pines
/*
#define DDR_SPI		DDRB
#define PORT_SPI	PORTB
#define SS			0
#define SCK			1
#define MOSI		2
#define MISO		3
*/

// Tama�o de los buffers de transmmisi�n y recepci�n
/*
#define SPI_TX_BUFF_SIZE	2
#define SPI_RX_BUFF_SIZE	2
*/

/* Definici�n de tipos y estructuras */

typedef enum{
	SPI_Slave = 0,
	SPI_Master = 16
}spi_mode_t;

typedef enum{
	CPOL_0 = 0,
	CPOL_1 = 8
}spi_clock_polarity_t;

typedef enum{
	CPHA_0 = 0,
	CPHA_1 = 4
}spi_clock_phase_t;

typedef enum{
	MSB_First = 0,
	LSB_First = 32
}spi_data_order_t;

typedef enum{
	SPI_Int_Enabled = 128,
	SPI_Int_Disabled = 0
}spi_interrupt_t;


typedef enum{
	SCK_Fosc_2 = 4,
	SCK_Fosc_4 = 0,
	SCK_Fosc_8 = 5,
	SCK_Fosc_16 = 1,
	SCK_Fosc_32 = 6,
	SCK_Fosc_64 = 2,
	SCK_Fosc_128 = 3
}spi_clock_source_t;


typedef struct{
	spi_mode_t mode;
	spi_clock_source_t sck_prescaler;
	spi_clock_polarity_t cpol;
	spi_clock_phase_t cpha;
	spi_data_order_t data_order;
	spi_interrupt_t interrupt;
}spi_config_t;


/* Declaraci�n de variables globales */
extern volatile uint8_t spi_tx_buffer[];
extern volatile uint8_t spi_rx_buffer[];

/* Declaraci�n de funciones (prototipos) */
void SPI_Configurar(spi_config_t *spi_param);
uint8_t SPI_Master_Tx_Byte(uint8_t byte);
void SPI_Master_Tx_Bitstream(uint8_t n_bytes, uint8_t *output_buffer, uint8_t *input_buffer);

#endif /* SPI_MASTER_H_ */
