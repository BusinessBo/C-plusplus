#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

int main() {
    cout << "Your Dice Number: ";
    srand(time(0));
    int x = rand() % (6 - 1 + 1) + 1;
    cout << x;
    return 0;
}
