#include <cstdlib>
#include <iostream>
void testrng();

unsigned int generate_range(unsigned from, unsigned to)
{
    return (rand() % (to - from)) + from;
}

void testrng()
{
    //weedList[SMOKE_WEED] = MeshBuilder::GenerateQuad("smokeweed", Color(0, 0, 0), 80, 20);
    unsigned num = 42;
    srand(num);

    int numweed[2];

    for (int i = 0; i < 2; ++i)
    {
        numweed[i] = generate_range(4, 8);
        std::cout << numweed[i] << std::endl;
    }
}
