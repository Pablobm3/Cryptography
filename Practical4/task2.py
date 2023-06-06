#!/usr/bin/env python

import time
import hashlib

NB_TESTS = 10000000

def measure_hash_function_performance(data, hash_func):
    start_time = time.time()
    hash_func(data)
    end_time = time.time()
    runtime = end_time - start_time
    return runtime

data = b"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed non risus. Suspendisse lectus tortor, dignissim sit amet, adipiscing nec, ultricies sed, dolor. Cras elementum ultrices diam. Maecenas ligula massa, varius a, semper congue, euismod non, mi. Proin porttitor, orci nec nonummy molestie, enim est eleifend mi, non fermentum diam nisl sit amet erat. Duis semper. Duis arcu massa, scelerisque vitae, consequat in, pretium a, enim. Pellentesque congue. Ut in risus volutpat libero pharetra tempor. Cras vestibulum bibendum augue. Praesent egestas leo in pede. Praesent blandit odio eu enim. Pellentesque sed dui ut augue blandit sodales. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Aliquam nibh. Mauris ac mauris sed pede pellentesque fermentum. Maecenas adipiscing ante non diam sodales hendrerit."

average_md5_runtime = 0
average_sha1_runtime = 0
average_sha256_runtime = 0
for i in range(0, NB_TESTS):
	md5_runtime = measure_hash_function_performance(data, hashlib.md5)
	sha1_runtime = measure_hash_function_performance(data, hashlib.sha1)
	sha256_runtime = measure_hash_function_performance(data, hashlib.sha256)
	average_md5_runtime += md5_runtime
	average_sha1_runtime += sha1_runtime
	average_sha256_runtime += sha256_runtime
average_md5_runtime /= NB_TESTS
average_sha1_runtime /= NB_TESTS
average_sha256_runtime /= NB_TESTS

md5_speed_MiB = len(data)/(average_md5_runtime*1000000)
sha1_speed_MiB = len(data)/(average_sha1_runtime*1000000)
sha256_speed_MiB = len(data)/(average_sha256_runtime*1000000)

performance_ratio_md5_sha1 = md5_speed_MiB/sha1_speed_MiB
performance_ratio_sha1_sha256 = sha1_speed_MiB/sha256_speed_MiB

md5_cpu_time = 1000000/md5_speed_MiB #for 1 TiB HDD
sha1_cpu_time = 1000000/sha1_speed_MiB #for 1 TiB HDD
sha256_cpu_time = 1000000/sha256_speed_MiB #for 1 TiB HDD

print("Average runtime MD5:", average_md5_runtime, "seconds, or a speed of:", md5_speed_MiB, "(MiB)")
print("Average runtime SHA-1:", average_sha1_runtime, "seconds, or a speed of:", sha1_speed_MiB, "(MiB)")
print("Average runtime SHA-256:", average_sha256_runtime, "seconds, or a speed of:", sha256_speed_MiB, "(MiB)")
print("\nPerformance ratio (MD5 vs. SHA-1):", performance_ratio_md5_sha1)
print("Performance ratio (SHA-1 vs. SHA-256):", performance_ratio_sha1_sha256)
print("\nCPU time if MD5 used for 1TiB HDD:", md5_cpu_time, "seconds, or", md5_cpu_time/3600, "hours, or", md5_cpu_time/(3600*24), "days")
print("CPU time if SHA-1 used for 1TiB HDD:", sha1_cpu_time, "seconds, or", sha1_cpu_time/3600, "hours, or", sha1_cpu_time/(3600*24), "days")
print("CPU time if SHA-256 used for 1TiB HDD:", sha256_cpu_time, "seconds, or", sha256_cpu_time/3600, "hours, or", sha256_cpu_time/(3600*24), "days")
