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
   {  /* // Teste BoundingRect<>
      BoundingRect<int> rec(1, 2, 5, 7);
      BoundingRect<int> rct(Point(0, 0), Sizei(5, 7));
      BoundingRect<int> ret(Point(1, 1), Point(0, 7));
      BoundingRect<int> cpy = rec;
      BoundingRect<float> cvt = BoundingRect<float>(ret);

      cout << rec() << endl << rct() << endl << ret() << endl << cpy() << endl << cvt() << endl;

      cout << rec.GetArea() << " " << rct.GetCenter() << " " << ret.GetSize() << endl;
      cout << rec.Contains(Point(6, 2)) << endl;
      cout << rec.Intersects(rct) << endl;
      cout << (rct == ret) << endl;
      cout << (rct == rec) << endl;
      cout << (rct != rec) << endl;
      cout << (rct != ret) << endl;

      cpy = rct;
      cout << cpy() << endl;

      rec(5, 7, 1, 2);
      rct(Point(-1, -3), Sizei(3, 2));
      ret(Point(-2, 5), Point(1, 3));

      cout << rec() << endl << rct() << endl << ret() << endl;
   */ }

   { // Teste TMatx
      TMatx m1;
      TMatx m2 = m1;

      cout << m1() << endl << m2() << endl;
   }

   return 0;
}
