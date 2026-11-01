#include<iostream>
#include<windows.h>
#include<sqlext.h>
#include<string>
#include<vector>
#include<map>
using namespace std;

class AccessDB
{
	private:
    SQLHENV hEnv;
    SQLHDBC hDbc;
    SQLHSTMT hStmt;
    bool isConnected;
    
    public:
	AccessDB()
	{
	    hEnv = NULL;
	    hDbc = NULL;
	    hStmt = NULL;
	    isConnected = false;
	}
	
	~AccessDB()
	{
	    disconnect();
	}
	
	bool connect(const string dsnName)
	{
	    SQLRETURN ret;
	    SQLCHAR outstr[1024];
	    SQLSMALLINT outstrlen;
	
	    SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
	    SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
	    SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);
	
	    string dsnConn = "DSN=" + dsnName + ";";
	    ret = SQLDriverConnect(hDbc, NULL,
	                           (SQLCHAR*)dsnConn.c_str(), SQL_NTS,
	                           outstr, sizeof(outstr), &outstrlen, SQL_DRIVER_COMPLETE);
	
	    if (SQL_SUCCEEDED(ret)) {
	        isConnected = true;
	        //std::cout << "Connected to database successfully.\n";
	        return true;
	    } else {
	        //std::cerr << "Failed to connect to database.\n";
	        return false;
	    }
	}
	
	void disconnect()
	{
	    if (isConnected) {
	        if (hStmt) {
	            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	        }
	        SQLDisconnect(hDbc);
	        SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
	        SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
	        isConnected = false;
	        //cout << "Disconnected from database.\n";
	    }
	}
	
	bool executeQuery(const string sql)
	{
	    if (!isConnected) {
	        cerr << "Not connected to the database.\n";
	        return false;
	    }
	
	    if (hStmt) {
	        SQLFreeHandle(SQL_HANDLE_STMT, hStmt); // Clean up previous statement
	    }
	
	    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
	    SQLRETURN ret = SQLExecDirect(hStmt, (SQLCHAR*)sql.c_str(), SQL_NTS);
	
	    if (SQL_SUCCEEDED(ret)) {
	        //cout << "Query executed successfully.\n";
	        return true;
	    } else {
	        //cerr << "Failed to execute query.\n";
	        return false;
	    }
	}
	
	vector<string> fetchColumn(int columnIndex)
	{
	    vector<string> results;
	    if (!isConnected || !hStmt) {
	        cerr << "Not connected or no statement prepared.\n";
	        return results;
	    }
	
	    char buffer[1024];
	    while (SQLFetch(hStmt) == SQL_SUCCESS) {
	        SQLGetData(hStmt, columnIndex, SQL_C_CHAR, buffer, sizeof(buffer), NULL);
	        results.push_back(std::string(buffer));
	    }
	    return results;
	}
	
	map<string, string> fetchRow()
	{
	    map<string, string> row;
	
	    if (!isConnected || !hStmt) {
	        cerr << "Not connected or no statement prepared.\n";
	        return row;
	    }
	
	    SQLRETURN ret = SQLFetch(hStmt);
	    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
	        // No more data, return empty map.
	        return row;
	    }
	
	    // Get number of columns
	    SQLSMALLINT numColumns;
	    SQLNumResultCols(hStmt, &numColumns);
	
	    for (SQLUSMALLINT i = 1; i <= numColumns; i++) {
	        SQLCHAR columnName[128];
	        SQLSMALLINT nameLength, dataType, decimalDigits, nullable;
	        SQLULEN columnSize;
	
	        // Get column name
	        SQLDescribeCol(hStmt, i, columnName, sizeof(columnName), &nameLength,
	                       &dataType, &columnSize, &decimalDigits, &nullable);
	
	        // Get column data
	        char buffer[1024] = {0};
	        SQLGetData(hStmt, i, SQL_C_CHAR, buffer, sizeof(buffer), NULL);
	
	        row[string((char*)columnName)] = string(buffer);
	    }
	
	    return row;
	}
	
	vector<map<string, string>> fetchAllRows()
	{
	    vector<map<string, string>> allRows;
	
	    if (!isConnected || !hStmt) {
	        cerr << "Not connected or no statement prepared.\n";
	        return allRows;
	    }
	
	    SQLSMALLINT numColumns;
	    SQLNumResultCols(hStmt, &numColumns);
	
	    while (true) {
	        SQLRETURN ret = SQLFetch(hStmt);
	        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
	            break;  // No more rows
	        }
	
	        map<string, string> row;
	        for (SQLUSMALLINT i = 1; i <= numColumns; i++) {
	            SQLCHAR columnName[128];
	            SQLSMALLINT nameLength, dataType, decimalDigits, nullable;
	            SQLULEN columnSize;
	
	            // Get column name
	            SQLDescribeCol(hStmt, i, columnName, sizeof(columnName), &nameLength,
	                           &dataType, &columnSize, &decimalDigits, &nullable);
	
	            // Get column data
	            char buffer[1024] = {0};
	            SQLGetData(hStmt, i, SQL_C_CHAR, buffer, sizeof(buffer), NULL);
	
	            row[string((char*)columnName)] = string(buffer);
	        }
	        allRows.push_back(row);
	    }
	
	    return allRows;
	}	
};



