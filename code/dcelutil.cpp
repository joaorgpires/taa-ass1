#include "dcelutil.h"
#include <iostream>

using namespace std;

int main() {
  double x1, y1, x2, y2;

  cin >> x1 >> y1 >> x2 >> y2;
  Point v1 = Point(x1, y1), v2 = Point(x2, y2);

  cout << v1.X << " " << v2.X << endl;

  cout << v1.Y << " " << v2.Y << endl;

  return 0;
}
