mkdir -p target
gcc -o target/aes aes.c
target/aes
gcc -o target/aes_omp aes_omp.c -fopenmp
target/aes_omp
nvcc -o target/aes_cuda aes_cuda.cu
target/aes_cuda
