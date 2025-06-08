#include "moo.h"
#include <iostream>
#include <map>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace pt = boost::property_tree;

#define MAX_PACKET_SIZE (4 * 1024)


uint32_t ntohl(uint32_t netlong) {

    return ::ntohl(netlong);
}

void str_delete(std::string& bu, int pos)
{
    int i;
    for (i = pos; i < bu.size(); i++)
    {
        if (bu[i] == '#')
        {
            break;
        }
    }

    bu.erase(0, i + 1);

    return;
}

My_Moo::Moo::Moo()
{
}

My_Moo::Moo::~Moo()
{
}

bool My_Moo::Moo::run(const std::string& data) {
    static std::string buffer;

    buffer += data;

    size_t pos = 0;
    const size_t buffer_size = buffer.size();
    bool processed = false; 

    while (pos + 10 <= buffer_size) {
  
        if (pos + 8 > buffer_size) break;


        uint32_t flag = ntohl(*reinterpret_cast<const uint32_t*>(buffer.data() + pos));
        uint32_t declared_len = ntohl(*reinterpret_cast<const uint32_t*>(buffer.data() + pos + 4));


        if (declared_len > MAX_PACKET_SIZE) {
            buffer.clear();
            return false;
        }


        const size_t chunk_len = 8 + declared_len + 1;
        if (pos + chunk_len > buffer_size) break;

        if (buffer[pos + chunk_len - 1] != '#') {
            const size_t next_delim = buffer.find('#', pos);
            if (next_delim == std::string::npos) {
                buffer.clear();
                break;
            }
            pos = next_delim + 1;
            continue;
        }

        std::string chunk = buffer.substr(pos + 8, declared_len);
        run_tool(flag, std::string(chunk)); 

        processed = true;
        pos += chunk_len;
    }

    if (pos > 0) {
        buffer.erase(0, pos);
    }

    return processed; 
}

bool My_Moo::Moo::run_tool(int mark,const std::string& data)
{


	return false;
}

bool My_Moo::handle_one::str_map(std::string& data, std::map<std::string, std::string>& result)
{
    pt::ptree ptree;

    bool mark = true;
    try {
        std::istringstream iss(data);
        pt::read_json(iss, ptree);

        for (const auto& pair : ptree) {
            result[pair.first] = pair.second.get_value<std::string>();
        }
    }
    catch (const pt::json_parser_error& e) {
        std::cerr << "JSON ½âÎö´íÎó: " << e.what() << std::endl;
        mark = false;
    }
    catch (...) {
        std::cerr << "Î´Öª´íÎó" << std::endl;
        mark = false;
    }

    return mark;
}

std::string My_Moo::handle_one::handle_str(std::string& data)
{
    std::map<std::string, std::string> result;

    bool re = true;
    if (str_map(data, result))
    {
        re = handle_str_tool(result);
    }
    else
    {
        re = false;
    }

    if (re)
    {
        return "true";
    }
    else
    {
        return "false";
    }
}

bool My_Moo::handle_one::handle_str_tool(std::map<std::string, std::string>& result)
{
    auto user = result.find("user");
    auto password = result.find("password");

    if (user != result.end() && password != result.end())
    {
        return config->mysql->checkUserCredentials(user->second, password->second);
    }

    return false;
}
