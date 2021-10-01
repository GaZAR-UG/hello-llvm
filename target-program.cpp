int add(int A, int B) { return A + B; }

int inc(int A) { return ++A; }

int (*Fptr)(int) = &inc; // NOLINT

int main() {
  int I = 42;
  int J = 13;
  int K = add(I, J);
  K = inc(K);
  for (int Idx = 0; Idx < 100; ++Idx) {
    ++K;
  }
  K = Fptr(K);
  return K;
}
