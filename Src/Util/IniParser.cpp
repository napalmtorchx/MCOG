#include "Util/IniParser.h"
#include "Minecraft.h"

namespace Minecraft
{
    // -- IniObject -----------------------------------------------------------------------------------------------------------------------

    IniObject::IniObject() : type(INITYPE_INVALID), name(""), sz(0), value(NULL) {}

    IniObject::IniObject(INITYPE type, std::string& name, DWORD sz, void* value) : type(type), name(name), sz(sz), value(value) {}

    int IniObject::ToInt() { return ((int*)value)[0]; }

    DWORD IniObject::ToUInt() { return ((DWORD*)value)[0]; }

    float IniObject::ToFloat() { return ((float*)value)[0]; }

    bool IniObject::ToBool() { return ((bool*)value)[0]; }

    char* IniObject::ToString() { return (char*)value; }

    // -- IniSection ----------------------------------------------------------------------------------------------------------------------

    IniSection::IniSection() : name("") {}

    IniSection::IniSection(const std::string& name) : name(name) {}

    void IniSection::Dispose()
    {
        std::map<std::string, IniObject>::iterator it = objects.begin();
        while (it != objects.end())
        {
            if (objects[it->first].value != NULL) { free(objects[it->first].value); } it++;
        }
        objects.clear();
    }

    bool IniSection::Contains(std::string str) { return objects.count(str) >= 1; }

    // -- IniParser -----------------------------------------------------------------------------------------------------------------------

    void IniParser::Dispose()
    {
        std::map<std::string, IniSection>::iterator it = sections.begin();
        while (it != sections.end())
        {
            sections[it->first].Dispose();
            it++;
        }
        sections.clear();
    }

    IniSection IniParser::ParseSection(std::vector<std::string>& lines, int index, int* end_index)
    {
        IniSection section;
        section.name = lines[index].substr(1, lines[index].find("]") - 1);
        index++;

        while (index < (int)lines.size())
        {
            if (lines[index][0] == '[') { *end_index = index; break; }
            else if ((lines[index][0] >= 'a' && lines[index][0] <= 'z') || (lines[index][0] >= 'A' && lines[index][0] <= 'Z'))
            {
                std::string key, value;
                int j = 0;
                bool done = false;
                while (j < (int)lines[index].size())
                {
                    if (lines[index][j] == ' ' || lines[index][j] == '\t') { j++; continue; }
                    if (lines[index][j] == '=') { done = true; break; }
                    key += lines[index][j++];
                }
                if (!done) { return IniSection(); }
                j++;
                while (lines[index][j] == ' ' || lines[index][j] == '\t') { j++; }

                while (j < (int)lines[index].size())
                {
                    if (lines[index][j] < 32 || lines[index][j] > 126) { break; }
                    value += lines[index][j++];
                }

                INITYPE type = INITYPE_INT;
                if (value[0] == '\"') { type = INITYPE_STRING; }
                else if (value == "true" || value == "false" || value == "TRUE" || value == "FALSE") { type = INITYPE_BOOL; }
                else if (value[0] == '0' && toupper(value[1]) == 'X') { type = INITYPE_UINT; }
                else if (value.find('.') != std::string::npos) { type = INITYPE_FLOAT; }

                IniObject obj;
                obj.type = type;
                obj.name = key;

                if (type == INITYPE_STRING)
                {
                    if (value.size() == 2) { obj.value = NULL; obj.sz = 0; }
                    else
                    {
                        obj.sz = value.size() - 1;
                        obj.value = new char[obj.sz];
                        memset(obj.value, 0, obj.sz);
                        memcpy(obj.value, value.c_str() + 1, value.size() - 2);
                    }
                }
                else if (type == INITYPE_BOOL)
                {
                    obj.sz = 1;
                    obj.value = new bool[1];
                    ((bool*)obj.value)[0] = ((value == "true" || value == "TRUE") ? true : false);
                }
                else if (type == INITYPE_FLOAT)
                {
                    obj.sz = 4;
                    obj.value = new float[1];
                    ((float*)obj.value)[0] = static_cast<float>(atof(value.c_str()));
                }
                else if (type == INITYPE_UINT)
                {
                    obj.sz = 4;
                    obj.value = new DWORD[1];
                    ((DWORD*)obj.value)[0] = strtoul(value.c_str() + 2, NULL, 16);
                }
                else if (type == INITYPE_INT)
                {
                    obj.sz = 4;
                    obj.value = new int[1];
                    ((int*)obj.value)[0] = atoi(value.c_str());
                }

                section.objects[key] = obj;
            }

            index++;
        }
        return section;
    }

    bool IniParser::Parse(LPCSTR path)
    {
        std::vector<std::string> lines = FileSystem::ReadLines(path);

        DWORD i = 0;
        while (i < lines.size())
        {
            if (lines[i][0] == '[')
            {
                int end_index = -1;
                IniSection section = ParseSection(lines, i, &end_index);
                sections[section.name] = section;
                i = end_index;
            }
            else { i++; }
        }

        lines.clear();
        return true;
    }
}