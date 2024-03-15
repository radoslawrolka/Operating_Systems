int collatz_conjecture(int n) {
    if (n%2 == 0) {
        return n/2;
    }
    return 3*n+1;
}

int test_collatz_convergence(int input, int max_iter) {
    for (int i = 1; i <= max_iter; i++) {
        input = collatz_conjecture(input);
        if (input == 1) {
            return i;
        }
    }
    return -1;
}