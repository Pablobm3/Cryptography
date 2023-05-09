#include <iostream>
#include <chrono>

int main() {
  int num_instructions = 1000000000;
  int a = 0;

  auto start = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < num_instructions; i++) {
    a = a + 1;
  }

  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
  
  std::cout << "Total execution time: " << duration << " microseconds\n";
  std::cout << "Average execution time per instruction: " << duration / static_cast<double>(num_instructions) << " microseconds\n";

  return 0;
}
