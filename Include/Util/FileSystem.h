#pragma once
#include "Common.h"

namespace Minecraft
{
    class FileSystem
    {
        public:
            static bool DirExists(LPCSTR path);
            static bool FileExists(LPCSTR path);

        public:
            static DWORD WriteBinary(LPCSTR path, const void* src, DWORD sz);
            static DWORD WriteText(LPCSTR path, LPCSTR src, DWORD sz);
            static DWORD WriteLines(LPCSTR path, std::vector<std::string> lines);

        public:
            static void ReadBinary(LPCSTR path, void** dest, DWORD* sz);
            static void ReadText(LPCSTR path, char** dest, DWORD* sz);
            static std::vector<std::string> ReadLines(LPCSTR path);

        public:
            static std::vector<std::string> GetDirs(LPCSTR path);
            static std::vector<std::string> GetFiles(LPCSTR path);

        public:
            static bool CreateDir(LPCSTR path);
            static bool DeleteDir(LPCSTR path);
    };
}