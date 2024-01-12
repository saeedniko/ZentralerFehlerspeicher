#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define MAX_ERRORS 100
#define MAX_RETRY_COUNT 3

typedef enum {
    INFORMATION,
    LOW,
    MID,
    HIGH,
    VERY_CRITICAL
} Severity;

typedef struct {
    uint16_t moduleID;
    uint16_t errorID;
    Severity severity;
    uint32_t timeStamp;
    uint32_t timeDelta;
    uint8_t occurrenceCount;
    uint8_t parameters[8];
} ErrorEntry;

static ErrorEntry errorStorage[MAX_ERRORS];
static uint8_t errorCount = 0;
static uint32_t systemStartTime = 0;
static uint32_t engineStartCount = 0;
static uint32_t TempTickcount = 0;
static bool Start,Error;

static uint32_t GetTickCount() {
    TempTickcount = TempTickcount + 1000;

    // Implement this function to get the current system tick count in milliseconds
    // This function should return the elapsed time since system start
    return 0;
}

void error_init(){
    if (errorCount == 0) {
        systemStartTime = GetTickCount();
    }
};
int error_set(){

};
int error_get(){

};
int error_remove(){

};
int error_countEnginStart(){

};
int error_Hanlder(){

};
int error_deinit(){
    
};

int main() {
error_init();


    


}
