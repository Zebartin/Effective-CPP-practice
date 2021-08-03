#include "fileSystem.h"

std::size_t FileSystem::numDisks() const
{
    return 517;
}
FileSystem& tfs()
{
    static FileSystem fs;
    return fs;
}