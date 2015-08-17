#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <time.h>

#include <avr/core/Core.hpp>

using std::cout;
using std::endl;
using std::cin;
using namespace avr;


int main(int argc, char* args[]){
   {  // Teste BRect<>
      BRect<int> rec(1, 2, 5, 7);
      BRect<int> rct(Point(0, 0), Sizei(5, 7));
      BRect<int> ret(Point(1, 1), Point(0, 7));
      BRect<int> cpy = rec;
      BRect<float> cvt = BRect<float>(ret);

      cout << rec << endl << rct << endl << ret << endl << cpy << endl << cvt << endl;

      cout << rec.GetArea() << " " << rct.GetCenter() << " " << ret.GetSize() << endl;
      cout << rec.Contains(Point(6, 2)) << endl;
      cout << rec.Intersects(rct) << endl;
      cout << (rct == ret) << endl;
      cout << (rct == rec) << endl;
      cout << (rct != rec) << endl;
      cout << (cvt != cvt) << endl;

      cpy = rct;
      cout << cpy << endl;

      rec(5, 7, 1, 2);
      rct(Point(-1, -3), Sizei(3, 2));
      ret(Point(-2, 5), Point(1, 3));

      cout << rec << endl << rct << endl << ret << endl;
   }

   { // Teste TMatx
      TMatx m1;
      TMatx m2 = m1;

      cout << m1 << endl << m2 << endl;
   }

   { // Teste Quaternion
      Quaternion q(Deg(45), 0, 0, 1);
      Quaternion qi = q.Inv();
      Point3d p(1, 1, 1);
      Quaternion m = q * p * qi;

      cout << q << endl;
      cout << qi << endl;
      cout << m << endl;
   }

//   TMatx tm( {0, -1, 0, 0, 1, 0, 0, 0, 0, 0, 1} );
//   Point3d p(0, 0, 1);
//   Quaternion q(Deg(90), 1, 1, 1);
//   TMatx tm = q.GetMatrix();
//
//   cout << tm.Get() << endl;
//   cout << q << endl;
//   cout << q * p * q.Inv() << endl;

   return 0;
}
