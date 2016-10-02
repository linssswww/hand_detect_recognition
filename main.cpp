#include <iostream>
#include "hand_detect.h"

using namespace std;

int main() {
    hand_detect hd;
    hd.meanshift_init();
    return 0;
}
