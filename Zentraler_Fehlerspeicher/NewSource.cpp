#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
 
#define MAX_ERRORS 100
#define MAX_RETRY_COUNT_INFO 3
#define MAX_RETRY_COUNT_LOW 5
#define MAX_RETRY_COUNT_MID 8
#define MAX_RETRY_COUNT_HIGH 10
#define MAX_RETRY_COUNT_VERY_CRITICAL 15
 
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
static uint32_t TempTickCount = 0;

 void print(int number){
     char str[20]; // Make sure the buffer is large enough to hold the resulting string

    // Using sprintf
    sprintf(str, "%d", number);
   // printf("%s\n", str);
    printf("%s", str);
    printf(".");
 }

static uint32_t GetTickCount() {
    TempTickCount = TempTickCount + 1000;
    
    // Implement this function to get the current system tick count in milliseconds
    // This function should return the elapsed time since system start
    return TempTickCount;
}
 
void ERR_Init() {
    // Initialize the error module
    if (errorCount == 0) {
        systemStartTime = GetTickCount();
        printf("Error Init\n");
        // Additional initialization code if needed
    }
}
 bool update(uint16_t moduleID, uint16_t errorID, Severity severity, uint8_t parameters[8]){
    for (size_t i = 0; i < MAX_ERRORS; i++)
    {
        if (errorStorage[i].moduleID==moduleID  &&  errorStorage[i].errorID==errorID && errorStorage[i].severity==severity)
        {
            errorStorage[i].moduleID = moduleID;
            errorStorage[i].errorID = errorID;
            errorStorage[i].severity = severity;
            errorStorage[i].timeStamp = GetTickCount();
            // Set other parameters...
            return true;
        }
        
    }
    return false;
    
 }
void ERR_Set(uint16_t moduleID, uint16_t errorID, Severity severity, uint8_t parameters[8]) {
    // Set a new error entry
    // Find the next available position in the error storage
    //--->majid update here
    if (errorCount < MAX_ERRORS && !update(moduleID,errorID,severity,parameters) ) {
        ErrorEntry *newError = &errorStorage[errorCount];
        newError->moduleID = moduleID;
        newError->errorID = errorID;
        newError->severity = severity;
        newError->timeStamp = GetTickCount();
        // Set other parameters...
        errorCount++;
         printf("error added\n");
    }
    else{
        printf("error Updated\n");

    }
}
 
bool ERR_Get(uint16_t moduleID, uint16_t errorID, Severity* severity) {
    // Check if a specific error is set for a module
    
    for (uint8_t i = 0; i < errorCount; i++) {
        if (errorStorage[i].moduleID == moduleID && errorStorage[i].errorID == errorID) {
            *severity = errorStorage[i].severity;
            printf("Error get\n");
            return true;
        }
    }
    return false;
}
 
void ERR_Remove(Severity severity) {
    printf("Error remove\n");
    // Remove errors based on severity
    ErrorEntry *lastError = &errorStorage[errorCount - 1];
    for (uint8_t i = 0; i < errorCount; i++) {
        if (errorStorage[i].severity <= severity) {
            // Swap the current error with the last one
            errorStorage[i] = *lastError;
            errorCount--;
            i--;
        }
    }
}
 
void ERR_CountEngineStart() {
    printf("Engine Start\n");
    // Increment the engine start counter
    engineStartCount++;
}
 
void ERR_Handler() {
    printf("error handel\n");
    // Perform error handling tasks, such as clearing non-critical errors after a certain number of engine starts
    ///------->saeed
    for (uint8_t i = 0; i < errorCount; i++) {
        if (errorStorage[i].severity <= INFORMATION) {
            // Clear non-critical errors after a certain number of engine starts
            if (engineStartCount >= MAX_RETRY_COUNT_INFO) {
                // Swap the current error with the last one
                printf("Error clear\n");
                print(errorCount);
                errorStorage[i] = errorStorage[errorCount - 1];
                errorCount--;
                i--;
            }
            if (errorStorage[i].severity <= LOW) {
                // Clear non-critical errors after a certain number of engine starts
                if (engineStartCount >= MAX_RETRY_COUNT_LOW) {
                    // Swap the current error with the last one
                    printf("Error clear\n");
                    print(errorCount);
                    errorStorage[i] = errorStorage[errorCount - 1];
                    errorCount--;
                    i--;
                }
            }
            if (errorStorage[i].severity <= MID) {
                // Clear non-critical errors after a certain number of engine starts
                if (engineStartCount >= MAX_RETRY_COUNT_MID) {
                    // Swap the current error with the last one
                    printf("Error clear\n");
                    print(errorCount);
                    errorStorage[i] = errorStorage[errorCount - 1];
                    errorCount--;
                    i--;
                }
            }
            if (errorStorage[i].severity <= HIGH) {
                // Clear non-critical errors after a certain number of engine starts
                if (engineStartCount >= MAX_RETRY_COUNT_HIGH) {
                    // Swap the current error with the last one
                    printf("Error clear\n");
                    print(errorCount);
                    errorStorage[i] = errorStorage[errorCount - 1];
                    errorCount--;
                    i--;
                }
            }
            if (errorStorage[i].severity <= VERY_CRITICAL) {
                // Clear non-critical errors after a certain number of engine starts
                if (engineStartCount >= MAX_RETRY_COUNT_VERY_CRITICAL) {
                    // Swap the current error with the last one
                    printf("Error clear\n");
                    print(errorCount);
                    errorStorage[i] = errorStorage[errorCount - 1];
                    errorCount--;
                    i--;
                }
            }
        }
    }
}
 
void ERR_DeInit() {
    printf("deint\n");
    // Reset the entire module and all error messages
    errorCount = 0;
    systemStartTime = 0;
    engineStartCount = 0;
    // Additional deinitialization code if needed
}
void error_Show(){
    for (size_t i = 0; i < MAX_ERRORS; i++)
    {
       print(errorStorage[i].errorID);
       print(errorStorage[i].moduleID);
       print(errorStorage[i].timeStamp);
       print(errorStorage[i].severity);
       printf("\n");
    }
    
}
 
int main() {
    // Example usage
    ERR_Init();
    for (size_t i = 0; i < 105; i++)
    {
        print(i);
  
    
 
    uint8_t params[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    ERR_Set(1, 100, HIGH, params);
    
 
    Severity severity;
    if (ERR_Get(1, 100, &severity)) {
        // Error is set for module 1 with error ID 100
        // Handle the error based on severity
    }
 
    // Perform other tasks...
 
    ERR_CountEngineStart();
    ERR_Handler();
    }
    // Perform other tasks...
    error_Show();
    ERR_DeInit();
 
    return 0;
}