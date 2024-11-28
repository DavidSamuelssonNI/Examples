#include "mymodel.h"
#include <cmath>

MyModel::MyModel() {
    // Constructor
}

MyModel::~MyModel() {
    // Destructor
}

int MyModel::output(int time,int frequency) {
    return (int)(sin(2 * 3.14 * frequency * time) * 100);
}