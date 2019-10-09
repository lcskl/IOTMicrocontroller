#include "compatibility.h"

void __msleep(int milisec) {

}

void __usleep(int milisec) {

}

void __start_timer() {

}

long __millis() {
	return 0;
}
