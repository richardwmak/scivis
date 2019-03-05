#include "config.hpp"
#include "controller.hpp"

int main(int, char **)
{
    Controller *controller = new Controller();
    controller->begin();

    return 0;
}