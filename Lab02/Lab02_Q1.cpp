int findMax(const vector<int>& arr) {
    // 1 step: array access(arr[0])
    // 1 step: assignment(max = arr[0])
    int max = arr[0];
    // 1 step: loop initialization; assignment(i=1)
    for (int i = 1; i < arr.size(); i++) {
        // 1 step: comparison(i<arr.size())
        // 1 step: array access(arr[i])
        // 1 step: comparison(arr[i]>max)
        if (arr[i] > max) {
            // 1 step: array access(arr[i])
            // 1 step: assignment(max=arr[i])
            max = arr[i];
        }
        // 1 step: increment(i++)
    }
    // 1 step: loop exit; comparison(i<arr.size())
    // 1 step: return(max)
    return max;
}
// 3 steps(initial) + 6*arr.size() steps(loop) + 2 step(loop exit + return)
// Total steps: 5+6n
