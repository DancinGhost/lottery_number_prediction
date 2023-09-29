#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>

int main() {
    std::ifstream inputFile("list.txt");
    std::vector<int> lotteryNumbers;
    int number;

    // Read the lottery numbers from the file
    while (inputFile >> number) {
        lotteryNumbers.push_back(number);
    }

    // Close the file
    inputFile.close();

    // Create a map to store the frequency of each number
    std::map<int, int> frequencyMap;
    for (int num : lotteryNumbers) {
        frequencyMap[num]++;
    }

    // Find the number with the highest frequency (hot number)
    int hotNumber = std::max_element(frequencyMap.begin(), frequencyMap.end(), 
                                      { return a.second < b.second; })->first;

    // Find the number with the lowest frequency (cold number)
    int coldNumber = std::min_element(frequencyMap.begin(), frequencyMap.end(), 
                                       { return a.second < b.second; })->first;

    // Find the number that has not appeared in the longest time (overdue number)
    int overdueNumber = lotteryNumbers.back();

    // Print the hot, cold, and overdue numbers
    std::cout << "Hot Number: " << hotNumber << "\n";
    std::cout << "Cold Number: " << coldNumber << "\n";
    std::cout << "Overdue Number: " << overdueNumber << "\n";

    return 0;
}
