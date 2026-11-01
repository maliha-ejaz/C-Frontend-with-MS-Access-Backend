#include<iostream>
#include<conio.h>
using namespace std;

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

int main()
{
	string password;
	cout<<"Enter Password: ";
	password = getHiddenInput();
	
	cout<<"Password is: "<<password<<endl;
	
	return 0;
}
