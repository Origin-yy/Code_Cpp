#include <stdio.h>

void bingbao(int n);

int main() {
  int n;
  int arr[100];
  int cnt = 0;
  scanf("%d", &n);
  bingbao(n);
  return 0;
}
void bingbao(int n) {
  int arr[100];
  int cnt = 0;
  if (n == 1) {
    printf(" 1 ");
    return;
  }
  while (n > 1) {
    if (n % 2 == 1) {
      n = n * 3 + 1;
      arr[cnt] = n;
    } else {
      n = n / 2;
      arr[cnt] = n;
    }
    cnt++;
  }
  cnt --;
  for (; cnt >= 0; cnt--) {
    printf(" %d ", arr[cnt]);
  }
}