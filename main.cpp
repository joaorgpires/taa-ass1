#include <iostream>
#include "code/dcelutil.h"

using namespace std;

Polygon P;

int main() {
  int option; cin >> option;
  int n_vertices; cin >> n_vertices;

  for(int i = 0; i < n_vertices; i++) {
    double x, y; cin >> x >> y;
    Point p = Point(x, y);
    P.push_back(p);
  }

  for(int i = 0; i < n_vertices; i++)
    cout << P[i].X << " " << P[i].Y << endl;

  return 0;
}
