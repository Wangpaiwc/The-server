#include "mysql.h"
#include <iostream>

MySQLConnector::MySQLConnector() : session(nullptr) {
    connect();
    createUserTable();
}

MySQLConnector::~MySQLConnector() {
    if (session) {
        delete session;
    }
}


bool MySQLConnector::connect() {
    try {
        ConfigManager& c = ConfigManager::instance();

        session = new mysqlx::Session(c.host, c.port, c.user, c.password, c.database);
        createUserTable();
        
        return true;
    }
    catch (const mysqlx::Error& err) {
        std::cerr << "Connection failed: " << err.what() << std::endl;
        return false;
    }
}

bool MySQLConnector::createUserTable() {
    if (!session) {
        std::cerr << "Not connected to database!" << std::endl;
        return false;
    }

    try {

        session->sql("CREATE TABLE IF NOT EXISTS users ("
            "id INT AUTO_INCREMENT PRIMARY KEY, "
            "username VARCHAR(50) NOT NULL UNIQUE, "
            "password VARCHAR(100) NOT NULL)")
            .execute();
        std::cout << "User table created successfully!" << std::endl;

      
        session->sql("CREATE TABLE IF NOT EXISTS user_relations ("
            "user1_id INT NOT NULL, "
            "user2_id INT NOT NULL, "
            "relation_type VARCHAR(40) NOT NULL, "
            "FOREIGN KEY (user1_id) REFERENCES users(id), "
            "FOREIGN KEY (user2_id) REFERENCES users(id), "
            "CHECK (user1_id <> user2_id))")
            .execute();
        std::cout << "User table created successfully!" << std::endl;
        return true;
    }
    catch (const mysqlx::Error& err) {
        std::cerr << "Failed to create table: " << err.what() << std::endl;
        return false;
    }
}

bool MySQLConnector::insertUser(const std::string& username, const std::string& password) {
    if (!session) {
        std::cerr << "Not connected to database!" << std::endl;
        return false;
    }

    try {
        session->sql("INSERT INTO users (username, password) VALUES (?, ?)")
            .bind(username, password)
            .execute();
        std::cout << "User inserted successfully!" << std::endl;
        return true;
    }
    catch (const mysqlx::Error& err) {
        std::cerr << "Failed to insert user: " << err.what() << std::endl;
        return false;
    }

   
    }



bool MySQLConnector::insertRelation(const std::string& username1,
    const std::string& username2,
    const std::string& relation_type) {
    if (!session) {
        std::cerr << "Not connected to database!" << std::endl;
        return false;
    }

    try {
        // 使用一条SQL语句完成操作
        session->sql(
            "INSERT INTO user_relations (user1_id, user2_id, relation_type) "
            "SELECT u1.id, u2.id, ? "
            "FROM users u1, users u2 "
            "WHERE u1.username = ? AND u2.username = ? AND u1.id <> u2.id")
            .bind(relation_type, username1, username2)
            .execute();

        std::cout << "Relation inserted successfully!" << std::endl;
        return true;

    }
    catch (const mysqlx::Error& err) {
        std::cerr << "Failed to insert relation: " << err.what() << std::endl;
        return false;
    }
}

bool MySQLConnector::checkUserCredentials(const std::string& username, const std::string& password) {
    if (!session) {
        std::cerr << "Not connected to database!" << std::endl;
        return false;
    }

    try {
        mysqlx::RowResult result = session->sql("SELECT id FROM users WHERE username = ? AND password = ?")
            .bind(username, password)
            .execute();

        return result.count() > 0;
    }
    catch (const mysqlx::Error& err) {
        std::cerr << "Failed to check user credentials: " << err.what() << std::endl;
        return false;
    }
}

std::vector<Yu> MySQLConnector::getUserRelationsByUsername(const std::string& username) {
    std::vector<Yu> relations;

    if (!session) {
        std::cerr << "Not connected to database!" << std::endl;
        return relations;
    }

    try {
        mysqlx::RowResult result = session->sql(
            "SELECT u2.username AS related_username, r.relation_type "
            "FROM users u1 "
            "JOIN user_relations r ON u1.id = r.user1_id "
            "JOIN users u2 ON r.user2_id = u2.id "
            "WHERE u1.username = ?")
            .bind(username)
            .execute();

        for (mysqlx::Row row : result) {
            std::string relatedUser = row[0].get<std::string>();
            std::string relationType = row[1].get<std::string>();
            relations.emplace_back(relatedUser, relationType);
        }

    }
    catch (const mysqlx::Error& err) {
        std::cerr << "Failed to get user relations by username: " << err.what() << std::endl;
    }

    return relations;
}