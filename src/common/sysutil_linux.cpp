#ifdef __linux__

#include "sysutil.h"
#include <cstdio>


void SetThreadName(const char* name) {}

void panic(const char* title, const char* msg)
{
    fprintf(stderr, "PANIC! [%s] %s\n", title, msg); 
    abort(); 
}


#include <filesystem>
void GetExecutablePath(char* output, size_t bufsize, size_t& len)
{
    char fullpath[256];
    memset(fullpath, 0, sizeof(fullpath));

    char szTmp[32];
    sprintf(szTmp, "/proc/%d/exe", getpid());
    int bytes = std::min(readlink(szTmp, fullpath, sizeof(fullpath)), static_cast<ssize_t>(sizeof(fullpath) - 1));
    if (bytes >= 0)
        fullpath[bytes] = '\0';

    using namespace std::filesystem;
    auto parent = path(fullpath).parent_path();
    strncpy(output, (const char*)parent.u8string().c_str(), bufsize);
    len = strlen(output);
}

void addWMEventHandler(void* f)
{

}

void callWMEventHandler(void* arg1, void* arg2, void* arg3, void* arg4)
{

}

#endif // __linux__
