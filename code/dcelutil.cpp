#include "dcelutil.h"
#include <iostream>

using namespace std;

double dist(Vertex v1, Vertex v2) 
{
  Vertex q = Vertex(v2 - v1);

  return sqrt(q.X * q.X + q.Y * q.Y);
}

int main() 
{
  double x1, y1, x2, y2;

  cin >> x1 >> y1 >> x2 >> y2;
  Vertex v1 = Vertex(x1, y1), v2 = Vertex(x2, y2);

  cout << dist(v1, v2) << endl;
  
  return 0;
}
