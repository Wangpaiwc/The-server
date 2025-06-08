#pragma once

#include <mysqlx/xdevapi.h>
#include <string>

class Yu
{
public:
    std::string user;
    std::string contend;

    Yu(std::string& user, std::string& contend) : user(user), contend(contend)
    {

    }
};

class MySQLConnector {
public:
    MySQLConnector();
    ~MySQLConnector();

    // 连接MySQL数据库
    bool connect();

    // 创建用户表
    bool createUserTable();

    // 插入用户数据
    bool insertUser(const std::string& username, const std::string& password);

    // 插入关联数据（使用用户名而非ID）
    bool insertRelation(const std::string& username1,
        const std::string& username2,
        const std::string& relation_type);

    bool checkUserCredentials(const std::string& username, const std::string& password);

    std::vector<Yu> getUserRelationsByUsername(const std::string& username);

private:
    mysqlx::Session* session;
};

class ConfigManager
{
public:
    std::string host;
    int port;
    std::string user;
    std::string password;
    std::string database;
    int thread_number;

    static ConfigManager& instance()
    {
        static ConfigManager cm;

        return cm;
    }

    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;
private:
    ConfigManager()
    {
        read();
    }

    bool read()
    {
        host = "127.0.0.1";
        port = 33060;
        user = "root";
        password = "root";
        database = "list";

        thread_number = 2;

        return true;
    }
};