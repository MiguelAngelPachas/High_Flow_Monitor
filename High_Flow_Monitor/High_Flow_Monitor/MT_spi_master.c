/*
 * spi_master.c
 *
 *  Created on: Apr 13, 2018
 *      Author: mtorres
 */

/* Inclusiones */
#include <avr/io.h>
#include <stdint.h>
#include "project_defines.h"
#include "MT_spi_master.h"


/* Declaraci�n de variables globales */
volatile uint8_t spi_tx_buffer[SPI_TX_BUFF_SIZE];
volatile uint8_t spi_rx_buffer[SPI_RX_BUFF_SIZE];

/* Definici�n de funciones */


/****************************************************************************
* Nombre de la funci�n: SPI_Configurar
* retorna : Nada
* arg : spi_param
* Descripci�n : Recibe un puntero hacia una estructura de tipo spi_config_t,
*               la cual contiene los par�metros de configuraci�n) y configura
* 				el m�dulo TWI.
* Notas : Si el la interfaz SPI se configura en modo maestro, entonces esta
* 		  funci�n se encargar� de configurar el pin SS como salida, por lo
* 		  cual se debe tener cuidado de no conectar ninguna se�al a ese pin.
*****************************************************************************/
void SPI_Configurar(spi_config_t *spi_param){

	/* Configuraci�n de los pines */
	DDR_SPI &= ~(1 << MISO);
	DDR_SPI |= (1 << MOSI);
	DDR_SPI |= (1 << SCK);

	if(spi_param->mode == SPI_Master){
		DDR_SPI |= (1 << SS);
	}else{
		DDR_SPI &= ~(1 << SS);
	}

	/* Inicializaci�n de los registros de configuraci�n */
	SPCR = 0;
	SPSR = 0;

	/* Configurar la interrupci�n, el orden de los datos y el modo */
	SPCR |= spi_param->interrupt | spi_param->data_order | spi_param->mode;
	/* Configurar la polaridad y fase de la se�al de reloj */
	SPCR |= spi_param->cpha | spi_param->cpol;
	/* Configurar el pre-scaler de la se�al de reloj */
	SPCR |= spi_param->sck_prescaler & 0x03;
	SPSR |= (spi_param->sck_prescaler >> 2) & 1;

	/* Habilitar la interfaz SPI */
	SPCR |= (1 << SPE);

}


/****************************************************************************
* Nombre de la funci�n: SPI_Master_Tx_Byte
* retorna : el byte transmitido por el dispositivo esclavo
* arg : byte
* Descripci�n : Recibe como par�metro un dato de 8 bits y lo transmite por
* 				la l�nea MOSI del bus SPI. Al mismo tiempo, recibe un byte
* 				por la l�nea MISO y retorna su valor al salir de la funci�n.
* Notas : Si el dispositivo esclavo no transmite ning�n valor o no cuenta con
* 		  l�nea MISO, entonces el valor de retorno debe ignorarse.
*****************************************************************************/
uint8_t SPI_Master_Tx_Byte(uint8_t byte){

	uint8_t miso_data;

	/* Iniciar transferencia */
	SPDR = byte;
	/* Esperar a que la transferencia se complete */
	while(!(SPSR & (1<<SPIF))){
		// No hacer nada (blocking polling)
	}

	/* Almacenar el byte recibido */
	miso_data = SPDR0;

	return miso_data;
}


/****************************************************************************
* Nombre de la funci�n: SPI_Master_Tx_Bitstream
* retorna : nada
* arg : n_bytes
* arg: output_buffer
* arg: input_buffer
* Descripci�n : Transmite y recibe 'n' bytes de forma consecutiva. Todos los
* 				bytes que ser�n transmitidos son almacenados en el arreglo
* 				output_buffer. Por otro lado, todos los bytes recibidos ser�n
* 				almacenados en el arreglo input_buffer.
* Notas : A pesar que el dispositivo esclavo no transmita ning�n valor o no
* 		  cuente con l�nea MISO, debe definirse un arreglo que se pase como
* 		  par�metro input_buffer (puede usar el arreglo spi_rx_buffer).
*****************************************************************************/
void SPI_Master_Tx_Bitstream(uint8_t n_bytes, uint8_t *output_buffer, uint8_t *input_buffer){

	uint8_t indice;

	for(indice = 0; indice < n_bytes; indice++){
		/* Iniciar transferencia */
		SPDR = *(output_buffer + indice);
		/* Esperar a que la transferencia se complete */
		while(!(SPSR & (1<<SPIF))){
			// No hacer nada (blocking polling)
		}
		/* Almacenar el byte recibido en el buffer de recepci�n */
		*(input_buffer + indice) = SPDR0;
	}
}
