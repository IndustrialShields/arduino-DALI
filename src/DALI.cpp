/*
   Copyright (c) 2019 Boot&Work Corp., S.L. All rights reserved

   This library is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "DALI.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
DALI::DALI(int txPin, int rxPin) : txPin(txPin), rxPin(rxPin)
{

}

////////////////////////////////////////////////////////////////////////////////////////////////////
void DALI::begin()
{
	pinMode(txPin, OUTPUT);
	pinMode(rxPin, INPUT);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void DALI::sendZero(void)
{
	digitalWrite(txPin, LOW);
	delayMicroseconds(416);
	digitalWrite(txPin, HIGH);
	delayMicroseconds(416);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void DALI::sendOne(void)
{
	digitalWrite(txPin, HIGH);
	delayMicroseconds(416);
	digitalWrite(txPin, LOW);
	delayMicroseconds(416);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void DALI::sendCommand(uint8_t command, uint8_t data)
{
	uint16_t info = (uint16_t)((command << 8) | data);

	// Enviamos el bit de start
	sendOne();

	for ( uint8_t i = 0 ; i < 16 ; i++ )
	{
		if (info&0x8000)
			sendOne();
		else
			sendZero();
		info <<= 1;
	}
	digitalWrite(txPin, LOW);
	delayMicroseconds(1700);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
bool DALI::sendSearchAddr(uint32_t addr)
{
	// Enviamos la address high
	sendCommand(SEARCHADDRH, (addr >> 16) & 0xFF);
	delay(20);
	// Enviamos la address mid
	sendCommand(SEARCHADDRM, (addr >> 8) & 0xFF);
	delay(20);
	// Enviamos la address low
	sendCommand(SEARCHADDRL, addr & 0xFF);
	delay(20);

	// Enviamos un compare
	sendCommand(COMPARE, 0);

	// Esperamos una respuesta
	for ( uint32_t n = 0 ; n < 50000 ; n++ )
	{
		if (digitalRead(rxPin))
		{
			delay(20);
			return true;
		}
		delayMicroseconds(1);
	}
	delay(20);
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void DALI::withdrawNode(uint32_t addr)
{
	// Enviamos la address high
	sendCommand(SEARCHADDRH, (addr >> 16) & 0xFF);
	delay(20);
	// Enviamos la address mid
	sendCommand(SEARCHADDRM, (addr >> 8) & 0xFF);
	delay(20);
	// Enviamos la address low
	sendCommand(SEARCHADDRL, addr & 0xFF);
	delay(20);

	// Enviamos un compare
	sendCommand(WITHDRAW, 0);
	delay(20);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
int DALI::searchNodes(uint32_t* addresses, uint8_t numAddresses)
{
	uint32_t searchAddr;
	uint32_t searchLower;
	uint32_t searchDifference;

	int ret = 0;
	while(1)
	{
		searchAddr = 0xFFFFFF;
		// Mientras encontremos que alguien nos responde
		while (sendSearchAddr(searchAddr))
		{
			// Si hay algún nodo que tiene el numero 0, no lo aceptaremos
			if (searchAddr == 0)
			{
				return ret;
			}
			searchAddr >>= 1;
			searchAddr &= 0xFFFFFF;
		}

		// Si detectamos que ya no nos responde nadie, acabar la función
		if (searchAddr == 0xFFFFFF)
		{
			break;
		}

		// A partir de ahora tenemos que buscar entre el numero actual
		// y el numero anterior
		searchLower = searchAddr;
		searchDifference = (searchAddr << 1) - searchLower;
		while( searchDifference != 0)
		{
			while (sendSearchAddr(searchLower+searchDifference))
			{
				searchDifference >>= 1;
			}
			searchLower = searchLower+searchDifference;
			searchDifference <<= 1;
		}
		withdrawNode(searchLower+1);
		*addresses++ = searchLower+1;
		ret++;

		if (ret == numAddresses) {
			break;
		}
	}

	return ret;
}

// Función que sirve para que un nodo se ponga a una potencia concreta
// - Requiere una variable del tipo uint8_t con la short address del nodo
// - Requiere una variable del tipo uint8_t con la potencia que se le
// quiere aplicar al nodo
// Tener en mente que el numero maximo de "power" és 254
void DALI::setValue(uint8_t nodeNumber, uint8_t value)
{
	if (value > 254) {
		return;
	}

	sendCommand(SHORT_POWER | (nodeNumber << 1) , value);
	delay(20);
}

// Función que sirve para programar el nodo con una short addr
// - Necesita una variable del tipo uint8_t con la dirección del nodo
// -Devuelve TRUE si se ha programado correctamente el nodo. FALSO en caso contrario
// Recordar que "nodeNumber" puede ser como maximo el numero 63 (decimal)
bool DALI::sendProgramShortAddr(uint8_t nodeNumber)
{
	// Enviamos la orden de programar la short address seleccionada
	sendCommand(PROGRAM_SHORT_ADDRESS, 1 | (nodeNumber << 1));
	delay(20);

	// Enviamos la orden de que nos responda la short address que acabamos de programar
	sendCommand(VERIFY_SHORT_ADDRESS, 1 | (nodeNumber << 1));

	// Esperamos una respuesta
	for ( uint32_t n = 0 ; n < 50000 ; n++ )
	{
		if (digitalRead(DALI_RX))
		{
			delay(20);
			return true;
		}
		delayMicroseconds(1);
	}
	delay(20);
	return false;
}

// Función de inicializar los nodos del bus DALI
// Esta función REINICIA todo el bus DALI. Tener en mente por si se ejecuta en
// un bus donde ya se encuentren dispositivos configurados
void DALI::initNodes(void)
{
	// Reinicia los modulos
	sendCommand(COMMAND_BROADCAST, RESET);
	delay(10);
	sendCommand(COMMAND_BROADCAST, RESET);
	delay(300);

	// Termina con todos los nodos que puedan estar en configuración
	sendCommand(TERMINATE, 0);
	delay(100);

	// Reinicia los modulos
	sendCommand(INITIALISE, 0);
	delay(10);
	sendCommand(INITIALISE, 0);
	delay(200);

	// Pone a una dirección aleatoria los nodos
	sendCommand(RANDOMISE, 0);
	delay(10);
	sendCommand(RANDOMISE, 0);
	delay(200);
}
