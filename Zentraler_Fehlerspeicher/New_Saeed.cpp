#include <Windows.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_ERRORS 10
#define MAX_RETRY_COUNT_INFO 3
#define MAX_RETRY_COUNT_LOW 5
#define MAX_RETRY_COUNT_MID 8
#define MAX_RETRY_COUNT_HIGH 10
#define MAX_RETRY_COUNT_VERY_CRITICAL 15

typedef enum
{
    INFORMATION,
    LOW,
    MID,
    HIGH,
    VERY_CRITICAL
} Severity;

typedef unsigned char t_BYTE;
typedef unsigned short t_WORD;
typedef unsigned long t_DWORD;

typedef struct
{
    t_WORD moduleID;
    t_WORD errorID;
    Severity severity;
    t_DWORD timeStamp;
    t_DWORD timeDelta;
    t_BYTE occurrenceCount;
    t_BYTE parameters[8];
} ErrorEntry;

static ErrorEntry errorStorage[MAX_ERRORS];
static t_BYTE errorCount = 0;
static t_DWORD systemStartTime = 0;
static t_DWORD engineStartCount = 0;
static t_DWORD TempTickCount = 0;
static t_DWORD MaxStamp = 0;
static t_DWORD MaxOcca = 0;
static t_DWORD errorNumber = 0;

void print(int number)
{
    char str[20]; // Make sure the buffer is large enough to hold the resulting string

    // Using sprintf
    sprintf(str, "%d", number);
    // printf("%s\n", str);
    printf("%s", str);
    printf(".");
}

t_DWORD GetTickCount()
{
    TempTickCount = clock();

    // Implement this function to get the current system tick count in milliseconds
    // This function should return the elapsed time since system start
    return TempTickCount;
}

void ERR_Init()
{
    // Initialize the error module
    if (errorCount == 0)
    {
        systemStartTime = GetTickCount();
        printf("Error Init\n");
        // Additional initialization code if needed
    }
}

size_t ERR_Get(t_WORD moduleID, t_WORD errorID)
{
    MaxStamp = 0;
    MaxOcca = 0;

    size_t i;
    for (i = 0; i < MAX_ERRORS; i++)
    {
        // Check if a specific error is set for a module
        if (errorStorage[i].moduleID == moduleID && errorStorage[i].errorID == errorID)
        {
            if (MaxStamp < errorStorage[i].timeStamp)
            {
                MaxStamp = errorStorage[i].timeStamp;
                MaxOcca = errorStorage[i].occurrenceCount;
                errorNumber = i;
            }
        }
    }
    return i;
}

void ERR_Set(t_WORD moduleID, t_WORD errorID, Severity severity, t_BYTE parameters[8])
{
    // Set a new error entry
    // Find the next available position in the error storage
    if (errorCount < MAX_ERRORS)
    {
        ErrorEntry *newError = &errorStorage[errorCount];
        newError->moduleID = moduleID;
        newError->errorID = errorID;
        newError->severity = severity;
        newError->timeStamp = GetTickCount();
        newError->occurrenceCount = MaxOcca + 1;
        newError->timeDelta = GetTickCount() - MaxStamp;
        newError->parameters[8] = parameters[8];
        errorCount++;
        printf("error added with success\n");
    }
    else
    {
        ERR_Handler();
        printf("There is no memory free and calling ERR_Hanlder\n");
    }
}

bool ERR_Update(t_WORD moduleID, t_WORD errorID, Severity severity, t_BYTE parameters[8])
{
    errorStorage[errorNumber].timeDelta = GetTickCount() - errorStorage[errorNumber].timeStamp;
    errorStorage[errorNumber].timeStamp = GetTickCount();
    errorStorage[errorNumber].occurrenceCount = errorStorage[errorNumber].occurrenceCount + 1;
    errorStorage[errorNumber].parameters[8] = parameters[8];

    return true;
}

