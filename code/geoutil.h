#ifndef geoutil_h
#define geoutil_h

using namespace std;

struct Point {
  double X;
  double Y;

  Point(){}
  Point(double _x, double _y) : X(_x), Y(_y){}
  Point operator + (const Point &p)  const { return Point(X + p.X, Y + p.Y); }
  Point operator - (const Point &p)  const { return Point(X - p.X, Y - p.Y); }
  Point operator * (double c) const { return Point(X * c, Y * c); }
  Point operator / (double c) const { return Point(X / c, Y / c); }
};

struct Segment
{
  Point p0, p1;
  Segment(){}
  Segment(Point _p0, Point _p1) : p0(_p0), p1(_p1) {}
};

struct Line
{
  double a, b, c; // ax + by = c
  Line(){}
  Line(double _a, double _b, double _c) : a(_a), b(_b), c(_c) {}
};

typedef vector<Point> Polygon;

#endif
