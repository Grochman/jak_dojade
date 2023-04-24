#include "String.h"
#define BASE_LENGTH 16

String::String() {
	size = BASE_LENGTH;
	zajete = 0;
	array = new char[size];
	array[0] = '\0';
}
String::String(const String& other) : size(other.size), array{new char[other.size]}, zajete(other.zajete) {
	for (int i = 0; i <= zajete; i++)
		array[i] = other.array[i];
}

void String::Zeroj() {
	//size = BASE_LENGTH;
	zajete = 0;
	if (size > BASE_LENGTH) {
		delete[] array;
		size = BASE_LENGTH;
		array = new char[size];
	}
	//array = new char[size];
	array[0] = '\0';
}
void String::AddChar(char a){
	zajete++;
	if (zajete >= size) {
		size *= 2;
		char* temp = this->array;
		this->array = new char[size];
		memcpy(this->array, temp, size);
		delete[] temp;
	}
	this->array[zajete - 1] = a;
	this->array[zajete] = '\0';
}
char String::GetLast(int x) const{
	return array[zajete - x];
}
void String::DeleteLast() {
	array[zajete - 1] = '\0';
	zajete--;
}
int String::GetSize() const{
	return zajete;
}

std::ostream& operator<<(std::ostream& os, const String& string) {
	os << string.array;
	return os;
}
std::istream& operator>>(std::istream& is, String& string) {
	string.Zeroj();
	is >> string.array;
	for (int i = 0; i < string.size; i++) {
		if (string.array[i] != '\0') string.zajete++;
		else break;
	}
	return is;
}
bool String::operator ==(const char array[]) const{
	int i = 0;
	while (i <= zajete) {
		if (array[i] != this->array[i]) return false;
		i++;
	}
	return true;
}
bool String::operator==(const String& other) const{
	int i = 0;
	if (other.zajete != this->zajete) return false;
	while (i < zajete) {
		if (other.array[i] != this->array[i]) return false;
		i++;
	}
	return true;
}
String& String::operator=(const String& other) {
	/* z uzyciem swap
	String tmp = other;
	std::swap(array, tmp.array);
	std::swap(size, tmp.size);
	std::swap(zajete, tmp.zajete);
	*/
	delete[] array;
	this->array = new char[other.size];
	strcpy_s(this->array, other.size ,other.array);
	this->size = other.size;
	this->zajete = other.zajete;

	return *this;
}

String::~String() {
	if(array != nullptr)
		delete [] array;
}
