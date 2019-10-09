#include "compatibility.h"

void __msleep(int milisec) {
	for(int i = 0; i < milisec*10000; i++) { }
}

void __usleep(int milisec) {
	for(int i = 0; i < milisec*10000; i++) { }
}

void __start_timer() {

}

long __millis() {
	return 0;
}
