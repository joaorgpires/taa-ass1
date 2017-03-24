#ifndef dcelutil_h
#define dcelutil_h

#include <math.h>
#include <list>
#include <vector>
#include "geoutil.h"

using namespace std;

struct Vertex;
struct Face;
struct HalfEdge;

struct Vertex {
  Point coord;
  HalfEdge *outer;
};

struct Face {
  HalfEdge *outer;
  int index;
};

struct HalfEdge {
  Vertex *origin;
  Face *incident;
  HalfEdge *twin;
  HalfEdge *prev;
  HalfEdge *next;
};

#endif
