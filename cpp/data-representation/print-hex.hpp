// Print a hexadecimal view of how a number is stored in memory.
// There is an adjustment for the little endian (as it is the most common
// endianness one may encounter) - i.e., it is assumed that the most 
// significant byte is stored last. Besides that, there are no other
// modifications on the number: for example, the negative integer -1 will be
// printed (assuming two's complement) as
// ff ... ff

void print_bytes(unsigned char* start, int len) {
  for (int i = len - 1; i >= 0; --i) {
    printf("%.2x ", start[i]);
  }
  printf("\n");
}

template <typename T>
void print_hex(T number) {
  unsigned char* ptr = (unsigned char*) &number;
  print_bytes(ptr, sizeof(T));
}
