#include <iostream>
#include <cstdlib>
#include <cstdio>

#include "Point.hpp"
#include "Bounding.hpp"
#include "SafePointer.hpp"
#include "TransformationMatrix.hpp"

#include <opencv2/opencv.hpp>

using namespace std;
using namespace avr;

class Base {
public:
   int x;
   Base(int _x) : x(_x){}
   virtual ~Base(){cout << "dtor1\n";}

   friend ostream& operator << (ostream& out, const Base& b) {
      out << b.x;
      return out;
   }
};

class Derivated : public Base {
public:
   int y;
   Derivated(int x, int y = 0) : Base(x), y(y) {}
   ~Derivated(){cout << "dtor\n"; }

   friend ostream& operator << (ostream& out, const Derivated& d) {
      out << d.x << " " << d.y;
      return out;
   }
};

#define ASSERT(e) (e) ? (void) (cout << "Assertion failed (" << #e << ") in " << __FILE__ << ":" << __LINE__) : (void) 0

int main(int argc, char* args[]){
   // = += -= + - neg == !=

   Point2i p1, p2(4, 3), p3 = (p1 -p2) * 2;
   p1 -= p3 + p3;

   // QUE COISA LINDA *-----*
   ASSERT(p1 != p2);

//   Point3i p5 = Point3i(p1);
//   Point2d p4(p5);
//
//   cout << p5 << " " << p4 << " " << p3 << " " << ((p1) != (p1)) << " " << !p1 << endl;
//
//   SafePointer<Point2i> p = (&p2 + 0xA4);
//   cout << p;

   return 0;
}
