#include <gtest/gtest.h>
#include <thread>
#include <mutex>

#include "spinlock.hpp"

TEST(one_thread, try_lock) {
    threads_sync::spinlock s;
    ASSERT_EQ(s.try_lock(), true);
    ASSERT_EQ(s.try_lock(), false);
    ASSERT_EQ(s.try_lock(), false);
}

TEST(one_thread, lock) {
    threads_sync::spinlock s;
    s.lock();
    ASSERT_EQ(s.try_lock(), false);
}

TEST(one_thread, unlock) {
    threads_sync::spinlock s;
    s.lock();
    s.unlock();
    ASSERT_EQ(s.try_lock(), true);
}

TEST(one_thread, multiple_lock) {
    threads_sync::spinlock s;
    s.lock();
    ASSERT_EQ(s.try_lock(), false);
    s.unlock();
    s.lock();
    ASSERT_EQ(s.try_lock(), false);
}

namespace {

size_t foo_n = 0;
size_t bar_n = 0;


void foo() {
    static threads_sync::spinlock s;
    for (size_t k = 0; k < 1000000; ++k) {
        s.lock();
        ++foo_n;
        s.unlock();
    }
}

void bar() {
    static threads_sync::spinlock s;
    for (size_t k = 0; k < 1000; ++k) {
        s.lock();
        for (size_t i = 0; i < 1000; ++i) {
            ++bar_n;
        }
        s.unlock();
    }
}

} // namespace

TEST(multiple_threads, data_races_short_lock) {
    std::thread t1(foo);
    std::thread t2(foo);
    t2.join();
    t1.join();
    ASSERT_EQ(foo_n, 2000000);
}

TEST(multiple_threads, data_races_long_lock) {
    std::thread t1(bar);
    std::thread t2(bar);
    std::thread t3(bar);
    t3.join();
    t2.join();
    t1.join();
    ASSERT_EQ(bar_n, 3000000);
}

TEST(std, lock_guard) {
    threads_sync::spinlock s;
    {
    std::lock_guard<threads_sync::spinlock> lock(s);
    ASSERT_EQ(s.try_lock(), false);
    }
    ASSERT_EQ(s.try_lock(), true);
}


int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}