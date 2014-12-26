// Copyright 2014 Maxim Maximov
#include "include/alghuffman.h"
#include "include/alghuffman_application.h"
#include <cstring>
#include <string>

HuffmanAlgorithmApplication::HuffmanAlgorithmApplication() : message_("") {}

void HuffmanAlgorithmApplication::help(const char* appname) {
    message_ += std::string("This is an application for")
             + "encode and decode text with Huffman Algorithm.\n\n"
             + "Please provide arguments in the following format:\n\n"
             + "  $ " + appname + " <operation> <textfile> <outputfile>\n\n"
             + "<operation> is 'encode' or 'decode'\n"
             + "<textfile> is file for encoding or decoding\n"
             + "<outputfile> is file where will be result\n";
}

void HuffmanAlgorithmApplication::parse_and_Run(int argc, const char** argv) {
    if (argc == 1) {
        message_ = "";
        help(argv[0]);
        return;
    } else if (argc != 4) {
        message_ = "ERROR: Should be 3 arguments.\n\n";
        help(argv[0]);
        return;
    }

    if (!strcmp(argv[1], "encode")) {
        FILE* f;
        char buf;
        std::string input = "";
        if (!(f = fopen(argv[2], "r"))) {
            message_ = "Cannot read from file " +
            std::string(argv[2]) + "\n";
            return;
        }
        fscanf(f, "%c", &buf);
        while (!feof(f)) {
            input += buf;
            fscanf(f, "%c", &buf);
            if (buf == '~') {
                message_ = "Input text can't include '~'\n";
                return;
            }
        }
        fclose(f);
        HuffmanAlgorithm huff;
        std::string output = huff.code(input);
        std::string freq = huff.getFrequencies();
        f = fopen(argv[3], "w");
        fprintf(f, "%s", freq.c_str());
        fprintf(f, "%s", output.c_str());
        fclose(f);
        message_ = "Encoding succesful, result in "
                + std::string(argv[3]) + "\n";
    } else {
        if (!strcmp(argv[1], "decode")) {
            FILE* f;
            char buf;
            std::string input = "";
            std::string output = "";
            if (!(f = fopen(argv[2], "r"))) {
                message_ = "Cannot read from file " +
                std::string(argv[2]) + "\n";
                return;
            }
            HuffmanAlgorithm huff;
            fscanf(f, "%c", &buf);
            while (!feof(f)) {
                input += buf;
                if (buf == '~') {
                    try {
                        huff.setFrequencies(input);
                    }
                    catch(...) {
                        message_ = "Wrong head of input file!\n";
                        return;
                    }
                    input = "";
                }
                fscanf(f, "%c", &buf);
            }
            fclose(f);
            try {
                output = huff.decode(input);
            }
            catch(...) {
                message_ = "Wrong body of input file!\n";
                return;
            }
            f = fopen(argv[3], "w");
            fprintf(f, "%s", output.c_str());
            fclose(f);
            message_ = "Decoding succesful, result in "
                + std::string(argv[3]) + "\n";
        } else {
            message_ = "ERROR: First should be 'encode' or 'decode'.\n\n";
            help(argv[0]);
            return;
        }
    }
}

std::string HuffmanAlgorithmApplication::operator()
(int argc, const char** argv) {
    parse_and_Run(argc, argv);
    return message_;
}
