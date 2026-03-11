void printPairs(const vector<int>& arr) {
    int stepCount = 0; // variable initialization: stepCount
    stepCount++; // step 1: outer loop initialization(i=0)
    for (int i = 0; i < arr.size(); i++) {
        stepCount++; // step 1: comparison(i<arr.size())
        for (int j = i + 1; j < arr.size(); j++) {
            stepCount++; // step 1: comparison(j<arr.size())
            cout << arr[i] << "," << arr[j] << endl;
            stepCount += 2; // step 2: array access(arr[i] & arr[j])
            stepCount++; // step 1: increment(j++)
        }
        stepCount++; // step 1: loop exit; comparison(j<arr.size())
        stepCount++; // step 1: increment(i++)
    }
    stepCount++; // step 1: loop exit; comparison(i<arr.size())
}
