int countElement(const vector<int>& arr, int target) {
    // step 1: assignment(count=0)
    int count = 0;
    // step 1: loop initialization; assignment(i=0)
    for (int i = 0; i < arr.size(); i++) {
        // step 1: comparison(i<arr.size())
        // step 1: array access(arr[i])
        // step 1: comparison(arr[i]==target)
        if (arr[i] == target) {
            // step 1: arithmetic(count++)
            count++;
        }
        // step 1: increment(i++)
    }
    // step 1: loop exit; comparison(i<arr.size())
    // step 1: return(count)
    return count;
}
// 2 steps(initial) + 5*arr.size() steps(loop) + 2 steps(loop exit + return)
// Total steps: 4+5n
