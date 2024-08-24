#include <iostream>

using namespace std;

int main() {
    int Sales = 95000;
    double State_Tax  = 0.04;
    double County_Tax = 0.02;
    double x = Sales * State_Tax;
    double y = Sales * County_Tax;
    int z = Sales - (x+y);
    cout << "Sales = $" << Sales << endl
         << "State Tax = $" << x << endl
         << "County Tax = $" << y << endl
         << "Money Left = $" << z;
    return 0;
}
