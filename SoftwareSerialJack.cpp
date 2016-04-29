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
   
#include "SoftwareSerialJack.h"

//SOFTWARE SERIAL JACK

char SoftwareSerialJack::MESSAGE_START_CHARACTER = '<'; //carattere inzio messaggio
char SoftwareSerialJack::MESSAGE_FINISH_CHARACTER = '>'; //carattere fine messaggio

SoftwareSerialJack::SoftwareSerialJack(int RX, int TX, long baudRate) {
	
	messageBuffer = "";
	
	softwareSerial = new SoftwareSerial(RX, TX);
	
	softwareSerial->begin(baudRate);
	
	/*
	//svuoto il buffer
	Serial.println("Svuoto buffer");
	
	String buffer = "";
	int time = millis();
	
	Serial.println(millis());
	
	while (millis() - time < 20)  {
		while (softwareSerial->available() > 0) {
			buffer += char(softwareSerial->read());
		}
	}
	
	Serial.println(millis());
	
	Serial.println("Contenuto del buffer: ");
	Serial.println(buffer);
	Serial.println("");
	*/
}

SoftwareSerialJack::~SoftwareSerialJack() {
	
	delete softwareSerial;
	
}

void SoftwareSerialJack::send(String message) { //invia il messaggio

	String messageToSend = String(MESSAGE_START_CHARACTER);
	
	messageToSend += message;
	
	messageToSend += String(MESSAGE_FINISH_CHARACTER);
	
	Serial.println(messageToSend);
	
	softwareSerial->print(messageToSend);
	
	//softwareSerial->print("<{\"id\":123456789,\"values\":{[\"GSR\":2200,\"TIMESTAMP\":123456789,\"TEMPERATURE\":38.5]}}>");
	
}

int SoftwareSerialJack::available() { //restituisce true se ci sono dati da elaborare

	////Serial.print("new: ");
    while (softwareSerial->available() > 0) { //scarico i caratteri ricevuti nel buffer
	
		//char c = char(softwareSerial->read());
	
		messageBuffer += char(softwareSerial->read());
	  
		//Serial.println(messageBuffer);
    }

	//Serial.print("		time: ");
	//Serial.println(millis());
	
	return 1;
	
}

String SoftwareSerialJack::receive() { //deve restituire il messaggio da passare a Jack
		
	String message = ""; //variabile che conterra il messaggio da restituire
		
	int nCharIncorrect = 0; //caratteri incorretti all'inizio
	int nCharMessage = 0; //caratteri del mesaggio
	
	//if (messageBuffer.length() > 0) //Serial.println(messageBuffer);
			
	//controllo che non ci siani caratteri non validi prima del messaggio (mi fermo quando trovo il char di inizio)
	for(int i = 0; i < messageBuffer.length() && messageBuffer.charAt(i) != MESSAGE_START_CHARACTER; i++) {
				
		nCharIncorrect++;
			
	}					
	
	messageBuffer = messageBuffer.substring(nCharIncorrect);
	
	
	////Serial.println(nCharIncorrect);
	
	//Serial.print("test initchar");
	
	if (messageBuffer.length() > 0 && messageBuffer.charAt(0) == MESSAGE_START_CHARACTER) { //messaggio con almeno 1 carattere e primo carattere è il carattere di inizio messaggio
				
		for (int i = 1; i < messageBuffer.length() && messageBuffer.charAt(i) != MESSAGE_FINISH_CHARACTER; i++) {
			nCharMessage++;
				
			message += messageBuffer.charAt(i);
				
		}
				
				
		if ((nCharMessage + 2) <= messageBuffer.length() && messageBuffer.charAt(nCharMessage + 2 - 1) == MESSAGE_FINISH_CHARACTER) {
					
			//E' presente un messaggio
			messageBuffer = messageBuffer.substring(nCharMessage + 2); //elimino dal bufffer il messaggio
					
		} else {
					
			message = ""; //non è presente un messaggio azzero il messaggio da restituire
					
		}
				
	} //fine prelievo messaggio
	
	
	//Serial.println(messageBuffer);
	
	//Serial.println("message ricevuto MMJTM: " + message);
		
	return message; //restituisco il messaggio o stringa vuota}
	
	//return "";

}