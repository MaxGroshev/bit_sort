__kernel void comp_and_swap(__global int* cl_data,
                            int comp_type,int a_ind, int b_ind) {
    printf("Hello %d %d\n", cl_data[a_ind], cl_data[b_ind]);
    printf("%d %d \n", get_group_id(0), get_local_id(0));

    int n_pair = get_local_id(0);
    int fst_ind = n_pair * 2;
    if (!(n_pair % 1) == 0) {
        if (cl_data[fst_ind] > cl_data[fst_ind + 1]) {
            int tmp = cl_data[fst_ind + 1];
            cl_data[fst_ind + 1] = cl_data[fst_ind];
            cl_data[fst_ind] = tmp;
        }
    }
    else {
        if (cl_data[fst_ind] < cl_data[fst_ind + 1]) {
            // atomic_exchage(cl_data[fst_ind + 1], cl_data[fst_ind])
            int tmp = cl_data[fst_ind + 1];
            cl_data[fst_ind + 1] = cl_data[fst_ind];
            cl_data[fst_ind] = tmp;
        }
    }
    // barrier(CLK_LOCAL_MEM_FENCE);
}
