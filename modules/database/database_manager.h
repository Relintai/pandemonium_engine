#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include "core/string.h"

#include <vector>
#include <map>
#include <functional>

#include "core/object.h"

#include "database.h"

class DatabaseManager : public Object {
    RCPP_OBJECT(DatabaseManager, Object);

public:
    std::vector<Database *> databases;
    Database *ddb;

    void load();

    static DatabaseManager *get_singleton();

    //note: not threadsafe, create these at the start of your program!
    uint32_t create_database(const String &name);

    static void _register_db_creation_func(const String &name, std::function<Database*()> func);
    static void _unregister_db_creation_func(const String &name);

    static Database *_create_database(const String &name);

    DatabaseManager();
    ~DatabaseManager();

private:
    static DatabaseManager * _instance;

    static std::map<String, std::function<Database *()> > _db_creation_func_map;
};

#endif