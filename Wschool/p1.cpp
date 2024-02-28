#include <iostream>
using namespace std;


struct car {
    int myNum;
    string myString;
    };

void fuction1(){

    car var1;
    var1.myNum = 1000;
    cout << var1.myNum << "Fuction generated"<<"\n";
    cout << "Fuction generated"<<"\n";
};


int main(){
    car myStructure;
    car myStructure2;
    fuction1();
    fuction1();
    fuction1();

    string food = "Pizza";
    int num1 = 100;

    cout << "Hellow World"<<"\n";
    myStructure.myNum = 1;
    myStructure.myString = "This is string";
    myStructure2.myString = "This is 2nd string";
    cout << myStructure.myString << "\n";
    cout << myStructure2.myString<< "\n";
    cout << food << "\n";
    cout << num1 << "\n";
    return 0;
}


