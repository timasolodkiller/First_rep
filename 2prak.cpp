#include <iostream>
#include "Date.h"

using namespace std;

int main() {
  // Test 1: Default constructor
  Date d1;
  cout << "d1 (default constructor): " << d1 << endl;

  // Test 2: Parameterized constructor
  Date d2(14, 3, 2023);
  cout << "d2 (parameterized constructor): " << d2 << endl;

  // Test 3: Copy constructor
  Date d3 = d2;
  cout << "d3 (copy constructor): " << d3 << endl;

  // Test 4: Assignment operator
  Date d4;
  d4 = d2;
  cout << "d4 (assignment operator): " << d4 << endl;

  // Test 5: Accessors
  cout << "d2.day(): " << d2.getDay() << endl;
  cout << "d2.month(): " << d2.getMonth() << endl;
  cout << "d2.year(): " << d2.getYear() << endl;

  // Test 7: Invalid input
  try {
    Date d5(29, 2, 2023); // leap year
  } catch (const std::exception& e) {
    cout << "d5 (invalid input): " << e.what() << endl;
  }
  return 0;
}