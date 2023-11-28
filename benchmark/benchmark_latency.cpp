#include <stdio.h>
#include <iostream>

#include "../io/IoOptions.hpp"
#include "../io/utils/Mean.hpp"
#include "utils/PointerChase.cpp"
#include "Config.hpp"

void runPointerChasing()
{
    auto num_chases = 1'000'000u;
    auto chasePointerFun = [&](mean::BlockedRange bb, std::atomic<bool> &cancelled)
    {
        auto &buffer = mean::IoInterface::allocIoMemoryChecked(FLAGS_pagesize, 512);
        mean::task::read()
    };

    auto bb = mean::BlockedRange{0, num_chases};
    mean::task::parallelFor(bb, chasePointerFun, 1, 0);
    mean::env::shutdown();
}

int main(int argc, char **argv)
{

    mean::IoOptions ioOptions("auto", "/home/fabian/Desktop/HPI3/Data_processing/DPMH/build/test");
    ioOptions.write_back_buffer_size = 4 * 1024;
    ioOptions.engine = "io_uring";
    ioOptions.ioUringPollMode = true;
    ioOptions.ioUringShareWq = 0;
    ioOptions.channelCount = 16;
    ioOptions.raid5 = false;
    // ioOptions.iodepth = 32;

    mean::env::init(16, 0, ioOptions, 0);
    auto &channel = mean::IoInterface::instance().getIoChannel(0);
    auto buffer = (char *)mean::IoInterface::allocIoMemoryChecked(1024 * 4, 512);

    writePointerChase(1000, FLAGS_pagesize);
    auto workerThreads = 8;

    mean::env::adjustWorkerCount(workerThreads);
    mean::env::start(runPointerChasing);
    mean::env::join();
    std::cout << "finished \n";
}