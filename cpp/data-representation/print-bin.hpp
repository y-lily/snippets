// Print a binary view of how a number is stored in memory.
// There is an adjustment for the little endian (as it is the most common
// endianness one may encounter) - i.e., it is assumed that the most 
// significant byte is stored last. Besides that, there are no other
// modifications on the number: for example, the negative integer -1 will be
// printed (assuming two's complement) as
// 11111111 ... 11111111

void print_bits(unsigned char* start, int len) {
  for (int i = len - 1; i >= 0; --i) {
    printf("%.8b ", start[i]);
  }
  printf("\n");
}

template <typename T>
void print_bin(T number) {
  unsigned char* ptr = (unsigned char*) &number;
  print_bits(ptr, sizeof(T));
}
