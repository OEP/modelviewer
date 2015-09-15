#include <iostream>
#include <fstream>

#include "model.h"

int main(int argc, char **argv)
{
    std::ifstream is("/home/pkilgo/models/unitsphere.obj");
    Model::read(is);
    return 0;
}
