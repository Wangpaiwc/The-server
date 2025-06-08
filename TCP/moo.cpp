#include "moo.h"
#include <iostream>
uint32_t ntohl(uint32_t netlong) {

    return ::ntohl(netlong);
}

My_Moo::Moo::Moo()
{
}

My_Moo::Moo::~Moo()
{
}

bool My_Moo::Moo::run(const std::string& data)
{
    static std::string buffer; 

    buffer += data;

    size_t pos = 0;
    const size_t buffer_size = buffer.size();

    while (pos + 10 <= buffer_size) {
        if (pos + 8 > buffer_size) break;

        uint32_t flag;
        memcpy(&flag, buffer.data() + pos, sizeof(uint32_t));
        flag = ntohl(flag);

        uint32_t declared_len;
        memcpy(&declared_len, buffer.data() + pos + 4, sizeof(uint32_t));
        declared_len = ntohl(declared_len);

        size_t chunk_len = 8 + declared_len + 1;
        if (pos + chunk_len > buffer_size) break;

        std::string chunk = buffer.substr(pos, chunk_len);

        if (!in.inspect(chunk)) {
            std::cout << "³ö´í" << std::endl;
            
            return false;
        }

        std::string content = chunk.substr(8, declared_len);

        run_tool(flag, content);

        pos += chunk_len;
    }

    if (pos > 0) {
        buffer.erase(0, pos);
    }

    return true;
}

bool My_Moo::Moo::run_tool(int mark,const std::string& data)
{


	return false;
}



bool My_Moo::Inspect::inspect(const std::string& chunk)
{
    if (chunk.size() < 10) return false;


    uint32_t declared_len;
    memcpy(&declared_len, chunk.data() + 4, sizeof(uint32_t));
    declared_len = ntohl(declared_len);

    size_t actual_content_len = chunk.size() - 9; 


    if (declared_len != actual_content_len) return false;
    if (chunk.back() != '#') return false;

    return true;

}
