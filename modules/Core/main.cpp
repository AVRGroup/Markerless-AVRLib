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
#define C 3
#define L 3


   return 0;
}
