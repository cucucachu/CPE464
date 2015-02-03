#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <arpa/inet.h>

#include "message.h"
#include "exceptions.h"

#define MIN_MESSAGE_SIZE 7

using namespace std;

Message::Message() {
	sequence_number = 0;
	flag = 0;
	to_length = 0;
	to = NULL;
	from_length = 0;
	from = NULL;
	text_length = 0;
	text = NULL;
	bytes = NULL;
}

Message::Message(const char *text) {
	sequence_number = 0;
	flag = 0;
	to_length = 0;
	to = NULL;
	from_length = 0;
	from = NULL;
	text_length = strlen(text);
	strcpy(this->text, text);
	bytes = NULL;

}

Message::Message(uint8_t *recieved, int length) {
	bytes = new uint8_t[length + 1];
	memcpy(bytes, recieved, length + 1);
	unpack();
}

Message::~Message() {
	if (bytes != NULL)
		delete[] bytes;
}

int Message::pack() {
	int total_size;
	
	if (to == NULL || from == NULL)
		throw PACK_EX;
		
	total_size = MIN_MESSAGE_SIZE;
	
	total_size += to_length + from_length + text_length;
	
	if (bytes != NULL)
		delete[] bytes;
	
	bytes = new uint8_t[total_size];
	
	bytes[0] = htonl(sequence_number);
	bytes[4] = flag;
	bytes[5] = to_length;
	
	if (to_length)
		memcpy(bytes + 6, to, to_length);
	
	bytes[6 + to_length] = from_length;
	
	if (from_length)
		memcpy(bytes + 6 + to_length + 1, from, from_length);
	
	memcpy(bytes + 6 + to_length + 1 + from_length, text, text_length);
	
	return total_size;
}

uint8_t *Message::sendable() {
	return bytes;
}

void Message::unpack() {
	if (bytes == NULL)
		throw UNPACK_EX;
		
	sequence_number = ntohl(bytes[0]);
	flag = bytes[4];
	to_length = bytes[5];
	memcpy(to, bytes + 6, to_length);
	from_length = bytes[6 + to_length];
	memcpy(from, bytes + 6 + to_length + 1, from_length);
	
}

/*
 * Setter methods
 */
void Message::set_sequence_number(uint32_t number) {
	sequence_number = number;
}

void Message::set_flag(uint8_t flag) {
	this->flag = flag;
}

void Message::set_to(char *to, int length) {
	if (this->to != NULL)
		delete[] this->to;
	
	to_length = length;
	this->to = new char[length + 1];
	memcpy(this->to, to, length + 1);
}
	
void Message::set_from(char *from, int length) {
	if (this->from != NULL)
		delete[] this->from;
	
	from_length = length;
	this->from = new char[length + 1];
	memcpy(this->from, from, length + 1);
}
		
void Message::set_text(char *text, int length) {
	if (this->text != NULL)
		delete[] this->text;
	
	text_length = length;
	this->text = new char[length + 1];
	memcpy(this->text, text, length + 1);
}

/*
 * Getter methods
 */
uint32_t Message::get_sequence_number() {
	return sequence_number;
}

uint8_t Message::get_flag() {
	return flag;
}

char *Message::get_to() {
	return to;
}

char *Message::get_from() {
	return from;
}

char *Message::get_text() {
	return text;
}

