__kernel void comp_and_swap(__global int* cl_data, int i, int j) {
    int n_pair = get_local_id(0);
    printf("Id %d\n", n_pair);
    int xor_res = n_pair ^ j;
    if (xor_res > n_pair) {
        if (((n_pair & i) == 0) && (cl_data[n_pair] > cl_data[xor_res]) ||
            ((n_pair & i) != 0) && (cl_data[n_pair] < cl_data[xor_res])) {

            int tmp = cl_data[xor_res];
            cl_data[xor_res] = cl_data[n_pair];
            cl_data[n_pair] = tmp;
        }
    }
}
