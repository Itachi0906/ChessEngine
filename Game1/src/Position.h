#pragma once
class Position
{
private:
	int rank;
	int file;

public:
	void instantiate(int r, int c);
	Position()
	{
		rank = 0;
		file = 0;
	}
	int getRank();
	int getFile();
};

