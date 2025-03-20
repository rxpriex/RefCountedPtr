#include <iostream>
#include "RxPointer/RxPointer.h"

int main(){

    RxPointer<int> rx(1);
    std::cout << *rx.get_data() << std::endl;

    return 0;
}