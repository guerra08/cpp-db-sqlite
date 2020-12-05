#include <iostream>
#include <sqlite3.h>
#include <sstream>

int create_table(std::string db_name, std::string tb_name, std::string tb_definition, std::string tb_constraints);
int insert_into(std::string db_name, std::string tb_name, std::string tb_definition, std::string values);
int select_from(std::string db_name, std::string tb_name, std::string selection, std::string where_clause);
int exec_callback(void* data, int argc, char** argv, char** azColName);

int main()
{
    if ((create_table("my_db.db", "entries", "id INTEGER NOT NULL PRIMARY KEY, entry TEXT NOT NULL", "") != 0)) 
    {
        std::cerr << "Error creating table!" << std::endl;
        return 1;
    }
    if ((insert_into("my_db.db", "entries", "entry", "'Hello, world!'")) != 0) 
    {
        std::cerr << "Error inserting into table!" << std::endl;
        return 1;
    }
    if ((select_from("my_db.db", "entries", "*", "id = 1")) != 0)
    {
        std::cerr << "Error inserting into table!" << std::endl;
        return 1;
    }
    return 0;
}

/*
    Creates a table in the given database;
*/
int create_table(std::string db_name, std::string tb_name, std::string tb_definition, std::string tb_constraints) 
{
    sqlite3* db;
    if ((sqlite3_open(db_name.c_str(), &db)))
        return -1;
    std::stringstream sql;
    char* err;
    sql << "CREATE TABLE IF NOT EXISTS " << tb_name << "(" << tb_definition << tb_constraints << ")";
    int exit;
    exit = sqlite3_exec(db, sql.str().c_str(), NULL, 0, &err);
    if (exit != SQLITE_OK) 
    {
        sqlite3_free(err);
        return 1;
    }
    sqlite3_close(db);
    return 0;
}

/*
    Inserts a row into a given table
*/
int insert_into(std::string db_name, std::string tb_name, std::string tb_definition, std::string values) 
{
    sqlite3* db;
    int exit;
    if ((sqlite3_open(db_name.c_str(), &db)))
        return -1;
    std::stringstream sql;
    char* err;
    sql << "INSERT INTO " << tb_name << " (" << tb_definition << ") VALUES (" << values << ");";
    exit = sqlite3_exec(db, sql.str().c_str(), NULL, 0, &err);
    if (exit != SQLITE_OK) 
    {
        sqlite3_free(err);
        return 1;
    }
    sqlite3_close(db);
    return 0;
}

/*
    Selects rows from a given table, with a where clause.
*/
int select_from(std::string db_name, std::string tb_name, std::string selection, std::string where_clause)
{
    sqlite3* db;
    int exit;
    if ((sqlite3_open(db_name.c_str(), &db)))
        return -1;
    std::stringstream sql;
    char* err;
    sql << "SELECT " << selection << " FROM " << tb_name << " WHERE " << where_clause << ";";
    exit = sqlite3_exec(db, sql.str().c_str(), exec_callback, 0, &err);
    if (exit != SQLITE_OK)
    {
        sqlite3_free(err);
        return 1;
    }
    sqlite3_close(db);
    return 0;
}

int exec_callback(void* data, int argc, char** argv, char** azColName)
{
    int i;
    for (i = 0; i < argc; i++)
    {
        std::cout << azColName[i] << " " << argv[i] << "\n";
    }
    return 0;
}