void ERR_Handler()
{
    // Perform error handling tasks, such as clearing non-critical errors after a certain number of engine starts
    printf("error handel started success\n");
    
    for (t_BYTE i = 0; i < errorCount; i++)
    {
        if (errorStorage[i].severity <= INFORMATION)
        {
            // Clear non-critical errors after a certain number of engine starts
            if (engineStartCount >= MAX_RETRY_COUNT_INFO)
            {
                // Swap the current error with the last one
                printf("Error clear\n");
                print(errorCount);
                errorStorage[i] = errorStorage[errorCount - 1];
                errorCount--;
                i--;
            }
            if (errorStorage[i].severity <= LOW)
            {
                // Clear non-critical errors after a certain number of engine starts
                if (engineStartCount >= MAX_RETRY_COUNT_LOW)
                {
                    // Swap the current error with the last one
                    printf("Error clear\n");
                    print(errorCount);
                    errorStorage[i] = errorStorage[errorCount - 1];
                    errorCount--;
                    i--;
                }
            }
            if (errorStorage[i].severity <= MID)
            {
                // Clear non-critical errors after a certain number of engine starts
                if (engineStartCount >= MAX_RETRY_COUNT_MID)
                {
                    // Swap the current error with the last one
                    printf("Error clear\n");
                    print(errorCount);
                    errorStorage[i] = errorStorage[errorCount - 1];
                    errorCount--;
                    i--;
                }
            }
            if (errorStorage[i].severity <= HIGH)
            {
                // Clear non-critical errors after a certain number of engine starts
                if (engineStartCount >= MAX_RETRY_COUNT_HIGH)
                {
                    // Swap the current error with the last one
                    printf("Error clear\n");
                    print(errorCount);
                    errorStorage[i] = errorStorage[errorCount - 1];
                    errorCount--;
                    i--;
                }
            }
            if (errorStorage[i].severity <= VERY_CRITICAL)
            {
                // Clear non-critical errors after a certain number of engine starts
                if (engineStartCount >= MAX_RETRY_COUNT_VERY_CRITICAL)
                {
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

void ERR_Remove(Severity severity)
{
    printf("Error remove\n");
    // Remove errors based on severity
    ErrorEntry *lastError = &errorStorage[errorCount - 1];
    for (t_BYTE i = 0; i < errorCount; i++)
    {
        if (errorStorage[i].severity <= severity)
        {
            // Swap the current error with the last one
            errorStorage[i] = *lastError;
            errorCount--;
            i--;
        }
    }
}

void ERR_CountEngineStart()
{
    printf("Engine Start\n");
    // Increment the engine start counter
    engineStartCount++;
}

void ERR_DeInit()
{
    printf("deint\n");
    // Reset the entire module and all error messages
    errorCount = 0;
    systemStartTime = 0;
    engineStartCount = 0;
}

void error_Show()
{
    for (size_t i = 0; i < MAX_ERRORS; i++)
    {
        print(errorStorage[i].errorID);
        print(errorStorage[i].moduleID);
        print(errorStorage[i].timeStamp);
        print(errorStorage[i].severity);
        printf("\n");
    }
}

int main()
{
    // Flag to ensure ERR_DeInit is called only once
    bool errDeInitCalled = false;

    // Run continuously with a 100 ms delay
    while (1)
    {
        // Introduce a timer with a 100 ms delay
        Sleep(100);

        // Simulate checking if the engine has started
        int engineStarted = 0; // Replace with your actual condition

        if (engineStarted)
        {
            // Run ERR_CountEngineStart one time after the engine has started
            ERR_CountEngineStart();

            // Run ERR_Init after counting the engine start
            ERR_Init();

            // Run ERR_Handler after initializing
            ERR_Handler();

            // Break out of the loop once the engine has started
            break;
        }
        else if (!errDeInitCalled)
        {
            // If the engine hasn't started, call ERR_DeInit only once
            ERR_DeInit();
            printf("ERR_DeInit called before engine start.\n");
            errDeInitCalled = true; // Set the flag to ensure ERR_DeInit is called only once
        }
    }
    // Another while loop after started engine
    while (1)
    {
        // Introduce a timer with a 100 ms delay
        Sleep(100);

        // Simulate checking for errors
        int errorOccurred = 0; // Replace with your actual condition

        if (errorOccurred)
        {
            // Call ERR_Get function when an error occurs
            ERR_Get(/* provide moduleID and errorID */);
            printf("ERR_Get called due to an error.\n");

            if (MaxStamp > 0)
            {
                if (errorStorage[errorNumber].occurrenceCount > 5)
                {
                    ERR_Update(moduleID, errorID, severity, &parameters[8]);
                }
                else
                {
                    ERR_Set(moduleID, errorID, severity, &parameters[8]);
                }
            }
            else
            {
                ERR_Set(moduleID, errorID, severity, &parameters[8]);
            }
            

            // You can add additional logic here to handle the error
        }
    }
}