#include "Config.hpp"

DEFINE_uint64(pagesize, 4096, "Pagesize");
DEFINE_uint64(n_cpu, 16, "Number of cpus to be used (threads will be pinned to 0...n_cpu)");