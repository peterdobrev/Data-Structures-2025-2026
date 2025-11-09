#include <iostream>
#include <stack>
#include <vector>

std::vector<int> canSeePersonsCount(std::vector<int>& heights) {
    int n = heights.size();
    std::vector<int> res(n);

    // TODO
    
    return res;
}

int main() {
    std::vector<int> input1 = {10,6,8,5,11,9};
    // expected => {3,1,2,1,1,0}
    std::vector<int> res1 = canSeePersonsCount(input1); 
    for (size_t i = 0; i < res1.size(); ++i) {
        std::cout << res1[i] << " ";
    }
    std::cout << std::endl;

    // std::vector<int> input2 = {5,1,2,3,10};
    // expected => {4,1,1,1,0}

}