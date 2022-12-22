#ifndef PGSQL_CONNECTION
#define PGSQL_CONNECTION

#include "database/database.h"

//Brynet has it as well, and because of using namespace it is defined here as well
//later this will be fixed better
//#ifdef IS_NUM
//#undef IS_NUM
//#endif

#include <libpq-fe.h>

class PGSQLDatabase : public Database {
public:
	static Database *_creation_func();
	static void _register();
	static void _unregister();

	PGSQLDatabase() :
			Database() {
		conn = PQconnectStart("");
	}
	~PGSQLDatabase() {
		PQfinish(conn);
	}

	PGconn *conn;
};

//#undef IS_NUM

#endif