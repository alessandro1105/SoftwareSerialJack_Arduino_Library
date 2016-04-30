/*
   Copyright 2016 Alessandro Pasqualini

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
   
   @author         Alessandro Pasqualini <alessandro.pasqualini.1105@gmail.com>
   @url            https://github.com/alessandro1105
*/

#ifndef __SOFTWARE_SERIAL_JACK_H__
#define __SOFTWARE_SERIAL_JACK_H__

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Jack.h>

//---COSTANTI---
#define MESSAGE_START_CHARACTER '<' //carattere di inzio messaggio
#define MESSAGE_FINISH_CHARACTER '>' //carattere di fine messaggio


class SoftwareSerialJack : public JTrasmissionMethod {

	public:
	
		SoftwareSerialJack(int RX, int TX, long baudRate);
		~SoftwareSerialJack();
		
		String receive(); //deve restituire il messaggio da passare a Jack
		void send(String message); //invia il messaggio
		
		uint8_t available(); //restituisce true se ci sono dati da elaborare


	private:
		
		SoftwareSerial *softwareSerial;
		String messageBuffer;

};


#endif