#include <iostream>

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        std::cerr << "You have to specify the two array dimensions" << std::endl;
        return -1;
    }

    int sizeX, sizeY;

    sizeX = std::stoi(argv[1]);
    sizeY = std::stoi(argv[2]);

    if (sizeX <= 0)
    {
        std::cerr << "Invalid dimension x" << std::endl;
        return -1;
    }
    if (sizeY <= 0)
    {
        std::cerr << "Invalid dimension y" << std::endl;
        return -1;
    }

    /******** Create a two dimensional dynamic array in continuous memory ******
     *
     * - Define the pointer holding the array
     * - Allocate memory for the array (linear)
     * - Allocate memory for the pointers inside the array
     * - Assign the pointers inside the array the corresponding addresses
     *   in the linear array
     **************************************************************************/

    // The resulting array
    unsigned int** array2d;

    // Linear memory allocation
    unsigned int* temp = new unsigned int[sizeX * sizeY];

    // These are the important steps:
    // Allocate the pointers inside the array,
    // which will be used to index the linear memory
    array2d = new unsigned int*[sizeY];

    // Let the pointers inside the array point to the correct memory addresses
    for (int i = 0; i < sizeY; ++i)
    {
        array2d[i] = (temp + i * sizeX);
    }



    // Fill the array with ascending numbers
    for (int y = 0; y < sizeY; ++y)
    {
        for (int x = 0; x < sizeX; ++x)
        {
            array2d[y][x] = x + y * sizeX;
        }
    }



    // Code for testing
    // Print the addresses
    for (int y = 0; y < sizeY; ++y)
    {
        for (int x = 0; x < sizeX; ++x)
        {
            std::cout << std::hex << &(array2d[y][x]) << ' ';
        }
    }
    std::cout << "\n\n";

    // Print the array
    for (int y = 0; y < sizeY; ++y)
    {
        std::cout << std::hex << &(array2d[y][0]) << std::dec;
        std::cout << ": ";
        for (int x = 0; x < sizeX; ++x)
        {
            std::cout << array2d[y][x] << ' ';
        }
        std::cout << std::endl;
    }



    // Free memory
    delete[] array2d[0];
    delete[] array2d;
    array2d = nullptr;

    return 0;
}
