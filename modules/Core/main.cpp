#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <assert.h>

//#include <opencv2/opencv.hpp>

#include "Core.hpp"

using namespace std;
using namespace cv;



int main(int argc, char* args[]){
    Matx<float, 4, 4> a = Matx<float, 4, 4>::ones();
    Matx<float, 4, 2> b = Matx<float, 4, 2>::all(3);

//    Matx<float, 4, 1> c = a * b;
//    cout << c << endl;
//cout << setBreakOnError(true) << endl;
    Vec<float, 2> d = Vec<float, 2>(4, 3);
//    try{
        d.cross(Vec<float, 2>(1, 5));
//    } catch (const avr::Exception&) {
//        cout << "\n\neee\n\n";
//    }
//    cout << d;

   return 0;
}
