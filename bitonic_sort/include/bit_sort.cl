__kernel void comp_and_swap(__global int* cl_data, int i, int j) {
    int n_pair = get_group_id(0) * get_local_size(0) + get_local_id(0);
    int xor_res = n_pair ^ j;
    int and_res = n_pair & i;
    if (xor_res > n_pair) {
        if ((and_res == 0) && (cl_data[n_pair] > cl_data[xor_res]) ||
            (and_res != 0) && (cl_data[n_pair] < cl_data[xor_res])) {

            int tmp = cl_data[xor_res];
            cl_data[xor_res] = cl_data[n_pair];
            cl_data[n_pair] = tmp;
        }
    }
}
