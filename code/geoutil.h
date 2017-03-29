#ifndef geoutil_h
#define geoutil_h

#define INF 1000000000

using namespace std;

typedef long long int lld;

struct Point {
  lld X;
  lld Y;

  Point(){}
  Point(lld _x, lld _y) : X(_x), Y(_y){}
  Point operator + (const Point &p)  const { return Point(X + p.X, Y + p.Y); }
  Point operator - (const Point &p)  const { return Point(X - p.X, Y - p.Y); }
  Point operator * (double c) const { return Point(X * c, Y * c); }
  Point operator / (double c) const { return Point(X / c, Y / c); }
};

struct Segment {
  Point p1, p2;
  Segment(){}
  Segment(Point _p1, Point _p2) : p1(_p1), p2(_p2) {}
};

struct Line {
  double a, b, c; // ax + by = c
  Line(){}
  Line(lld _a, lld _b, lld _c) : a(_a), b(_b), c(_c) {}
};

typedef vector<Point> Polygon;

#endif
