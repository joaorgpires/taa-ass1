#ifndef dcelutil_h
#define dcelutil_h

#include <math.h>

struct Vertex;
struct Face;
struct HalfEdge;

struct Vertex {  
  double X;
  double Y;
  Vertex(){}
  Vertex(double _x, double _y) : X(_x), Y(_y) {}
  Vertex operator + (const Vertex &p)  const { return Vertex(X + p.X, Y + p.Y); }
  Vertex operator - (const Vertex &p)  const { return Vertex(X - p.X, Y - p.Y); }
  Vertex operator * (double c) const { return Vertex(X * c, Y * c); }
  Vertex operator / (double c) const { return Vertex(X / c, Y / c); }
};

double dist(Vertex v1, Vertex v2);

struct Face {
  HalfEdge *f;
};

struct HalfEdge {
  Vertex *target;
  Face *incident;
  HalfEdge *twin;
  HalfEdge *prev;
  HalfEdge *next;
};

#endif
