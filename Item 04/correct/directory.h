#include "fileSystem.h"
class Directory
{
public:
    Directory();
};

Directory::Directory()
{
    std::size_t disks = tfs().numDisks();
    std::cout << disks << std::endl;
}
Directory& tempDir()
{
    static Directory td;
    return td;
}