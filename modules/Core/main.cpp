#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <time.h>

//#include <opencv2/opencv.hpp>

#include "Core.hpp"

using namespace std;
using namespace cv;

template <typename Tp, int r, int c>
static inline ostream& operator << (ostream& out, Matx<Tp, r, c> m) {
   for(int i = 0; i < c; i++) {
      out << "[";
      for(int j = 0; j < r; j++)
         out << m(j, i) << ", ";
      out << "\b\b]" << endl;
   }
   return out << endl;
}

/**
 * Construtores e Métodos Matx<> [OK]
 * Construtores e Métodos Vec<> [OK]
 * Funções estáticas e operadores Matx<> [OK]
 * Funções estáticas e operadores Vec<> [OK]
 */

// Operações do Vec (alpha-> int, float, double)
// += -= (Vec<T1>, Vec<T2>)
// *= /= (Vec, alpha)
// +, -, (Vec, Vec)
// *     (Vec, alpha), (alpha, Vec), (Vec<4>, Vec<4>)
// /     (Vec, alpha)
// neg
// normalize(Vec)

// Operações do Matx (alpha->int, float, double)
// += -= (Matx<T1>, Matx<T2>)
// *=    (Matx, alpha)
// + -
// *     (Matx, alpha) (alpha, Matx) (Matx<m, n>, Matx<n, m>)
//       (Matx, Vec) (Matx<2,2>, Point2) (Matx<3,3>, Point3)
//       (Matx<3,3>, Point2), (Matx<4,4>, Point3) (Matx<4,4>, Scalar)
// ==, !=
// neg
// determinant, trace

int main(int argc, char* args[]){

    int boolean;
    Size s1 = Size(2, 3);
    Point p1 = Point(2, 4);
    Point p2 = Point(s1);

    Point2f p3 = Point2f(1.5, 3.5);

    Vec2i v = Vec2i(4, 6);
    Vec3i vec3 = Vec3i(7, 8, 9);
    Point p4 = Point(v);

    Point3i p3i1 = Point3i(1, 1, 1);
    Point3i p3i2 = p3i1;
    Point3i p3i3 = Point3i(5, 3, 4);

    p3i2 = p3i2.cross(p3i3);

//    cv::Size s2 = Size();
//    cv::Size2f sf = Size2f(4.1, 5.3);
//
//
//    sf=s1;
//
//    cout.flags(ios::fixed | ios::showpoint);
//    cout.precision(2);
//    cout << sf.width;

    Rect_<int> r = Rect_<int>(0, 0, 5, 10);
    Rect_<int> r2 = Rect_<int>(r);
    Rect_<int> r3 = Rect_<int>(p1, s1);
    Rect_<int> r4 = Rect_<int>(p2, p1);
    Rect_<int> r5 = r4;
    Rect_<int> r6 = Rect_<int>(1.1, 5.2, 10.1, 20.2);

    r4 = r6;
    s1 = r2.size();

    Range ran = Range(3, 5);
    Range ran2;
    ran2 = ran.all();

    if(ran.empty())
        boolean = 1;

//    cout << p4.x << ' ' << p4.y << endl;
//    cout << p1.x << ' ' << p1.y << endl;
//    cout << p2.x << ' ' << p2.y << endl;
//    cout << p3.x << ' ' << p3.y << endl;
//    cout << s1.width << ' ' << s1.height << endl;
//    cout << r2.width << ' ' << r2.height << endl;
//    cout << r3.width << ' ' << r3.height << endl;
//    cout << r4.width << ' ' << r4.height << ' ' << r4.x << ' ' << r4.y << endl;
    cout << boolean << endl;
//    cout << p1.inside(r);
//    cout << p3i1.x << ' ' << p3i1.y << ' ' << p3i1.z << endl;
//    cout << p3i2.x << ' ' << p3i2.y << ' ' << p3i2.z << endl;
//    cout << vec3.cols << ' ' << vec3.rows << endl;
//    cout << p3i3.ddot(p3i1);
    cout << ran.start << ' ' << ran.end << endl;
    cout << ran2.start << ' ' << ran2.end << endl;
    cout << ran.size() << endl;

   return 0;
}
