#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <deque>
#include <mutex>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <thread>
#include <vector>

// Define a structure to represent your data
struct CacheEntry {
    std::time_t key;
    std::string value;
};

// Utility function to convert std::time_t to a formatted string
std::string timeToString(std::time_t time) {
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%y%m%d_%H%M%S");
    return ss.str();
}

// Define a class for the thread-safe memory cache
class MemoryCache {
public:
    MemoryCache(std::size_t maxSize) : maxSize(maxSize) {}

    void insert(const std::string& value) {
        std::lock_guard<std::mutex> lock(mutex);

        // Generate a unique key using the current time
        std::time_t key = std::time(nullptr);

        // Check if the cache is full
        if (cache.size() >= maxSize) {
            // Remove the oldest entry from both the key-value and value-key maps
            const auto& oldestKey = cache.begin()->first;
            cache.erase(oldestKey);
            reverseCache.erase(cache[oldestKey]);
        }

        // Insert the new entry into the key-value and value-key maps
        cache[key] = value;
        reverseCache[value] = key;

        // Flush the memory cache to disk
        appendCsv(CacheEntry{key, value});
    }


    void appendCsv(const CacheEntry& entry) {
        //std::lock_guard<std::mutex> lock(mutex);

        std::ofstream file("cache_data.csv", std::ios::app); // Open file in append mode

        if (!file.is_open()) {
            std::cerr << "Error opening file: cache_data.csv" << std::endl;
            return;
        }

        // Append the new entry to the CSV file
        file << timeToString(entry.key) << "," << entry.value << std::endl;

        file.close();
    }
    void writeToCSV(const std::string& filename) {
        //std::lock_guard<std::mutex> lock(mutex);

        std::ofstream file(filename, std::ios::app); // Open file in append mode

        if (!file.is_open()) {
            std::cerr << "Error opening file: " << filename << std::endl;
            return;
        }

        // Write all entries in the cache to CSV
        for (const auto& entry : cache) {
            file << timeToString(entry.first) << "," << entry.second << std::endl;
        }

        file.close();
    }

    void loadFromCSV(const std::string& filename) {
        std::lock_guard<std::mutex> lock(mutex);

        std::ifstream file(filename);

        if (!file.is_open()) {
            std::cerr << "Error opening file: " << filename << std::endl;
            return;
        }

        // Read each line and populate the cache
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string timeStr, value;

            if (std::getline(iss, timeStr, ',') && std::getline(iss, value)) {
                std::tm tm = {};
                std::istringstream timeStream(timeStr);
                timeStream >> std::get_time(&tm, "%y%m%d_%H%M%S");
                std::time_t key = std::mktime(&tm);

                cache[key] = value;
                reverseCache[value] = key;
            }
        }

        file.close();
    }

    std::string searchByKey(std::time_t key) {
        std::lock_guard<std::mutex> lock(mutex);

        auto it = cache.find(key);
        if (it != cache.end()) {
            return it->second;
        } else {
            return "Key not found";
        }
    }

    std::time_t searchByValue(const std::string& value) {
        std::lock_guard<std::mutex> lock(mutex);

        auto it = reverseCache.find(value);
        if (it != reverseCache.end()) {
            return it->second;
        } else {
            return -1; // Indicates value not found
        }
    }

    std::vector<CacheEntry> searchByTimeRange(std::time_t beginTime, std::time_t endTime) {
        std::lock_guard<std::mutex> lock(mutex);

        std::vector<CacheEntry> result;

        for (const auto& entry : cache) {
            if (entry.first >= beginTime && entry.first <= endTime) {
                result.push_back({entry.first, entry.second});
            }
        }

        return result;
    }

private:
    std::map<std::time_t, std::string> cache;
    std::map<std::string, std::time_t> reverseCache;
    std::size_t maxSize;
    std::mutex mutex;
};

int kata08_memory_cache(int argc, char** argv) {
    // Create an instance of the MemoryCache with a maximum size of 2
    MemoryCache memoryCache(2);

    // Load data from CSV file to memory cache
    memoryCache.loadFromCSV("cache_data.csv");

    // Create threads for concurrent operations
    std::thread thread1([&memoryCache]() {
        std::cout << "insert John " << std::endl;
        memoryCache.insert("John");
    });

    std::thread thread2([&memoryCache]() {
        std::cout << "insert 25 " << std::endl;
        memoryCache.insert("25");
    });

    std::thread thread3([&memoryCache]() {
        // Simulate some delay before writing to CSV
        std::cout << "write csv" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        memoryCache.writeToCSV("cache_data.csv");
    });

    // Join threads to ensure they complete before program termination
    thread1.join();
    thread2.join();
    thread3.join();

    // Search for a value based on a key
    std::time_t currentTime = std::time(nullptr);
    std::cout << "Value for current time: " << memoryCache.searchByKey(currentTime) << std::endl;
    std::cout << "Value for 'City': " << memoryCache.searchByKey(currentTime - 3600) << std::endl;

    // Search for a key based on a value
    std::cout << "Key for value '25': " << memoryCache.searchByValue("25") << std::endl;
    std::cout << "Key for value 'Chicago': " << memoryCache.searchByValue("Chicago") << std::endl;

    // Search for data within a time range
    std::time_t startTime = currentTime - 3600;  // 1 hour ago
    std::time_t endTime = currentTime;
    
    std::vector<CacheEntry> result = memoryCache.searchByTimeRange(startTime, endTime);

    std::cout << "Data within the time range [" << timeToString(startTime)
              << " - " << timeToString(endTime) << "]: " << std::endl;

    for (const auto& entry : result) {
        std::cout << "Key: " << timeToString(entry.key) << ", Value: " << entry.value << std::endl;
    }

    return 0;
}
