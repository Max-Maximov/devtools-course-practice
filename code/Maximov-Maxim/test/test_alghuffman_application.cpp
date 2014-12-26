// Copyright 2014 Maxim Maximov
#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "include/alghuffman_application.h"

using ::testing::internal::RE;
using std::vector;
using std::string;

class AppTest : public ::testing::Test {
 protected:
    virtual void SetUp() {
        args.clear();
    }

    void Arrange(std::string input) {
        FILE* f;
        f = fopen(ipath, "w");
        fprintf(f, "%s", input.c_str());
        fclose(f);
    }

    void Act(vector<string> args_) {
        vector<const char*> starts;
        starts.push_back("appname");

        for (size_t i = 0; i < args_.size(); ++i) {
            starts.push_back(args_[i].c_str());
        }
        const char** argv = &starts.front();
        int argc = static_cast<int>(args_.size()) + 1;

        output_ = app_(argc, argv);
    }

    void AssertWithFile(std::string expected) {
        FILE* f;
        char buf;
        std::string str = "";
        f = fopen(opath, "r");
        fscanf(f, "%c", &buf);
        while (!feof(f)) {
            str += buf;
            fscanf(f, "%c", &buf);
        }
        fclose(f);
        EXPECT_EQ(str, expected);
    }

    void AssertWithOutput(std::string expected) {
       EXPECT_TRUE(RE::PartialMatch(output_, RE(expected)));
    }

    const char* opath = "../code/Maximov-Maxim/test/output.txt";
    const char* ipath = "../code/Maximov-Maxim/test/input.txt";
    HuffmanAlgorithmApplication app_;
    string output_;
    vector<string> args;
};

TEST_F(AppTest, Do_Print_Help_Without_Arguments) {
    Act(args);
    AssertWithOutput("This is an application.*");
}

TEST_F(AppTest, Check_Number_Of_Arguments) {
    args = {"1", "2"};
    Act(args);
    AssertWithOutput("ERROR: Should be 3 arguments.*");
}

TEST_F(AppTest, Check_First_Argument) {
    args = {"1", "2", "3"};
    Act(args);
    AssertWithOutput("ERROR: First should be.*");
}

TEST_F(AppTest, Try_Read_From_Wrong_Path_On_Encoding) {
    args = {"encode", "2", "3"};
    Act(args);
    AssertWithOutput("Cannot read from.*");
}

TEST_F(AppTest, Try_Code_Wrong_String) {
    Arrange("qweqwrqr~qw");
    args = {"encode", ipath, opath};
    Act(args);
    AssertWithOutput("Input text can't include.*");
}

TEST_F(AppTest, Check_Succesful_Coding) {
    Arrange("anznaarer");
    args = {"encode", ipath, opath};
    Act(args);
    AssertWithOutput("Encoding succesful, result.*");
}

TEST_F(AppTest, Check_Writing_In_File_On_Encoding) {
    Arrange("anznaarer");
    args = {"encode", ipath, opath};
    Act(args);
    AssertWithFile("$a3$e1$n2$r2$z1$~11001010011110110001");
}

TEST_F(AppTest, Try_Read_From_Wrong_Path_On_Decoding) {
    args = {"decode", "2", opath};
    Act(args);
    AssertWithOutput("Cannot read from file.*");
}

TEST_F(AppTest, Try_Decode_With_Wrong_Head) {
    Arrange("$adsad$~101010101");
    args = {"decode", ipath, opath};
    Act(args);
    AssertWithOutput("Wrong head of input file.*");
}

TEST_F(AppTest, Try_Decode_With_Wrong_Body) {
    Arrange("$a1$d2$~10a1000100sd01");
    args = {"decode", ipath, opath};
    Act(args);
    AssertWithOutput("Wrong body of input file.*");
}

TEST_F(AppTest, Check_Succesful_Decoding) {
    Arrange("$a3$e1$n2$r2$z1$~11001010011110110001");
    args = {"decode", ipath, opath};
    Act(args);
    AssertWithOutput("Decoding succesful, result.*");
}

TEST_F(AppTest, Check_Writing_In_File_On_Decoding) {
    Arrange("$a3$e1$n2$r2$z1$~11001010011110110001");
    args = {"decode", ipath, opath};
    Act(args);
    AssertWithFile("anznaarer");
}
