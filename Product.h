#ifndef PRODUCT_H
#define PRODUCT_H

#include<string>
using namespace std;
class Product
{
	private:
		int id;
		string name;
		double price;
	public:
		Product() : Product(0, "", 0.0)
		{
			
		}
		Product(int id, string name, double price)
		{
			this->id = id;
			this->name = name;
			this->price = price;
		}
		int getId()
		{
			return id;
		}
		void setId(int id)
		{
			this->id = id;
		}
		string getName()
		{
			return name;
		}
		void setName(string name)
		{
			this->name = name;
		}
		double getPrice()
		{
			return price;
		}
		void setPrice(double price)
		{
			this->price = price;
		}
};

#endif
