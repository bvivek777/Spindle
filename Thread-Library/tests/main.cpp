#include "../include/Spindle.h"
#include "../include/ThreadConfig.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <atomic>
#include <chrono>
#include <vector>
#include <thread>
#include <mutex>

#define _TRAINING_ true
// binary file -> build/Thread-library/tests/spindleTest

// Test utilities
std::atomic<int> test_counter(0);
std::mutex test_mutex;
std::vector<int> execution_order;

void reset_test_state() {
    test_counter = 0;
    execution_order.clear();
}

void record_execution(int id) {
    std::lock_guard<std::mutex> lock(test_mutex);
    execution_order.push_back(id);
    test_counter++;
}

// Simple test functions
void db()
{
   std::ifstream file;
   std::string readLine;
   file.open("../Thread-Library/tests/testreadFile.txt");
   while (std::getline(file, readLine));
   record_execution(1);
}

void mem()
{
   int *n = new int[100];
   delete[] n;
   record_execution(2);
}

void math()
{
   int a = 25;
   for (int i = 0; i < 10000; i++)
   {
      a++;
   }
   record_execution(3);
}

void io()
{
   std::cout << "Some function executing context switch\n";
   record_execution(4);
}

// Test functions with IDs for tracking
void test_func_1() { record_execution(10); }
void test_func_2() { record_execution(20); }
void test_func_3() { record_execution(30); }
void test_func_4() { record_execution(40); }
void test_func_5() { record_execution(50); }

// Empty function test
void empty_func() {
    record_execution(0);
}

// Long running function
void long_running() {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    record_execution(100);
}

// Original test functions
void test_1()
{
   io();
   math();
   io();
   db();
   mem();
   math();
   math();
   db();
   io();
   math();
   db();
}

void test_2()
{
   math();
   mem();
   math();
   math();
   db();
   mem();
   math();
   db();
}

void test_3()
{
   io();
   mem();
   io();
   math();
   io();
   db();
   mem();
   math();
   math();
   db();
}

void test_4()
{
   db();
   io();
   math();
   io();
   db();
   math();
   db();
   io();
   mem();
   db();
}

void test_5()
{
   mem();
   db();
   io();
   mem();
   math();
   io();
   mem();
   db();
   io();
   math();
   db();
   mem();
   math();
   db();
}

void test_6()
{
   io();
   mem();
   io();
   db();
   mem();
   math();
   mem();
   db();
   math();
   db();
}

void test_7()
{
   io();
   math();
   io();
   db();
   mem();
   math();
   mem();
   db();
   io();
   mem();
   db();
}

void test_8()
{
   mem();
   math();
   db();
   io();
   math();
   io();
   db();
}

void test_9()
{
   mem();
   io();
   mem();
   io();
   math();
   mem();
   math();
   mem();
   db();
}

// Test suite functions
void test_fcfs_scheduling() {
    std::cout << "Testing FCFS Scheduling...\n";
    reset_test_state();
    
    Config &config = Config::getInstance(RUN_MODE::PERFORMANCE, THREAD_MODE::CONSTANT);
    config.setThreadMode(THREAD_MODE::CONSTANT);
    config.setSchedulingType(SCHEDULING::FCFS_SC);
    
    Spindle &spindle = Spindle::getInstance(&config);
    assert(spindle.init(2));
    
    spindle.addProcess(test_func_1);
    spindle.addProcess(test_func_2);
    spindle.addProcess(test_func_3);
    spindle.addProcess(test_func_4);
    
    spindle.done();
    
    // Wait for completion
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    assert(test_counter >= 4);
    std::cout << "FCFS Scheduling test passed\n";
}

void test_constant_thread_mode() {
    std::cout << "Testing CONSTANT Thread Mode...\n";
    reset_test_state();
    
    Config &config = Config::getInstance(RUN_MODE::PERFORMANCE, THREAD_MODE::CONSTANT);
    config.setThreadMode(THREAD_MODE::CONSTANT);
    config.setSchedulingType(SCHEDULING::FCFS_SC);
    
    Spindle &spindle = Spindle::getInstance(&config);
    assert(spindle.init(3));
    
    for (int i = 0; i < 6; i++) {
        spindle.addProcess(test_func_1);
    }
    
    spindle.done();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    assert(test_counter >= 6);
    std::cout << "CONSTANT Thread Mode test passed\n";
}

void test_single_thread() {
    std::cout << "Testing Single Thread...\n";
    reset_test_state();
    
    Config &config = Config::getInstance(RUN_MODE::PERFORMANCE, THREAD_MODE::CONSTANT);
    config.setThreadMode(THREAD_MODE::CONSTANT);
    config.setSchedulingType(SCHEDULING::FCFS_SC);
    
    Spindle &spindle = Spindle::getInstance(&config);
    assert(spindle.init(1));
    
    spindle.addProcess(test_func_1);
    spindle.addProcess(test_func_2);
    spindle.addProcess(test_func_3);
    
    spindle.done();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    assert(test_counter >= 3);
    std::cout << "Single Thread test passed\n";
}

void test_empty_functions() {
    std::cout << "Testing Empty Functions...\n";
    reset_test_state();
    
    Config &config = Config::getInstance(RUN_MODE::PERFORMANCE, THREAD_MODE::CONSTANT);
    config.setThreadMode(THREAD_MODE::CONSTANT);
    config.setSchedulingType(SCHEDULING::FCFS_SC);
    
    Spindle &spindle = Spindle::getInstance(&config);
    assert(spindle.init(2));
    
    for (int i = 0; i < 5; i++) {
        spindle.addProcess(empty_func);
    }
    
    spindle.done();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    assert(test_counter >= 5);
    std::cout << "Empty Functions test passed\n";
}

