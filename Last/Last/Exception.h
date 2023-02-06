#pragma once
#include <string>

class Exception
{
public:
	Exception() { excpt = "Exception"; }
	Exception(std::string ex) { excpt = ex; }
	virtual std::string GetException() { return excpt; };

protected:
	std::string excpt;
};

class ArrayDataException : public Exception
{
public:
	ArrayDataException() { excpt = "ArrayDataException"; }
	ArrayDataException(std::string ex) : Exception(ex) {}
};

class ArraySizeException : public Exception
{
public:
	ArraySizeException() { excpt = "ArraySizeException"; }
	ArraySizeException(std::string ex) : Exception(ex) {}
};