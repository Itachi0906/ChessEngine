#include "Position.h"

void Position::instantiate(int r, int c)
{
	rank = r;
	file = c;
}

int Position::getRank()
{
	return rank;
}

int Position::getFile()
{
	return file;
}
