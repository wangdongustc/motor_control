#include "mbed.h"
#include "quick_sort.h"

#define LEN  14

Serial device(USBTX, USBRX); // tx, rx
DigitalOut servo[LEN] = {p11, p12, p15, p16, p17, p18, p19, p20, p26, p25, p24, p23, p22, p21};
//DigitalOut servo[LEN] = {p17, p18, p19, p29, p20, p30, p21, p23, p25, p27, p22, p24, p26, p28};
//DigitalOut servo[LEN] = {LED1, LED2, LED3, LED4, p20, p30, p21, p23, p25, p27, p22, p24, p26, p28};
Ticker timer;
DigitalOut led1(LED1);
DigitalOut led2(LED2);

long count = 0;
int degree[LEN];
int index[LEN];
bool running = false;

int degree_num = 0;
bool finished = false;
void atTime() {
    count++;
    if (count == 2000) { count = 0; running = false; timer.detach(); /*device.printf("detached");*/}
    if (count == degree[degree_num] && (!finished)) {
        do {
            servo[index[degree_num]] = 0;
            //device.printf("%6d", index[degree_num]);
            degree_num++;
            if (degree_num == LEN) {finished = true;}
        } while(degree[degree_num] == count);
    }
}

// in this function, the degrees are corresponding to the pwm high time.
timestamp_t t = 10; // the one degree time.
void servoMove() {
    count = 0;
    degree_num = 0;
    for (int i = 0; i < LEN; i++) {
        index[i] = i;
        degree[i] += 60;
        servo[i] = 0;        
    }
    quick_sort(degree, index, LEN);
    for (int i = 0; i < LEN; i++) {
        servo[i] = 1;        
    }
    running = true;
    finished = false;
    timer.attach_us(&atTime, t);
    while(running) {wait(0.0001);};
    
}

#define INIT_DEGREE_SET {75,75,75,75,75,75,75,75,75,75,75,75,75,75}
static int initial_degree[LEN] = INIT_DEGREE_SET;
static int last_degree[LEN] = INIT_DEGREE_SET;
void servoRun(int* end_degree, int period_number) {
    float delta_degree[LEN];
    float delta_degree_each_time[LEN];
    float degree_each_time[LEN];
    int i, j;
    for (i = 0; i < LEN; i++) {
        delta_degree[i] = end_degree[i] - last_degree[i];
        delta_degree_each_time[i] = delta_degree[i] / period_number;
        degree_each_time[i] = last_degree[i];
    }
    for (j = 0; j < period_number; j++) {
        for (i = 0; i < LEN; i++) {
            degree_each_time[i] += delta_degree_each_time[i];
            degree[i] = degree_each_time[i];
        }
        servoMove();
    }
    for (i = 0; i < LEN; i++) {
        last_degree[i] = end_degree[i];
    }
}

void initialize() {
    for (int j = 0; j < 50; j++) {
        for (int i = 0; i < LEN; i++) {
            degree[i] = initial_degree[i];
        }
        servoMove();
    }
}

int main() {
    initialize();
    int degree_ref[LEN];
    int degree_set[LEN];
    //timer.attach_us(&atTime, t);
    for(int i = 0; i < LEN; i++) {
        degree_ref[i] = rand() % 20 + 55;
        device.printf("%6d", degree_ref[i]);
    }
    
    while(1) {
        for(int i = 0; i < LEN; i++) {
            degree_set[i] = degree_ref[i];
        }
        servoRun(degree_set, 20);
        device.printf("OK-1");
        
        for(int i = 0; i < LEN; i++) {
            degree_set[i] = degree_ref[i] + 60;
        }
        servoRun(degree_set, 40);
        device.printf("OK-2");
    }
}
