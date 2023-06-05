#!/usr/bin/env python

import time
import hashlib

def measure_hash_function_performance(data, hash_func):
    start_time = time.time()
    hash_func(data)
    end_time = time.time()
    runtime = end_time - start_time
    return runtime

data = b"Hello, World!"

average_md5_runtime = 0
average_sha1_runtime = 0
average_sha256_runtime = 0
for i in range(0, 10000000):
	md5_runtime = measure_hash_function_performance(data, hashlib.md5)
	sha1_runtime = measure_hash_function_performance(data, hashlib.sha1)
	sha256_runtime = measure_hash_function_performance(data, hashlib.sha256)
	average_md5_runtime += md5_runtime
	average_sha1_runtime += sha1_runtime
	average_sha256_runtime += sha256_runtime
average_md5_runtime /= 1000
average_sha1_runtime /= 1000
average_sha256_runtime /= 1000
performance_ratio_md5_sha1 = average_md5_runtime/average_sha1_runtime
performance_ratio_sha1_sha256 = sha1_runtime / sha256_runtime

print("Average runtime MD5:", average_md5_runtime)
print("Average runtime SHA-1:", average_sha1_runtime)
print("Average runtime SHA-256:", average_sha256_runtime)
print("Performance ratio (MD5 vs. SHA-1):", performance_ratio_md5_sha1)
print("Performance ratio (SHA-1 vs. SHA-256):", performance_ratio_sha1_sha256)
