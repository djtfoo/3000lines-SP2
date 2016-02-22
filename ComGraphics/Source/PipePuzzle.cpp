#include "PipePuzzle.h"
#include <iostream>
using std::cout;
using std::endl;


PipePuzzle::PipePuzzle()
{
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 6; ++j) {
            puzzlerotation[i][j] = 0.f;
        }
    }
}

PipePuzzle::~PipePuzzle()
{
}

bool PipePuzzle::checkpuzzle(int row, int col, int orientation)
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

	if ((row == 8) && (col == 5))
	{
		if ((((puzzlemap[row][col] == 0) || (puzzlemap[row][col] == 9)) && (orientation == 1)) || (((puzzlemap[row][col] == 3) || (puzzlemap[row][col] == 7)) && (orientation == 2)) || (puzzlemap[row][col] == 8) || (puzzlemap[row][col] == 10))
		{
			return true;
		}
		return false;
	}
	if (puzzlemap[row][col] == 0)
	{
		if (orientation == 2)
		{
			return false;
		}
		bool splitcheck = false;
		if (row != 0)
		{
			splitcheck =  checkpuzzle(row - 1, col, 1);
			if (splitcheck == true)
			{
				return true;
			}
		}
		if (row != 8)
		{
			splitcheck = checkpuzzle(row + 1, col, 1);
			if (splitcheck == true)
			{
				return true;
			}
		}
		return false;
	}
	if (puzzlemap[row][col] == 1)
	{
		if (orientation == 1)
		{
			return false;
		}
		bool splitcheck = false;
		if (col != 0)
		{
			splitcheck = checkpuzzle(row, col - 1, 2);
			if (splitcheck == true)
			{
				return true;
			}
		}
		if (col != 5)
		{
			splitcheck = checkpuzzle(row, col + 1, 2);
			if (splitcheck == true)
			{
				return true;
			}
		}
		return false;
	}
	if (puzzlemap[row][col] == 2)
	{
		bool splitcheck = false;
		if (row != 0)
		{
			int blockage = puzzlemap[row - 1][col];
			if ((blockage != 2) || (blockage != 4) || (blockage != 6))
			{
				splitcheck = checkpuzzle(row - 1, col, 1);
				if (splitcheck == true)
				{
					return true;
				}
			}
		}
		if (col != 5)
		{
			int blockage = puzzlemap[row][col + 1];
			if ((blockage != 2) || (blockage != 5) || (blockage != 9))
			{
				splitcheck = checkpuzzle(row, col + 1, 2);
				if (splitcheck == true)
				{
					return true;
				}
			}
		}
		return false;
	}
	if (puzzlemap[row][col] == 3)
	{
		bool splitcheck = false;
		if (col != 0)
		{
			int blockage = puzzlemap[row][col - 1];
			if ((blockage != 3) || (blockage != 4) || (blockage != 8))
			{
				splitcheck = checkpuzzle(row, col - 1, 2);
				if (splitcheck == true)
				{
					return true;
				}
			}
		}
		if (row != 8)
		{
			int blockage = puzzlemap[row + 1][col];
			if ((blockage != 3) || (blockage != 5) || (blockage != 7))
			{
				splitcheck = checkpuzzle(row + 1, col, 1);
				if (splitcheck == true)
				{
					return true;
				}
			}
			
		}
		return false;
	}
	if (puzzlemap[row][col] == 4)
	{
		bool splitcheck = false;
		if (col != 0)
		{
			int blockage = puzzlemap[row][col - 1];
			if ((blockage != 3) || (blockage != 4) || (blockage != 8))
			{
				splitcheck = checkpuzzle(row, col - 1, 2);
				if (splitcheck == true)
				{
					return true;
				}
			}
		}
		if (row != 0)
		{
			int blockage = puzzlemap[row - 1][col];
			if ((blockage != 2) || (blockage != 4) || (blockage != 6))
			{
				splitcheck = checkpuzzle(row - 1, col, 1);
				if (splitcheck == true)
				{
					return true;
				}
			}
		}
		return false;
	}
	if (puzzlemap[row][col] == 5)
	{
		bool splitcheck = false;
		if (row != 8)
		{
			int blockage = puzzlemap[row + 1][col];
			if ((blockage != 3) || (blockage != 5) || (blockage != 7))
			{
				splitcheck = checkpuzzle(row + 1, col, 1);
				if (splitcheck == true)
				{
					return true;
				}
			}
		}
		if (col != 5)
		{
			int blockage = puzzlemap[row][col + 1];
			if ((blockage != 2) || (blockage != 5) || (blockage != 9))
			{
				splitcheck = checkpuzzle(row, col + 1, 2);
				if (splitcheck == true)
				{
					return true;
				}
			}
		}
		return false;
	}
	if (puzzlemap[row][col] == 6)
	{
		bool splitcheck = false;
		if (col != 0)
		{
			int blockage = puzzlemap[row][col - 1];
			if ((blockage != 3) || (blockage != 4) || (blockage != 8))
			{
				splitcheck = checkpuzzle(row, col - 1, 2);
				if (splitcheck == true)
				{
					return true;
				}
			}
		}
		if (col != 5)
		{
			int blockage = puzzlemap[row][col + 1];
			if ((blockage != 2) || (blockage != 5) || (blockage != 9))
			{
				splitcheck = checkpuzzle(row, col + 1, 2);
				if (splitcheck == true)
				{
					return true;
				}
			}
		}
		if (row != 0)
		{
			int blockage = puzzlemap[row - 1][col];
			if ((blockage != 2) || (blockage != 4) || (blockage != 6))
			{
				splitcheck = checkpuzzle(row - 1, col, 1);
				if (splitcheck == true)
				{
					return true;
				}
			}
		}
		return false;
	}
	if (puzzlemap[row][col] == 7)
	{
		bool splitcheck = false;
		if (col != 0)
		{
			int blockage = puzzlemap[row][col - 1];
			if ((blockage != 3) || (blockage != 4) || (blockage != 8))
			{
				splitcheck = checkpuzzle(row, col - 1, 2);
				if (splitcheck == true)
				{
					return true;
				}
			}
		}
		if (col != 5)
		{
			int blockage = puzzlemap[row][col + 1];
			if ((blockage != 2) || (blockage != 5) || (blockage != 9))
			{
				splitcheck = checkpuzzle(row, col + 1, 2);
				if (splitcheck == true)
				{
					return true;
				}
			}
		}
		if (row != 8)
		{
			int blockage = puzzlemap[row + 1][col];
			if ((blockage != 3) || (blockage != 5) || (blockage != 7))
			{
				splitcheck = checkpuzzle(row + 1, col, 1);
				if (splitcheck == true)
				{
					return true;
				}
			}
		}
		return false;
	}
	if (puzzlemap[row][col] == 8)
	{
		bool splitcheck = false;
		if (col != 0)
		{
			int blockage = puzzlemap[row][col - 1];
			if ((blockage != 3) || (blockage != 4) || (blockage != 8))
			{
				splitcheck = checkpuzzle(row, col - 1, 2);
				if (splitcheck == true)
				{
					return true;
				}
			}
		}
		if (row != 0)
		{
			int blockage = puzzlemap[row - 1][col];
			if ((blockage != 2) || (blockage != 4) || (blockage != 6))
			{
				splitcheck = checkpuzzle(row - 1, col, 1);
				if (splitcheck == true)
				{
					return true;
				}
			}
		}
		if (row != 8)
		{
			int blockage = puzzlemap[row + 1][col];
			if ((blockage != 3) || (blockage != 5) || (blockage != 7))
			{
				splitcheck = checkpuzzle(row + 1, col, 1);
				if (splitcheck == true)
				{
					return true;
				}
			}
		}
		return false;
	}
	if (puzzlemap[row][col] == 9)
	{
		bool splitcheck = false;
		if (col != 0)
		{
			int blockage = puzzlemap[row][col - 1];
			if ((blockage != 3) || (blockage != 4) || (blockage != 8))
			{
				splitcheck = checkpuzzle(row, col - 1, 2);
				if (splitcheck == true)
				{
					return true;
				}
			}
		}
		if (col != 5)
		{
			int blockage = puzzlemap[row][col + 1];
			if ((blockage != 2) || (blockage != 5) || (blockage != 9))
			{
				splitcheck = checkpuzzle(row, col + 1, 2);
				if (splitcheck == true)
				{
					return true;
				}
			}
		}
		if (row != 8)
		{
			int blockage = puzzlemap[row + 1][col];
			if ((blockage != 3) || (blockage != 5) || (blockage != 7))
			{
				splitcheck = checkpuzzle(row + 1, col, 1);
				if (splitcheck == true)
				{
					return true;
				}
			}
		}
		return false;
	}
	if (puzzlemap[row][col] == 10)
	{
		bool splitcheck = false;
		if (col != 0)
		{
			int blockage = puzzlemap[row][col - 1];
			if ((blockage != 3) || (blockage != 4) || (blockage != 8))
			{
				splitcheck = checkpuzzle(row, col - 1, 2);
				if (splitcheck == true)
				{
					return true;
				}
			}
		}
		if (col != 5)
		{
			int blockage = puzzlemap[row][col + 1];
			if ((blockage != 2) || (blockage != 5) || (blockage != 9))
			{
				splitcheck = checkpuzzle(row, col + 1, 2);
				if (splitcheck == true)
				{
					return true;
				}
			}
		}
		if (row != 0)
		{
			int blockage = puzzlemap[row - 1][col];
			if ((blockage != 2) || (blockage != 4) || (blockage != 6))
			{
				splitcheck = checkpuzzle(row - 1, col, 1);
				if (splitcheck == true)
				{
					return true;
				}
			}
		}
		if (row != 8)
		{
			int blockage = puzzlemap[row + 1][col];
			if ((blockage != 3) || (blockage != 5) || (blockage != 7))
			{
				splitcheck = checkpuzzle(row + 1, col, 1);
				if (splitcheck == true)
				{
					return true;
				}
			}
		}
		return false;
	}
}

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
		if (orientation == 2)
		{
			if (col != 5)
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
}

void PipePuzzle::setpuzzle()
{
	bool completedpuzzle = false;
	while (!completedpuzzle)
	{
		for (int j = 0; j < 6; j++)
		{
		for (int i = 0; i < 9; i++)
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
		//completedpuzzle = confirmpuzzle(0, 0, 1);
		completedpuzzle = checkpuzzle(0, 0, 1);
	}

}