#include "Util/FileSystem.h"
#include "Minecraft.h"

namespace Minecraft
{
    bool FileSystem::DirExists(LPCSTR path)
    {
        DWORD attrs = GetFileAttributes(path);
        if (attrs == 0xFFFFFFFF)
        {
            if (GetLastError() == ERROR_FILE_NOT_FOUND || GetLastError() == ERROR_PATH_NOT_FOUND)
            {
                return false;
            }
        }
        return (attrs & FILE_ATTRIBUTE_DIRECTORY) != 0;
    }

    bool FileSystem::FileExists(LPCSTR path)
    {
        DWORD attrs = GetFileAttributes(path);
        if (attrs == 0xFFFFFFFF)
        {
            if (GetLastError() == ERROR_FILE_NOT_FOUND || GetLastError() == ERROR_PATH_NOT_FOUND)
            {
                return false;
            }
        }
        return (!(attrs & FILE_ATTRIBUTE_DIRECTORY)) != 0;
    }

    DWORD FileSystem::WriteBinary(LPCSTR path, const void* src, DWORD sz)
    {
        if (src == NULL) { RXDK_LOG_ERROR("Attempt to write file with null source pointer: '%s'", path); return 0; }
        if (sz == 0) { RXDK_LOG_ERROR("Attempt to write file with size of zero: '%s'", path); return 0; }

        FILE* fp = fopen(path, "w");
        if (fp == NULL) { RXDK_LOG_ERROR("Failed to open file for writing: '%s'", path); return 0; }

        DWORD written = fwrite(src, sz, 1, fp);
        fclose(fp);
        //Debug::log("%s FileWrite(Binary): '%s'\n", MSG_OK, path);
        return written;
    }

    DWORD FileSystem::WriteText(LPCSTR path, LPCSTR src, DWORD sz)
    {
        if (src == NULL) { RXDK_LOG_ERROR("Attempt to write text file with null source pointer: '%s'", path); return 0; }
        if (sz == 0) { RXDK_LOG_ERROR("Attempt to write text file with size of zero: '%s'", path); return 0; }

        FILE* fp = fopen(path, "w");
        if (fp == NULL) { RXDK_LOG_ERROR("Failed to open text file for writing: '%s'", path); return 0; }

        DWORD written = fwrite(src, sz, 1, fp);
        fclose(fp);
        //Debug::log("%s FileWrite(Text): '%s'\n", MSG_OK, path);
        return written;
    }

    DWORD FileSystem::WriteLines(LPCSTR path, std::vector<std::string> lines)
    {
        std::string text;
        for (DWORD i = 0; i < lines.size(); i++) { text += lines[i] + "\r\n"; }
        return WriteText(path, text.c_str(), text.size());
    }

    void FileSystem::ReadBinary(LPCSTR path, void** dest, DWORD* sz)
    {
        if (dest == NULL) { RXDK_LOG_ERROR("Attempt to read file with null dest pointer: '%s'", path); return; }

        FILE* fp = fopen(path, "w");
        if (fp == NULL) { RXDK_LOG_ERROR("Failed to open file for reading: '%s'", path); return; }

        fseek(fp, 0, SEEK_END);
        DWORD filesz = ftell(fp);
        rewind(fp);

        *dest = malloc(filesz);
        if (*dest == NULL) { RXDK_LOG_ERROR("Failed to allocate memory for file: '%s'", path); return; }

        fread(*dest, filesz, 1, fp);
        fclose(fp);

        if (sz != NULL) { *sz = filesz; }
        //Debug::log("%s FileRead(Binary): '%s'\n", MSG_OK, path);
    }

    void FileSystem::ReadText(LPCSTR path, char** dest, DWORD* sz)
    {
        if (dest == NULL) { RXDK_LOG_ERROR("Attempt to read text file with null dest pointer: '%s'", path); return; }

        FILE* fp = fopen(path, "r");
        if (fp == NULL) { RXDK_LOG_ERROR("Failed to open text file for reading: '%s'", path); return; }

        fseek(fp, 0, SEEK_END);
        DWORD filesz = ftell(fp);
        rewind(fp);

        *dest = (char*)malloc(filesz);
        if (*dest == NULL) { RXDK_LOG_ERROR("Failed to allocate memory for text file: '%s'", path); return; }

        fread(*dest, filesz, 1, fp);
        fclose(fp);

        if (sz != NULL) { *sz = filesz; }
        //Debug::log("%s FileRead(Text): '%s'\n", MSG_OK, path);
    }

    std::vector<std::string> FileSystem::ReadLines(LPCSTR path)
    {
        char* data = NULL;
        DWORD sz = 0;
        ReadText(path, &data, &sz);
        if (data == NULL) { return std::vector<std::string>(); }

        std::vector<std::string> lines;
        std::string              temp;
        for (DWORD i = 0; i < sz; i++)
        {
            if (i == sz - 1 || data[i] == '\n') { lines.push_back(temp); temp.clear(); }
            else { temp += data[i]; }
        }

        if (data != NULL) { free(data); }
        return lines;
    }

    std::vector<std::string> FileSystem::GetDirs(LPCSTR path)
    {
        WIN32_FIND_DATA find;
        HANDLE          handle = INVALID_HANDLE_VALUE;

        std::vector<std::string> dirs;
        std::string fullpath = std::string(path) + "\\*";

        handle = FindFirstFile(fullpath.c_str(), &find);
        if (handle == INVALID_HANDLE_VALUE) { return dirs; }

        do
        {
            if (find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                if (strcmp(find.cFileName, ".") != 0 && strcmp(find.cFileName, "..") != 0)
                {
                    dirs.push_back(find.cFileName);
                }
            }
        } while (FindNextFile(handle, &find) != 0);

        FindClose(handle);
        return dirs;
    }

    std::vector<std::string> FileSystem::GetFiles(LPCSTR path)
    {
        WIN32_FIND_DATA find;
        HANDLE          handle = INVALID_HANDLE_VALUE;

        std::vector<std::string> files;
        std::string fullpath = std::string(path) + "\\*";

        handle = FindFirstFile(fullpath.c_str(), &find);
        if (handle == INVALID_HANDLE_VALUE) { return files; }

        do
        {
            if (!(find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                if (strcmp(find.cFileName, ".") != 0 && strcmp(find.cFileName, "..") != 0)
                {
                    files.push_back(find.cFileName);
                }
            }
        } while (FindNextFile(handle, &find) != 0);

        FindClose(handle);
        return files;
    }

    bool FileSystem::CreateDir(LPCSTR path) { return (bool)CreateDirectory(path, NULL); }

    bool FileSystem::DeleteDir(LPCSTR path) { return (bool)RemoveDirectory(path); }
}