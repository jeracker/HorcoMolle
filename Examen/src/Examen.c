/* Copyright 2015, Eduardo Filomena, Juan Manuel Reta
 * All rights reserved.
 *
 * This file is part of CIAA Firmware.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/** \brief Blinking Bare Metal example source file
 **
 ** This is a mini example of the CIAA Firmware.
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */

/** \addtogroup Examples CIAA Firmware Examples
 ** @{ */
/** \addtogroup Baremetal Bare Metal example source file
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 *
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * yyyymmdd v0.0.1 initials initial version
 */

/*==================[inclusions]=============================================*/
#include "Examen.h"       /* <= own header */

#ifndef CPU
#error CPU shall be defined
#endif
#if (lpc4337 == CPU)
#include "chip.h"
#elif (mk60fx512vlq15 == CPU)
#else
#endif


/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/
static int Coeficiente = 100;
static int Incremento = 10;
static unsigned char strAumento[]="1 - Aumento la ganancia\r\n";
static unsigned char strDisminucion[]="2 - Disminuyo la ganancia\r\n";
static unsigned char strMute[]="3 - Mute\r\n";
static unsigned char strSatura[]="4 - Satura\r\n";
static unsigned char strBlanco[]="\r\n";


/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
/** \brief Main function
 *
 * This is the main entry point of the software.
 *
 * \returns 0
 *
 * \remarks This function never returns. Return value is only to avoid compiler
 *          warnings or errors.
 */



int main(void)
{
   /* perform the needed initialization here */
	int led;
	InicializarLeds();
	InicializarTeclas();
	InicializarTimer();
	InicializarDAC();
	InicializarADC();
	UART_Init();

	for (led=0;led<=5;led++){
		ApagarLed(led);};
	/******		Menu	 ****/
	EnviaAConsola(strBlanco);
	EnviaAConsola(strBlanco);
	EnviaAConsola(strBlanco);
	EnviaAConsola(strAumento);
	EnviaAConsola(strDisminucion);
	EnviaAConsola(strMute);
	EnviaAConsola(strSatura);
	EnviaAConsola(strBlanco);
	EnviaAConsola(strBlanco);
	EnviaAConsola(strBlanco);

	/*for (led=0;led<=5;led++){
	ParpadearLed(led,2,1000000);};*/

	ConfigurarFrecuenciaTimer(100);
	HabilitarTimer();

    return 0;
}

void ServicioIRQ_RIT(void){
	uint16_t datos;
	int tecla;
	LimpiarBitInterrupcionTimer();

	datos=LeerCanalADC(1);
	InvertirLed(2);

	tecla=EscanearTeclado();

	switch (tecla){
	case 0: break;
	case 1: Coeficiente = Coeficiente + Incremento;
			EnviaAConsola(strAumento);
			break;
	case 2: Coeficiente = Coeficiente - Incremento;
			EnviaAConsola(strDisminucion);
			break;
	case 3: Coeficiente=0;
			EnviaAConsola(strMute);
			break;
	case 4: Coeficiente= CalculaCoeficienteSaturacion(datos);
			EnviaAConsola(strSatura);
			break;
	default: break;}

	datos = ModificaDatos(datos, Coeficiente);
	ConfigurarSalidaDAC(datos);

}

int ModificaDatos(int datos, int coeficiente){
	int resultado;
	resultado = datos * coeficiente / 100;
	if (resultado>1023){resultado = 1023;};
	return resultado;
}

void EnviaAConsola(char *cadena){
	int i;
	i = 0;
	while (cadena[i] != 0){
		UART_Send(cadena[i]);
		i++;
	}

}

int CalculaCoeficienteSaturacion(int datos){
	int coeficiente;
	coeficiente = (1023*100/2);
	coeficiente++;
	return coeficiente;
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

