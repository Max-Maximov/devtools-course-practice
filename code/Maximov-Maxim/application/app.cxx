// Copyright 2014 Maxim Maximov

#include <stdio.h>
#include <string>
#include "include/alghuffman_application.h"

int main(int argc, const char** argv) {
    HuffmanAlgorithmApplication app;
    std::string message = app(argc, argv);
    printf("%s\n", message.c_str());
    return 0;
}
