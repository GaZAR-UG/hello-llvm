int add(int a, int b) { return a + b; }

int inc(int a) { return ++a; }

int (*fptr)(int) = &inc;

int main(int argc, char **argv) {
  int i = 42;
  int j = 13;
  int k = add(i, j);
  k = inc(k);
  for (int i = 0; i < 100; ++i) {
    ++k;
  }
  k = fptr(k);
  return k;
}