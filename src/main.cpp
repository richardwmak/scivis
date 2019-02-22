#include "config.h"
#include "controller.h"

int main(int argc, char **argv)
{
    Tramp::glob_controller->begin(argc, argv);
    return 0;
}