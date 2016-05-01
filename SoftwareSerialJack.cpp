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
  
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Jack.h>


//---PUBLIC---

//costruttore con la scelta della dimensione del buffer
SoftwareSerialJack::SoftwareSerialJack(int RX, int TX, long baudRate, size_t bufferSize) {
	
	//istanzio Software Serial
	_serial = new SoftwareSerial(RX, TX);
	_serial->begin(baudRate);

	//inizializzo il buffer
	bufferInitialize(bufferSize);
}


//costruttore senza la scelta della dimensione del buffer
SoftwareSerialJack::SoftwareSerialJack(int RX, int TX, long baudRate): SoftwareSerialJack(RX, TX, baudRate, SSJ_BUFFER_SIZE) {}


//distruttore
SoftwareSerialJack::~SoftwareSerialJack() {
		
	//elimino l'istanza di SoftwareSerial
	delete softwareSerial;

	//dsitruggo il buffer
	bufferDestroy();
	
}


//metodo di send
void SoftwareSerialJack::send(char *message, size_t length) { //invia il messaggio

	//invio il carattere di inzio messaggio
	_serial.print(SSJ_MESSAGE_START_CHARACTER);

	//invio il messaggio
	for (int i = 0; i < length; i++) {
		_serial.print(message[i]);
	}

	//invio il carattere di fine messaggio
	_serial.print(SSJ_MESSAGE_FINISH_CHARACTER);
	
}


//metodo per verificare se sono presenti messaggi nel buffer
uint8_t SoftwareSerialJack::available() { //restituisce true se ci sono dati da elaborare

	//finchè ci sono caratteri in entrata e posizioni libere nel buffer
    while (_serial->available() && bufferAvailable() ) {
		bufferPut(_serial.read());
    }

 	//restituisco la dimensione del buffer
 	return bufferLength();

}


//metodo che inserisce il messaggio in un buffer e restituisce la dimensione del messaggio
int SoftwareSerialJack::receive(char *buffer, size_t size) {

	int position = 0; //imposto la posizione all'interno del buffer di ritorno

	//elimino tutti i caratteri finchè non trovo il carattere di inzio messaggio
	while (bufferLength() && bufferGet() != SSJ_MESSAGE_START_CHARACTER) {}

	//se sono rimasti caratteri nel buffer
	if (bufferLength()) {

		//finchè ci sono caratteri nel buffer e non viene raggiunta la dimensione del buffer di ritorno o il carattere di fine
		//inserisco i caratteri nel buffer di ritorno

		while (bufferLength() && position < size && (buffer[position++] = bufferGet()) != SSJ_MESSAGE_FINISH_CHARACTER) {}

		//se è stato trovato il carattere di fine
		if (buffer[position -1] == SSJ_MESSAGE_FINISH_CHARACTER) {

			//inserisco il carattere di terminazione di stringa
			buffer[position -1] = 0;

			//ritorno la lunghezza del messaggio
			return position -1;
		
		//altrimenti
		} else {

			//inserisco il carattere di terminazione di stringa all'inzio perchè il messaggio non è valido
			buffer[0] = 0;

			return 0;
		}


	}
	
}


//---PRIVATE---

//---gestione del buffer circolare---

//pulisce il buffer
void SoftwareSerialJack::bufferInitialize(size_t size) {

	//creo il buffer
	_buffer = (char *) malloc(bufferSize * sizeof(char)); //alloco la memoria
	_buffer[0] = 0; //inserisco la fine del buffer

	//imposto la dimensione del buffer
	_size = size;
	//posizione di testa
	_position = 0;
	//numero di elementi
	_length = 0;

}

//inserisce il dato nel buffer (0 buffer pieno, 1= successo)
void SoftwareSerialJack::bufferPut(char c) {

	//se il buffer ha spazio disponibile
	if (_length < size) {
		_buffer[(_position + _length++) % _size] = c; //salvo il dato
	}

}

//restituisce il dato alla posizione corrente
char SoftwareSerialJack::bufferGet() {

	//se il buffer ha qualcosa di memorizzato
	if (_length > 0) {

		char c = _buffer[_position]; //recupero il dato memorizzato

		//aggiorno la posizione di testa
		_position = _position++ % _size;

		return c; //ritorno il dato salvato
	}

}

//restituisce il numero di posizioni libere
int SoftwareSerialJack::bufferAvailable() {
	return _size - _length;
}

//restituisce il numero di elementi memorizzati nel buffer
int SoftwareSerialJack::bufferLength() {
	return _length;
}

//distrugge il buffer e libera la memoria
void SoftwareSerialJack::bufferDestroy() {

	//libero la memoria allocata dal buffer
	free(_buffer);
}

