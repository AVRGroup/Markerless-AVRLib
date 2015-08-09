#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <time.h>

#include <opencv/core/core.hpp>

using namespace std;
using namespace avr;

int main(int argc, char* args[]){
    cv::Vec<double, 3> v1(1., 2., 2.);
    cv::Vec<double, 3> v2(2., 1., 1.);
    cout << v1.cross(v2);

    return 0;
}
