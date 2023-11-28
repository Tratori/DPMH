#include <stdio.h>
#include <iostream>

#include "../io/IoOptions.hpp"
#include "../io/utils/Mean.hpp"
#include "utils/PointerChase.cpp"
#include "Config.hpp"

void runPointerChasing()
{
    auto num_chases = 1'000'000u;
    auto chasePointerFun = [](mean::BlockedRange bb, std::atomic<bool> &cancelled)
    {
        auto buffer = (char *)mean::IoInterface::allocIoMemoryChecked(FLAGS_pagesize, 512);
        auto &channel = mean::exec::ioChannel();
        auto curr_page = bb.begin % 1000; // TODO: random starting page.
        for (int i = bb.begin; i < bb.end; i++)
        {
            channel.pushRead(buffer, FLAGS_pagesize * curr_page, FLAGS_pagesize, {[](mean::IoBaseRequest * /**/) {}});
            channel.submit();
            while (!channel.poll())
                ;

            memcpy(&curr_page, buffer, sizeof(int));
        }
        mean::IoInterface::freeIoMemory(buffer, FLAGS_pagesize);
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
    ioOptions.channelCount = 32;
    ioOptions.raid5 = false;
    // ioOptions.iodepth = 32;

    mean::env::init(32, 0, ioOptions, 0, 16);
    writePointerChase(1000, FLAGS_pagesize);
    auto workerThreads = 8;

    mean::env::adjustWorkerCount(32, 16);
    mean::env::start(runPointerChasing);
    mean::env::join();
    std::cout << "finished \n";
}