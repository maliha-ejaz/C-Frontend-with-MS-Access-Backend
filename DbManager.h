#ifndef DBMANAGER_H
#define DBMANAGER_H

#include "Product.h"
#include "AccessDB.h"
#include<iostream>
#include<vector>
#include<map>

using namespace std;

class DbManager
{
	private:
		AccessDB db;
		
	public:
		bool connect(string dsName)
		{
			return db.connect(dsName);
		}
		
		bool insertProduct(Product p)
		{
			string sql = "INSERT INTO Products (name, price) VALUES('"+p.getName()+"', "+to_string(p.getPrice())+")";
			return db.executeQuery(sql);
		}
		
		bool deleteProduct(int productId)
		{
			string sql = "DELETE FROM Products WHERE ID="+to_string(productId);
			return db.executeQuery(sql);
		}
		
		bool updateProduct(Product p)
		{
			string sql = "UPDATE Products SET Name='"+p.getName()+"', Price="+to_string(p.getPrice())+" WHERE ID="+to_string(p.getId());
			return db.executeQuery(sql);
		}
		
		vector<Product> getAllProducts()
		{
			vector<Product> products;
			
			string sql = "SELECT * FROM Products";
			if(db.executeQuery(sql))
			{
				vector<map<string, string>> rows = db.fetchAllRows();
				for(map<string, string> row : rows)
				{
					Product p;
					p.setId(stoi(row["ID"]));
					p.setName(row["Name"]);
					p.setPrice(stod(row["Price"]));
					
					products.push_back(p);
				}
			}
			
			return products;
		}
		
		Product getProductById(int id)
		{
			string sql = "SELECT * FROM Products WHERE ID = "+to_string(id);
			if(db.executeQuery(sql))
			{
				map<string, string> row = db.fetchRow();
				Product p;
				p.setId(stoi(row["ID"]));
				p.setName(row["Name"]);
				p.setPrice(stod(row["Price"]));
				
				return p;
			}
		}
		
		vector<Product> getProductsByName(string name)
		{
			vector<Product> products;
			
			string sql = "SELECT * FROM Products WHERE Name LIKE'%"+name+"%'";
			if(db.executeQuery(sql))
			{
				vector<map<string, string>> rows = db.fetchAllRows();
				for(map<string, string> row : rows)
				{
					Product p;
					p.setId(stoi(row["ID"]));
					p.setName(row["Name"]);
					p.setPrice(stod(row["Price"]));
					
					products.push_back(p);
				}
			}
			
			return products;
		}
		
		void disconnect()
		{
			db.disconnect();
		}
		
		bool validateLogin(string username, string password)
		{
			string sql = "SELECT * FROM Users WHERE Username='"+username+"' AND Password='"+password+"'";
			if(db.executeQuery(sql))
			{
				map<string, string> row = db.fetchRow();
				if(!row.empty())
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			
			return false;
		}
};

#endif
