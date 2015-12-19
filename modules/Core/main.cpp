#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <time.h>

#include <avr/core/core.hpp>

using std::cout;
using std::endl;
using std::cin;
using namespace avr;


int main(int argc, char* args[]){
//   {  // Teste BRect<>
//      avr::BRect<int> rec(1, 2, 5, 7);
//      avr::BRect<int> rct(avr::Point(0, 0), avr::Size2i(5, 7));
//      avr::BRect<int> ret(avr::Point(1, 1), avr::Point(0, 7));
//      avr::BRect<int> cpy = rec;
//      avr::BRect<float> cvt = avr::BRect<float>(ret);
//
//      cout << rec << endl << rct << endl << ret << endl << cpy << endl << cvt << endl;
//
//      cout << rec.GetArea() << " " << rct.GetCenter() << " " << ret.GetSize() << endl;
//      cout << rec.Contains(avr::Point(6, 2)) << endl;
//      cout << rec.Intersects(rct) << endl;
//      cout << (rct == ret) << endl;
//      cout << (rct == rec) << endl;
//      cout << (rct != rec) << endl;
//      cout << (cvt != cvt) << endl;
//
//      cpy = rct;
//      cout << cpy << endl;
//
//      rec(5, 7, 1, 2);
//      rct(avr::Point(-1, -3), avr::Size2i(3, 2));
//      ret(avr::Point(-2, 5), avr::Point(1, 3));
//
//      cout << rec << endl << rct << endl << ret << endl;
//   }

//   { // Teste TMatx
//      TMatx m1;
//      TMatx m2 = m1;
//
//      cout << m1 << endl << m2 << endl;
//   }

//   { // Teste Quaternion
//      Quaternion q(Deg(45), 0, 0, 1);
//      Quaternion qi = q.Inv();
//      Point3d p(1, 1, 1);
//      Quaternion m = q * p * qi;
//
//      cout << q << endl;
//      cout << qi << endl;
//      cout << m << endl;
//
//      TMatx mr( {0, -1, 0, 10,   1, 0, 0, -10,  0, 0, 1, 0} );
//      Quaternion qr(Deg(90), 0, 1, 0);
//
//      TMatx mq = qr.GetMatrix();
//      Quaternion qm = mr.GetQuaternion();
//
//      Point3d p(1, 0, 0);
//
//      cout << mr << endl << endl;
//      cout << mr.Inv() << endl << endl;
//      cout << mr * mr.Inv() << endl << endl;
//
//      cout << mr * p << endl;
//      cout << mr.Inv() * p << endl;
//
//      cout << qm * p * qm.Inv() << endl << endl;
//
//      cout << qr * p * qr.Inv() << endl;
//      cout << mq * p << endl;
//   }

   return 0;
}
