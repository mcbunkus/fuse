#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>

#include "fuse/threadpool.hpp"

void Task(const std::string &name)
{
    std::cout << "Hello, " << name << "!\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 10));
    std::cout << "Goodbye, " << name << " :(\n";
}

int main(int argc, char *argv[])
{
    srand(time(nullptr));

    fuse::ThreadPool pool{};

    std::vector<std::string> names{"Alpha", "Bravo", "Charlie",
                                   "Delta", "Echo",  "Foxtrot"};

    std::vector<std::future<void>> results;
    for (std::string name : names)
    {
        results.emplace_back(pool.enqueue(Task, name));
    }

    for (auto &result : results)
    {
        result.get();
    }

    return 0;
}
