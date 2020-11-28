#ifndef PGSQL_CONNECTION
#define PGSQL_CONNECTION

//Brynet has it aswell, and because of using namespace it is defined here aswell
//later this will be fixed better
//#ifdef IS_NUM
//#undef IS_NUM
//#endif

#include <libpq-fe.h>

class PGSQLConnection {
  public:
    PGSQLConnection() {
        conn = PQconnectStart("");
    }
    ~PGSQLConnection()
    {
        PQfinish(conn);
    }
    
    PGconn *conn;
};

//#undef IS_NUM

#endif