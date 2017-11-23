/*
 * FIR 32bit input, 16 taps. Program stops after injecting 128 inputs.
 */

#include "legup/streaming.h"
#include <stdio.h>
#include <pthread.h>

/*func process1(in, out chan int) {
  for i := 0; i < 10; i++ {
    item := <- in;
    out <- item * 2 + 1;
  }
}*/

void *process1(FIFO **arg) {
	int i;
	int item;

	for (i = 0; i < 10; i++) {
		item = fifo_read((FIFO*)arg[0]);
		fifo_write((FIFO*)arg[1], item * 2 + 1);
	}
    pthread_exit(NULL);

}

/*func process2(in, out chan int) {
  for i := 0; i < 10; i++ {
    item := <- in;
    out <- (item - 1) / 2;
  }
}*/

void *process2(FIFO **arg) {
	int i, item;
	for (i = 0; i < 10; i++) {
		item = fifo_read((FIFO*)arg[0]);
		fifo_write((FIFO*)arg[1], (item - 1) / 2);

	}
    pthread_exit(NULL);
}

/*
 * func main() {
    c1 := make(chan int, 10);
    c2 := make(chan int, 10);
    c3 := make(chan int, 10);
    for i := 0; i < 10; i++ {
      c1 <- i * i;
    }

    go process1(c1, c2);
    go process2(c2, c3);
    for i := 0; i < 10; i++ {
      x := <-c3;
      fmt.Println(x)
    }
}
 *
 */
int main() {

	FIFO *c1 = fifo_malloc(/*data_width =*/ 32, /*fifo_depth =*/ 10);
	FIFO *c2 = fifo_malloc(/*data_width =*/ 32, /*fifo_depth =*/ 10);
	FIFO *c3 = fifo_malloc(/*data_width =*/ 32, /*fifo_depth =*/ 10);

	int i;
	for(i = 0; i < 10; i++) {
		fifo_write(c1, i * i);
	}

    pthread_t process1_t, process2_t;
    FIFO * arg1[2] = {c1, c2};
    FIFO * arg2[2] = {c2, c3};
    pthread_create(&process1_t, NULL, process1, arg1);
    pthread_create(&process2_t, NULL, process2, arg2);

	for(i = 0; i < 10; i++) {
        int x = fifo_read(c3);
        printf("%d\n",x);
    }

	return 0;
}
