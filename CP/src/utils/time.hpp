#pragma once

/// C includes
#include <sys/resource.h>
#include <stdlib.h>

namespace timing {

    // Returns the current time
    double now();

    // Returns the elapsed time between 'begin' and 'end' in seconds
    double elapsed_time(const double& begin, const double& end, size_t n_threads = 1);
}

