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

static uint32_t GetTickCount() {
    TempTickcount = TempTickcount + 1000;

    // Implement this function to get the current system tick count in milliseconds
    // This function should return the elapsed time since system start
    return 0;
}

void ERR_Init() {
    // Initialize the error module
    if (errorCount == 0) {
        systemStartTime = GetTickCount();
        // Additional initialization code if needed
    }
}



bool ERR_Get(uint16_t moduleID, uint16_t errorID, Severity* severity) {
    // Check if a specific error is set for a module
    for (uint8_t i = 0; i < errorCount; i++) {
        if (errorStorage[i].moduleID == moduleID && errorStorage[i].errorID == errorID) {
            *severity = errorStorage[i].severity;
            return true;
        }
    }
    return false;
}

void ERR_Remove(Severity severity) {
    // Remove errors based on severity
    for (uint8_t i = 0; i < errorCount; i++) {
        if (errorStorage[i].severity <= severity) {
            // Remove the error entry
            // (This can involve shifting array elements to fill the gap)
            // Decrement the error count
            errorCount--;
            i--;
        }
    }
}
void ERR_Set(uint16_t moduleID, uint16_t errorID, Severity severity, uint8_t parameters[8]) {
    // Set a new error entry
    // Find the next available position in the error storage
    if (errorCount < MAX_ERRORS) {
        errorStorage[errorCount].moduleID = moduleID;
        errorStorage[errorCount].errorID = errorID;
        errorStorage[errorCount].severity = severity;
        errorStorage[errorCount].timeStamp = GetTickCount();
        // Set other parameters...
        errorCount++;
    }
    // inormation, low, Mid, High, Very Critical

    ERR_Remove(INFORMATION);
}

void ERR_CountEngineStart() {
    // Increment the engine start counter
    engineStartCount++;
}

void ERR_Handler() {
    // Perform error handling tasks, such as clearing non-critical errors after a certain number of engine starts
    for (uint8_t i = 0; i < errorCount; i++) {
        if (errorStorage[i].severity <= MID) {
            // Clear non-critical errors after a certain number of engine starts
            if (engineStartCount >= MAX_RETRY_COUNT) {
                // Remove the error entry
                // (This can involve shifting array elements to fill the gap)
                // Decrement the error count
                errorCount--;
                i--;
            }
        }
    }
}

void ERR_DeInit() {
    // Reset the entire module and all error messages
    errorCount = 0;
    systemStartTime = 0;
    engineStartCount = 0;
    // Additional deinitialization code if needed
}

int main() {
    bool Start,error;
    printf("1. Program Start\n");
    // Example usage
    ERR_Init();
  

    uint8_t params[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    

    Severity severity;
    if (ERR_Get(1, 100, &severity)) {
        
        // Error is set for module 1 with error ID 100
        // Handle the error based on severity
    }

    // Perform other tasks...
if (Start=true){
    if (error=true)
    {
        ERR_Set(1, 100, LOW, params);
       }
    }
    
    ERR_CountEngineStart();
    ERR_Handler();

    // Perform other tasks...

    ERR_DeInit();

    return 0;
}