void test_many_functions() {
    std::cout << "Testing Many Functions...\n";
    reset_test_state();
    
    Config &config = Config::getInstance(RUN_MODE::PERFORMANCE, THREAD_MODE::CONSTANT);
    config.setThreadMode(THREAD_MODE::CONSTANT);
    config.setSchedulingType(SCHEDULING::FCFS_SC);
    
    Spindle &spindle = Spindle::getInstance(&config);
    assert(spindle.init(4));
    
    for (int i = 0; i < 20; i++) {
        spindle.addProcess(test_func_1);
    }
    
    spindle.done();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    
    assert(test_counter >= 20);
    std::cout << "Many Functions test passed\n";
}

void test_concurrent_add() {
    std::cout << "Testing Concurrent Add...\n";
    reset_test_state();
    
    Config &config = Config::getInstance(RUN_MODE::PERFORMANCE, THREAD_MODE::CONSTANT);
    config.setThreadMode(THREAD_MODE::CONSTANT);
    config.setSchedulingType(SCHEDULING::FCFS_SC);
    
    Spindle &spindle = Spindle::getInstance(&config);
    assert(spindle.init(4));
    
    std::vector<std::thread> threads;
    for (int i = 0; i < 4; i++) {
        threads.emplace_back([&spindle, i]() {
            for (int j = 0; j < 5; j++) {
                spindle.addProcess(test_func_1);
            }
        });
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    spindle.done();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    
    assert(test_counter >= 20);
    std::cout << "Concurrent Add test passed\n";
}

void test_error_handling() {
    std::cout << "Testing Error Handling...\n";
    
    Config &config = Config::getInstance(RUN_MODE::PERFORMANCE, THREAD_MODE::CONSTANT);
    config.setThreadMode(THREAD_MODE::CONSTANT);
    config.setSchedulingType(SCHEDULING::FCFS_SC);
    
    Spindle &spindle = Spindle::getInstance(&config);
    
    // Should fail - not initialized
    assert(!spindle.addProcess(test_func_1));
    
    // Should succeed after init
    assert(spindle.init(2));
    assert(spindle.addProcess(test_func_1));
    
    spindle.done();
    std::cout << "Error Handling test passed\n";
}

void test_original_suite() {
    std::cout << "Testing Original Test Suite...\n";
    reset_test_state();
    
    Config &config = Config::getInstance(RUN_MODE::PERFORMANCE, THREAD_MODE::SPINDLE);
    config.setThreadMode(THREAD_MODE::SPINDLE);
    config.setSchedulingType(SCHEDULING::ML);
    Spindle &spindle = Spindle::getInstance(&config);
    spindle.init(2);
    
    spindle.addProcess(test_1);
    spindle.addProcess(test_2);
    spindle.addProcess(test_3);
    spindle.addProcess(test_4);
    spindle.addProcess(test_5);
    spindle.addProcess(test_6);
    spindle.addProcess(test_7);
    spindle.addProcess(test_8);
    spindle.addProcess(test_9);
    
    spindle.done();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    std::cout << "Original Test Suite completed\n";
}

void test_long_running_tasks() {
    std::cout << "Testing Long Running Tasks...\n";
    reset_test_state();
    
    Config &config = Config::getInstance(RUN_MODE::PERFORMANCE, THREAD_MODE::CONSTANT);
    config.setThreadMode(THREAD_MODE::CONSTANT);
    config.setSchedulingType(SCHEDULING::FCFS_SC);
    
    Spindle &spindle = Spindle::getInstance(&config);
    assert(spindle.init(2));
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < 10; i++) {
        spindle.addProcess(long_running);
    }
    
    spindle.done();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    assert(test_counter >= 10);
    assert(duration.count() < 1000); // Should complete in less than 1 second with 2 threads
    std::cout << "Long Running Tasks test passed\n";
}

void test_mixed_functions() {
    std::cout << "Testing Mixed Functions...\n";
    reset_test_state();
    
    Config &config = Config::getInstance(RUN_MODE::PERFORMANCE, THREAD_MODE::CONSTANT);
    config.setThreadMode(THREAD_MODE::CONSTANT);
    config.setSchedulingType(SCHEDULING::FCFS_SC);
    
    Spindle &spindle = Spindle::getInstance(&config);
    assert(spindle.init(3));
    
    spindle.addProcess(io);
    spindle.addProcess(math);
    spindle.addProcess(mem);
    spindle.addProcess(db);
    spindle.addProcess(empty_func);
    spindle.addProcess(long_running);
    
    spindle.done();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    
    assert(test_counter >= 6);
    std::cout << "Mixed Functions test passed\n";
}

int main(int argc, char *argv[])
{
    std::cout << "=== Starting Comprehensive Test Suite ===\n\n";
    
    try {
        test_fcfs_scheduling();
        test_constant_thread_mode();
        test_single_thread();
        test_empty_functions();
        test_many_functions();
        test_concurrent_add();
        test_error_handling();
        test_long_running_tasks();
        test_mixed_functions();
        test_original_suite();
        
        std::cout << "\n=== All Tests Passed! ===\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Test failed with unknown exception" << std::endl;
        return 1;
    }
}
