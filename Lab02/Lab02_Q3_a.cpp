void copyArray(const vector<int>& source, vector<int>& dest) {
    // step 1: loop initialization; assignment(i=0)
    for (int i = 0; i < source.size(); i++) {
        // step 1: comparison(i<source.size())
        // step 2: array access(dest[i] & source[i])
        // step 1: assignment(dest[i]=source[i])
        dest[i] = source[i];
        // step 1: increment(i++)
    }
    // step 1: loop exit; comparison(i<source.size())
}
// 1 step(initial) + 5*source.size() steps(loop) + 1 step(loop exit)
// Total steps: 2+5n
