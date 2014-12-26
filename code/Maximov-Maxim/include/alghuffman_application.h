// Copyright 2014 Maxim Maximov

#ifndef CODE_MAXIMOV_MAXIM_INCLUDE_ALGHUFFMAN_APPLICATION_H_
#define CODE_MAXIMOV_MAXIM_INCLUDE_ALGHUFFMAN_APPLICATION_H_

#include <string>

class HuffmanAlgorithmApplication {
 public:
    HuffmanAlgorithmApplication();
    std::string operator()(int argc, const char** argv);
 private:
    void parse_and_Run(int argc, const char** argv);
    void help(const char* appname);
    std::string message_;
};

#endif  // CODE_MAXIMOV_MAXIM_INCLUDE_ALGHUFFMAN_APPLICATION_H_
