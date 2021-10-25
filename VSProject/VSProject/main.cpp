#include "project.h"
#include <stdio.h>
#include <string>

int main(int argc, char* argv[]) {

    std::cout << "Starting up the application" << std::endl;

    if (argc != 4)
        throw std::runtime_error("Need three arguments - "
            "number of sheep, number of wolves, "
            "simulation time\n");

    init();

    unsigned v1 = std::stoul(argv[1]);

    std::cout << "Done with initilization" << std::endl;

    application my_app(v1, std::stoul(argv[2]));

    std::cout << "Created window" << std::endl;

    int retval = my_app.loop(std::stoul(argv[3]));

    std::cout << "Exiting application with code " << retval << std::endl;

    my_app.~application();

    return retval;
}