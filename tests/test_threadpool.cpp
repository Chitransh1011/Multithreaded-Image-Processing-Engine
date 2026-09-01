#include "ThreadPool.h"
#include "TestHelpers.h"

#include <future>
#include <vector>

int main() {
    test::expectThrows([] { imaging::ThreadPool invalidPool(0); },
                       "zero worker count should be rejected");

    imaging::ThreadPool pool(2);
    test::expect(pool.workerCount() == 2, "pool should create the requested worker count");

    std::vector<std::future<int>> results;
    for (int value = 1; value <= 16; ++value) {
        results.push_back(pool.submit([value] { return value * value; }));
    }

    int total = 0;
    for (std::future<int>& result : results) {
        total += result.get();
    }
    test::expect(total == 1496, "all queued tasks should execute and return through futures");
    return test::finish();
}
