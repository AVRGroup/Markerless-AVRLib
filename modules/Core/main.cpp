#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <time.h>

//#include <opencv2/opencv.hpp>

#include "Core.hpp"

using namespace std;
using namespace cv;

/**
 * Construtores e Métodos Matx<> [OK]
 * Construtores e Métodos Vec<> [OK]
 * Funções estáticas e operadores Matx<> [OK]
 * Funções estáticas e operadores Vec<> [OK]
 */

// Operações dos Points (alpha-> int, float, double)
// = += -=  (Point, Point)
// *=       (Point, alpha)
// + -      (Point, Point)
// *        (Point, alpha) (alpha, Point)
// == !=    (Point, Point)
// neg      -PointObject
// <<       cout << PointObject
// >>       cin >> PointObject
// Funções: norm(PointObject)

// Operações do Size
// = += -=  (Size, Size)
// + -      (Size, Size)
// *        (Size, Tp) (Tp, Size)
// == !=    (Size, Size)
// <<       cout << SizeObject
// >>       cin >> SizeObject

// Operações do Rect
// = &= |=  (Rect, Rect)
// += -=    (Rect, Rect), (Rect, Size)
// +        (Rect, Point2), (Rect, Size)
// -        (Rect, Point2)
// & |      (Rect, Rect)
// == !=    (Rect, Rect)
// <<       cout << RectObject
// >>       cin >> RectObject

// Operações do RotatedRect
// =        (RotatedRect, RotatedRect)
// <<       cout << RotatedObject
// >>       cin >> RotatedObject

// Operações do Range (delta-> int)
// =        (Range, Range)
// + -      (Range, delta)
// &= &     (Range, Range)
// == !=    (Range, Range)
// !        (Range)
// <<       cout << RangeObject
// >>       cin >> RangeObject

// Operações do Matx (alpha->int, float, double)
// =        (Matx, Matx)
// += -=    (Matx<T1>, Matx<T2>)
// *=       (Matx, alpha)
// + -      (Matx, Matx)
// *        (Matx, alpha) (alpha, Matx) (Matx<m, n>, Matx<n, m>)
//          (Matx, Vec) (Matx<2,2>, Point2) (Matx<3,3>, Point3)
//          (Matx<3,3>, Point2), (Matx<4,4>, Point3) (Matx<4,4>, Scalar)
// ==, !=   (Matx, Matx)
// neg      -MatxObject
// Funções: determinant(Matx), trace(Matx)

// Operações do Vec (alpha-> int, float, double)
// =        (Vec, Vec)
// += -=    (Vec<T1>, Vec<T2>)
// *= /=    (Vec, alpha)
// +, -,    (Vec, Vec)
// *        (Vec, alpha), (alpha, Vec), (Vec<4>, Vec<4>)
// /        (Vec, alpha)
// neg      -VecObject
// Funções: normalize(Vec)

// Operações Scalar
// = += -=  (Scalar, Scalar)
// *= /=    (Scalar, Tp) (Scalar, Scalar)
// + -      (Scalar, Scalar)
// * /      (Scalar, Tp) (Tp, Scalar) (Scalar, Scalar)
// == !=    (Scalar, Scalar)
// neg      -ScalarObject
// <<       cout << ScalarObject
// >>       cin >> ScalarObject

int main(int argc, char* args[]){
//    int boolean = 0;
//    Size s1 = Size(2, 3);
//    Point p1 = Point(5, 10);
//    Point p2 = Point(s1);
//
//    Point2f p3 = Point2f(1.5, 3.5);
//
//    Vec2i v = Vec2i(4, 6);
//    Vec3i vec3 = Vec3i(7, 8, 9);
//    Point p4 = Point(v);
//
//    Point3i p3i1 = Point3i(1, 1, 1);
//    Point3i p3i2 = p3i1;
//    Point3i p3i3 = Point3i(5, 3, 4);
//
//    p3i2 = p3i2.cross(p3i3);
//    cv::Size s2 = Size();
//    cv::Size2f sf = Size2f(4.1, 5.3);
//
//
//    sf=s1;
//
//    cout.flags(ios::fixed | ios::showpoint);
//    cout.precision(2);
//    cout << sf.width;

//    Rect_<int> r = Rect_<int>(0, 0, 5, 10);
//    Rect_<int> r2 = Rect_<int>(r);
//    Rect_<int> r3 = Rect_<int>(p1, s1);
//    Rect_<int> r4 = Rect_<int>(p2, p1);
//    Rect_<int> r5 = r4;
//    Rect_<int> r6 = Rect_<int>(1.1, 5.2, 10.1, 20.2);
//
//    r4 = r6;
//    s1 = r2.size();
//
//    Range ran = Range(3, 5);
//    Range ran2;
//    ran2 = Range::all();
//
//    if(ran.empty())
//        boolean = 1;
//
//    cout << p4.x << ' ' << p4.y << endl;
//    cout << p1.x << ' ' << p1.y << endl;
//    cout << p2.x << ' ' << p2.y << endl;
//    cout << p3.x << ' ' << p3.y << endl;
//    cout << s1.width << ' ' << s1.height << endl;
//    cout << r2.width << ' ' << r2.height << endl;
//    cout << r3.width << ' ' << r3.height << endl;
//    cout << r4.width << ' ' << r4.height << ' ' << r4.x << ' ' << r4.y << endl;
//    cout << boolean << endl;
//    cout << p1.inside(r) << endl;
//    cout << p3i1.x << ' ' << p3i1.y << ' ' << p3i1.z << endl;
//    cout << p3i2.x << ' ' << p3i2.y << ' ' << p3i2.z << endl;
//    cout << vec3.cols << ' ' << vec3.rows << endl;
//    cout << p3i3.ddot(p3i1);
//    cout << ran.start << ran.end << endl;
//    cout << ran2.start << ' ' << ran2.end << endl;
//    cout << ran2.size() << endl;

    /// Scalar_
    // Scalar <=> Scalar_<float>
    Scalar s1; // construtor default, tudo = 0
    Scalar s2(1.0f); // construtor de inicialização 1
    Scalar s3(1.0f, 1.0f); // inicialização 2
    Scalar s4(1.0f, 1.0f, 0.5f); // inicialização 3
    Scalar s5(1.0f, 1.0f, 0.5f, 0.3f); // inicialização 4
    Scalar s6(s5); // construtor de cópia

    Scalar_<uint32> s7(s5); // construtor de conversão
    Scalar_<double> s8 = s7; // conversão por atribuição

    cout << s1 << s2 << s3 << s4 << s5 << s6 << s7 << s8 << endl;
    cout << s2.isReal() << "  " << s3.isReal() << endl;

    s1 = s5;
    s2 = Scalar::all(-1.0f);
    s3 = s3.mul(s2);
    s4 = s5.conj();
    s7 = s4; // operador de conversão

    cout << s1 << s2 << s3 << s4 << s7 << endl;

    /// RotatedRect
    RotatedRect rr1;
    RotatedRect rr2(Point2f(1.0f, 1.0f), Size2f(1.0f, 1.0f), 45);

    cout << endl << rr1 << rr2 << endl;

    Point2f vertex[4];
    rr2.points(vertex);
    cout << vertex[0] << vertex[1] << vertex[2] << vertex[3] << endl;

    cout << rr2.boundingRect() << endl;

   return 0;
}
