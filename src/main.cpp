#include "config.hpp"
#include "controller.hpp"

int main(int argc, char **argv)
{
    Controller *controller = new Controller();
    controller->begin();

    return 0;
}