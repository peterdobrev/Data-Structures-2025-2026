#include <iostream>
#include <stack>
#include <vector>

std::vector<int> canSeePersonsCount(std::vector<int>& heights) {
    int n = heights.size();
    std::vector<int> res(n);
        
    std::stack<int> tallest_pos;
    res[n-1] = 0;
    tallest_pos.push(n-1);

    for (int i = n - 2; i >= 0; i--) {
        if (heights[i] <= heights[tallest_pos.top()]) {
            res[i] = 1;
            tallest_pos.push(i);
            continue;
        }
        
        int cnt = tallest_pos.size();
        while(!tallest_pos.empty() && heights[i] > heights[tallest_pos.top()]) {
            tallest_pos.pop();
        }
        
        res[i] = cnt - tallest_pos.size() + !tallest_pos.empty(); 
        tallest_pos.push(i);
    }

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
}