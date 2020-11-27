#ifndef MYSQL_CONNECTION
#define MYSQL_CONNECTION

//Brynet has it aswell, and because of using namespace it is defined here aswell
//later this will be fixed better
#ifdef IS_NUM
#undef IS_NUM
#endif

#include <mysql.h>

class MysqlConnection {
  public:
    MysqlConnection() {
        mysql = new MYSQL();
    }
    ~MysqlConnection()
    {
        delete mysql;
    }
    
    MYSQL *mysql;
};

#undef IS_NUM

#endif