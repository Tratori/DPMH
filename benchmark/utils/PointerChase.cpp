#include "../../io/utils/Mean.hpp"
#include "Exceptions.hpp"

#include <vector>
#include <numeric>
#include <random>

void writePointerChase(int numPages, int pageSize)
{
    ensure(numPages >= 1);
    std::vector<int> random_numbers(numPages);

    std::iota(random_numbers.begin(), random_numbers.end(), 0);
    auto rng = std::mt19937{42};
    std::shuffle(random_numbers.begin(), random_numbers.end(), rng);

    std::vector<int> pointers(numPages);
    pointers.reserve(numPages);
    auto zero_it = std::find(random_numbers.begin(), random_numbers.end(), 0);
    unsigned curr = 0;
    for (auto behind_zero = zero_it + 1; behind_zero < random_numbers.end(); behind_zero++)
    {
        pointers[curr] = *behind_zero;
        curr = *behind_zero;
    }
    for (auto before_zero = random_numbers.begin(); before_zero <= zero_it; before_zero++)
    {
        pointers[curr] = *before_zero;
        curr = *before_zero;
    }
    auto write_to_file = [&]()
    {
        mean::task::scheduleTaskSync(
            [&]()
            {
                auto buffer = (char *)mean::IoInterface::allocIoMemoryChecked(pageSize, 512);
                auto &channel = mean::exec::ioChannel();

                for (unsigned i = 0; i < numPages; i++)
                {
                    memcpy(buffer, &pointers.at(i), sizeof(int));
                    channel.pushBlocking(mean::IoRequestType::Write, buffer, pageSize * i, pageSize);
                }
                mean::IoInterface::freeIoMemory(buffer, pageSize);
            });

        mean::env::shutdown();
    };
    mean::env::start(write_to_file);
    mean::env::join();
}
