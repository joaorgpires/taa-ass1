#include "dcelutil.h"
#include <iostream>

using namespace std;

double dist(Vertex v1, Vertex v2) 
{
  double q1 = v2.X - v1.X, q2 = v2.Y - v1.Y;

  return sqrt(q1 * q1 + q2 * q2);
}

int main() 
{
  double x1, y1, x2, y2;

  cin >> x1 >> y1 >> x2 >> y2;
  Vertex v1 = Vertex(x1, y1), v2 = Vertex(x2, y2);

  cout << dist(v1, v2) << endl;
  
  return 0;
}
