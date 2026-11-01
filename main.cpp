#include<iostream>
#include"Product.h"
#include "DbManager.h"
#include<vector>
#include<conio.h>
using namespace std;

DbManager manager;

string getHiddenInput()
{
		string str = "";
	
	//cout<<"Enter a character: ";
	
	while(1)
	{
		char ch = getch();
		if(ch == '\r')
		{
			//representing enter key
			cout<<endl;
			break;
		}
		else if(ch == '\b')
		{
			//representing backspace key
			cout<<"\b \b";
			//cursor back->space entered->cursor back again
		}
		else
		{
			cout<<'*';
			str = str + ch;
		}		
	}
	
	return str;
}

bool login()
{
	string username;
	string password;
	
	cout<<"==LOGIN==\n";
	cout<<"Enter username: ";
	getline(cin, username);
	cout<<"Enter password: ";
	password = getHiddenInput();
	
	//check database
	
	if(manager.validateLogin(username, password))
	{
		cout<<"Login Successful\n";
		return true;
	}
	else
	{
		cout<<"Invalid Username\Password\n";
		return false;
	}
}

void addProduct()
{
	string name;
	double price;
	cout<<"Enter Product Name: ";
	getline(cin, name);
	cout<<"Enter Product Price: ";
	cin>>price;
	
	Product p;
	p.setName(name);
	p.setPrice(price);
	
	//Create Database
	if(manager.insertProduct(p))
	{
		cout<<"Product saved in database successfully\n";
	}
	else
	{
		cout<<"Failed to save product in database\n";
	}
}
void viewAllProducts()
{
	//Get Data from Database
	
	vector<Product> products = manager.getAllProducts();
	for(Product p : products)
	{
		cout<<"ID: "<<p.getId()<<"\n";
		cout<<"Name: "<<p.getName()<<"\n";
		cout<<"Price: "<<p.getPrice()<<"\n";
		cout<<"------------\n";
	}
}
void updateProduct()
{
	int id;
	cout<<"Enter Product ID to update: ";
	cin>>id;
	cin.ignore();
	string name;
	double price;
	cout<<"Enter Updated Name: ";
	getline(cin, name);
	cout<<"Enter Updated Price: ";
	cin>>price;
	
	Product p;
	p.setId(id);
	p.setName(name);
	cin.ignore();
	p.setPrice(price);
	
	//Update Record in Database
	if(manager.updateProduct(p))
	{
		cout<<"Product updated in database successfully\n";
	}
	else
	{
		cout<<"Failed to update product in database\n";
	}
}
void deleteProduct()
{
	int id;
	cout<<"Enter Product ID to delete: ";
	cin>>id;
	
	//Delete Record from Database	
	if(manager.deleteProduct(id))
	{
		cout<<"Product deleted from database successfully\n";
	}
	else
	{
		cout<<"Failed to delete product from database\n";
	}
}

void searchProductById()
{
	int id;
	cout<<"Enter Product ID to search: ";
	cin>>id;
	cin.ignore();
	
	Product p = manager.getProductById(id);
	if(p.getId() != 0)
	{
		cout<<"\n---Product Found---\n";
		cout<<"ID: "<<p.getId()<<"\n";
		cout<<"Name: "<<p.getName()<<"\n";
		cout<<"Price: "<<p.getPrice()<<"\n";
		cout<<"-----------\n";
	}
	else
	{
		cout<<"Product Not Found with ID: "<<id<<"\n";
	}
}

void searchProductByName()
{
	string name;
	cout<<"Enter Product Name to search: ";
	getline(cin, name);
	
	//get products from database
	vector<Product> products = manager.getProductsByName(name);
	for(Product p : products)
	{
		cout<<"ID: "<<p.getId()<<"\n";
		cout<<"Name: "<<p.getName()<<"\n";
		cout<<"Price: "<<p.getPrice()<<"\n";
		cout<<"----------\n";
	}
}

int main()
{
	if(!manager.connect("MyDataSource"))
	{
		cout<<"DB Connection Failed\n";
		return 1;
	}
	else
	{
		cout<<"DB Connection Successful\n";
	}
	
	manager.connect("MyDataSource");
	if(!login())
	{
		cout<<"Exiting....\n";
		return 0;
	}
	
	
	while(1)
	{
		cout<<"===Product Management System===\n";
		cout<<"1. Add Product\n";
		cout<<"2. View All Products\n";
		cout<<"3. Update Product\n";
		cout<<"4. Delete Product\n";
		cout<<"5. Search By ID\n";
		cout<<"6. Search By Name\n";
		cout<<"0. Exit\n";
		int choice;
		cout<<"Enter Choice: ";
		cin>>choice;
		cin.ignore();
		
		if(choice == 0)
		{
			cout<<"Exiting...\n";
			break;
		}
		else if(choice == 1)
		{
			addProduct();
		}
		else if(choice == 2)
		{
			viewAllProducts();
		}
		else if(choice == 3)
		{
			updateProduct();
		}
		else if(choice == 4)
		{
			deleteProduct();
		}
		else if(choice == 5)
		{
			searchProductById();
		}
		else if(choice == 6)
		{
			searchProductByName();
		}
		else
		{
			cout<<"Invalid Choice!!!\n";
		}
		
	}
	
	manager.disconnect();
	
	return 0;
}
