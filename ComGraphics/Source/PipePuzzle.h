#ifndef PIPEPUZZLE_H
#define PIPEPUZZLE_H

#include <cstdlib>

class PipePuzzle
{
private:
	int puzzlechecked[9][6];
public:
	int puzzlemap[9][6];
    float puzzlerotation[9][6];   //the rotation values of each puzzle stored

	PipePuzzle();
	~PipePuzzle();
	void checkpuzzle();
	
	bool confirmpuzzle(int row, int col, int orientation);
	void setpuzzle();
};

#endif