#pragma once
#include <stdio.h>
#include <WinSock.h>
#include <Windows.h>
#include <mysql.h>
#include <iostream>
#include <vector>

class MysqlConnection {
public:
  MysqlConnection(std::string host, std::string database, std::string user, std::string password, uint32_t port) : res_(nullptr) {
    Connect(host, database, user, password, port);
  }
  ~MysqlConnection() {
    mysql_close(&mysql_);
  }

  bool Execute(std::string query) {
    int retry = 0;
    do
    {
      if (res_)
        mysql_free_result(res_);

      mysql_query(&mysql_, query.c_str());
      res_ = mysql_store_result(&mysql_);
      status_ = mysql_errno( &mysql_ );
      if(status_ == 0)
        return true;
      err_ = mysql_error( &mysql_ );
      mysql_ping( &mysql_ );
      retry++;
    } while( retry <= 1 );
    return Success();
  }
  bool Success()  {
    return status_ == 0;
  }

  std::string GetError() {
    return err_;
  }

  uint32_t GetStatus() {
    return status_;
  }

  int Rows() {
    return (int)mysql_num_rows( res_ );
  }

  char* GetValue( int row, int column ) {
    std::vector<MYSQL_ROW> vec_rows;
    MYSQL_ROW mysql_row = 0;
    while( (mysql_row = mysql_fetch_row( res_ )) )
      vec_rows.push_back(mysql_row);
    
    if(vec_rows.size() <= row)
      return nullptr;
    return vec_rows[row][column];
  }

private:
  void Connect(std::string host, std::string database, std::string user, std::string password, uint32_t port) {
    try {
      mysql_init(&mysql_);
     
      if (!mysql_real_connect(&mysql_, host.c_str(), user.c_str(), password.c_str(), database.c_str(), port, NULL, 0)) {
        std::cout << "mysql_real_connect err" << mysql_error(&mysql_) << std::endl;
      }
    } catch (...) {
      std::cout << mysql_error(&mysql_) << std::endl;
    }
  }

private:
  MYSQL mysql_;
  MYSQL_RES* res_;

  uint32_t status_;
  std::string err_;
};
