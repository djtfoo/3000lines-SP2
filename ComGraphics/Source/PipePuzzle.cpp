#include "PipePuzzle.h"
#include <iostream>
using std::cout;
using std::endl;


PipePuzzle::PipePuzzle()
{
};

PipePuzzle::~PipePuzzle()
{
};

void PipePuzzle::checkpuzzle()
{

};

bool PipePuzzle::confirmpuzzle(int row, int col, int orientation)
{
	if (puzzlechecked[row][col] == 3)
	{
		return false;
	}
	if (orientation == 1)
	{
		if (puzzlechecked[row][col] == 0)
		{
			puzzlechecked[row][col] = 1;
		}
		else if (puzzlechecked[row][col] == 1)
		{
			return false;
		}
		else if (puzzlechecked[row][col] == 2)
		{
			puzzlechecked[row][col] = 3;
		}
	}
	if (orientation == 2)
	{
		if (puzzlechecked[row][col] == 0)
		{
			puzzlechecked[row][col] = 2;
		}
		else if (puzzlechecked[row][col] == 1)
		{
			puzzlechecked[row][col] = 3;
		}
		else if (puzzlechecked[row][col] == 2)
		{
			return false;
		}
	}

	if ((row == 8) && (col == 5) && (orientation == 1))
	{
		return true;
	}
	if ((puzzlemap[row][col] == 0) || (puzzlemap[row][col] == 1))
	{
		bool splitsearch = false;
		if (orientation == 1)
		{
			if (col != 8)
			splitsearch = confirmpuzzle(row, col + 1, orientation);
			if (splitsearch == true)
			{
				return true;
			}
			if (col != 0)
			splitsearch = confirmpuzzle(row, col - 1, orientation);
			if (splitsearch == true)
			{
				return true;
			}
		}
		else
		{
			if (row != 8)
			splitsearch = confirmpuzzle(row + 1, col, orientation);
			if (splitsearch == true)
			{
				return true;
			}
			if (row != 0)
			splitsearch = confirmpuzzle(row - 1, col, orientation);
			if (splitsearch == true)
			{
				return true;
			}
		}
		return false;
	}
	if ((puzzlemap[row][col] == 2) || (puzzlemap[row][col] == 3) || (puzzlemap[row][col] == 4) || (puzzlemap[row][col] == 5))
	{
		bool splitsearch = false;
		if (orientation == 1)
		{
			if (row != 0)
			{
				splitsearch = confirmpuzzle(row - 1, col, 2);
				if (splitsearch == true)
				{
					return true;
				}
			}
			if (row != 8)
			{
				splitsearch = confirmpuzzle(row + 1, col, 2);
				if (splitsearch == true)
				{
					return true;
				}
			}
		}
		if (orientation == 2)
		{
			if (col != 0)
			{
				splitsearch = confirmpuzzle(row, col - 1, 1);
				if (splitsearch == true)
				{
					return true;
				}
			}
			if (col != 5)
			{
				splitsearch = confirmpuzzle(row, col + 1, 1);
				if (splitsearch == true)
				{
					return true;
				}
			}
		}
		return false;
	}
	if ((puzzlemap[row][col] == 6) || (puzzlemap[row][col] == 7) || (puzzlemap[row][col] == 8) || (puzzlemap[row][col] == 9) || (puzzlemap[row][col] == 10))
	{
		bool splitsearch = false;
		if (row != 0)
		{
			splitsearch = confirmpuzzle(row - 1, col, 2);
			if (splitsearch == true)
			{
				return true;
			}
		}
		if (row != 8)
		{
			splitsearch = confirmpuzzle(row + 1, col, 2);
			if (splitsearch == true)
			{
				return true;
			}
		}
		if (col != 0)
		{
			splitsearch = confirmpuzzle(row, col - 1, 1);
			if (splitsearch == true)
			{
				return true;
			}
		}
		if (col != 5)
		{
			splitsearch = confirmpuzzle(row, col + 1, 1);
			if (splitsearch == true)
			{
				return true;
			}
		}
		return false;
	}
	return false;
};

void PipePuzzle::setpuzzle()
{
	bool completedpuzzle = false;
	while (!completedpuzzle)
	{
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 6; j++)
			{
				puzzlemap[i][j] = rand() % 11;
				if (puzzlemap[i][j] < 10)
				{
					cout << "  ";
				}
				else
				{
					cout << " ";
				}
				cout << puzzlemap[i][j];
				puzzlechecked[i][j] = 0;
			}
			cout << endl;
		}
		cout << endl;
		completedpuzzle = confirmpuzzle(0, 0, 1);
	}

};