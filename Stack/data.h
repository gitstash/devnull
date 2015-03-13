#include <string>
#include <iostream>
using namespace std;

//creating data class for future 
// to hold hetrogenous data
// for now it has only one member of string type
class Data 
{
    private: string _name;
    public:
             Data(std::string name)
             {_name = name;}
     void printData()
     {
         cout << _name ;
     }

	friend ostream& operator<<(ostream& COUT, Data& data)
	{
        COUT<< data._name <<"\t";
		COUT<<endl;
		return COUT;
	}
};
