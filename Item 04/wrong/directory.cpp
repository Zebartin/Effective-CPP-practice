/*  https://stackoverflow.com/questions/68637088/effective-c-3rd-edition-item-4-code-layout
 *  g++ fileSystem.cpp directory.cpp; ./a.out
 *  But it seems that the compiler is too smart to make such a mistake
 */
#include <iostream>
#include "directory.h"

Directory tempDir;
int main()
{
    return 0;
}