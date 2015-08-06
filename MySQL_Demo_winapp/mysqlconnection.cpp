#include <mysql.h>
#include <my_global.h>
#include "stdafx.h"
#include "Resource.h"
#include "MySQL_Demo_winapp.h"

bool mysqlconnection::SetConnection(char * sql_username,char *sql_pass,char * sql_db, char *sql_host, int sql_portno){

	user=new char[50];
	strcpy(user,sql_username);

	pass=new char[50];
	strcpy(pass,sql_pass);

	db=new char[50];
	strcpy(db,sql_db);

	host=new char[50];
	strcpy(host,sql_host);

	portno=sql_portno;
    connection = mysql_init(NULL);
if (connection== NULL)
	{
	//MessageBox(hDlg,L"Connection Failed",L"Connection Error",MB_OK | MB_ICONINFORMATION);
	fprintf(stderr, "mysql_init() failed\n");
	return false;
	}

if (mysql_real_connect(connection,host,user,pass,db,portno, NULL, CLIENT_MULTI_STATEMENTS) == NULL) 
			 {
				return false;
			 }  
return true;
}

MYSQL* mysqlconnection::GetConnection()
{
return connection;
}

void mysqlconnection::CloseConnection(){
	mysql_close(connection);
}

void mysqlconnection::ClearDetails(){
	delete[] user;
	delete[] pass;
	delete[] db;
	delete[] host;
}

bool mysqlconnection::RestoreConnection(){

	connection = mysql_init(NULL);
	if (connection== NULL)
	{
	 //MessageBox(hDlg,L"Connection Failed",L"Connection Error",MB_OK | MB_ICONINFORMATION);
	//fprintf(stderr, "mysql_init() failed\n");
	//exit(1);
		return false;
	}

if (mysql_real_connect(connection,host,user,pass,db,portno, NULL, CLIENT_MULTI_STATEMENTS) == NULL) 
			 {
				//exit(1);
				 return false;
			 }  
return true;
}
