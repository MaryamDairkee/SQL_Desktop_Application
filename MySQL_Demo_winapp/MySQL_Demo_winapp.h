#pragma once

#include <mysql.h>
#include "resource.h"
#pragma once

class mysqlconnection{

private:
	MYSQL* connection;
	char * user;
	char *pass;
	char *db;char *host;
	int portno;
public:
	
	bool SetConnection(char * user,char *pass,char *db,char *host,int portno);
	MYSQL* GetConnection();
	void CloseConnection();
	bool RestoreConnection();
	void ClearDetails();
};