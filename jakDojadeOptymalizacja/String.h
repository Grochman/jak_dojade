#pragma once
#include <iostream>
class String
{
	char* array;
	int size;
	int zajete;
public:
	String();
	String(const String& other);

	char GetLast(int x) const;
	void DeleteLast();
	int GetSize() const;
	void Zeroj();
	void AddChar(char a);
	friend std::ostream& operator<<(std::ostream& os, const String& string);
	friend std::istream& operator>>(std::istream& is, String& string);
	String& operator=(const String& other);
	bool operator==(const char string[]) const;
	bool operator==(const String& other) const;
	
	~String();
};

