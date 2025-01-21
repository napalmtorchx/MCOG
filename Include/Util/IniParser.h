#pragma once
#include "Common.h"
#include "FileSystem.h"

namespace Minecraft
{
    enum INITYPE
    {
        INITYPE_INVALID,
        INITYPE_INT,
        INITYPE_UINT,
        INITYPE_FLOAT,
        INITYPE_BOOL,
        INITYPE_STRING,
    };

    struct IniObject
    {
        public:
            INITYPE     type;
            std::string name;
            DWORD       sz;
            void* value;

        public:
            IniObject();
            IniObject(INITYPE type, std::string& name, DWORD sz, void* value);
            int   ToInt();
            DWORD ToUInt();
            float ToFloat();
            bool  ToBool();
            char* ToString();
    };

    struct IniSection
    {
        public:
            std::string            name;
            std::map<std::string, IniObject> objects;

        public:
            IniSection();
            IniSection(const std::string& name);
            void Dispose();
            bool Contains(std::string str);
        };

        class IniParser
        {
        public:
            std::map<std::string, IniSection> sections;

        public:
            void Dispose();
            bool Parse(LPCSTR path);
            IniSection ParseSection(std::vector<std::string>& lines, int index, int* end_index);
    };
}