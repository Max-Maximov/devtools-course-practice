#############################################
##### Declaring
#############################################

set(libname  "maximov-maxim-alghuffman-library")
set(testname "maximov-maxim-alghuffman-test")
set(appname  "maximov-maxim-alghuffman-application")

message("")
message("############################################")
message("##### Testing ${libname}")
message("############################################")
message("")

#############################################
##### Building
#############################################

# Declare directory with public headers
add_dir_inc("./")

# Declare static library
add_lib_static(${libname} "include" "src")

# Declare module with unit tests
add_local_dependency(${libname})
add_app(${testname} "test")
target_link_libraries(${testname})

# Declare application
add_local_dependency(${libname})
add_app(${appname} "application")

#############################################
##### Testing
#############################################

include("CTestTests.txt")
