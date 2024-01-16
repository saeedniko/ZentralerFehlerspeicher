#include <Windows.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_ERRORS 10
#define MAX_ERROR_ALLOWED 3

// Number of engine start to remove not repeated error
#define MAX_RETRY_COUNT_INFO 3
#define MAX_RETRY_COUNT_LOW 5
#define MAX_RETRY_COUNT_MID 8
#define MAX_RETRY_COUNT_HIGH 10
#define MAX_RETRY_COUNT_VERY_CRITICAL 15

// Number of repeated errors for change to a higher severity
#define MAX_REPEAT_ERROR_MID 100
#define MAX_REPEAT_ERROR_HIGH 80
#define MAX_REPEAT_ERROR_VERY_CRITICAL 50

typedef enum
{
    INFORMATION,
    LOW,
    MID,
    HIGH,
    VERY_CRITICAL,
    NeverComeBack
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
    t_BYTE parameters[7];
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
    char str[20]; // Adjust the size as needed

    // Using snprintf to prevent buffer overflow
    int result = snprintf(str, sizeof(str), "%d", number);

    if (result >= 0 && result < sizeof(str))
    {
        printf("%s.", str);
    }
    else
    {
        // Handle the error
        printf("Error in snprintf\n");
    }
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

size_t ERR_Get(t_WORD moduleID, t_WORD errorID, Severity severity, t_BYTE parameters[7])
{
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

                if (MaxOcca < MAX_ERROR_ALLOWED)
                {
                    // Find the next available position in the error storage
                    if (errorCount < MAX_ERRORS)
                    {
                        ERR_Set(moduleID, errorID, severity, &parameters[7]);
                    }
                    else
                    {
                        ERR_Handler();
                    }
                }
                else
                {
                    ERR_Update(moduleID, errorID, severity, &parameters[7]);
                }
            }
        }
        else
        {
            ERR_Set(moduleID, errorID, severity, &parameters[7]);
        }
    }
}

void ERR_Set(t_WORD moduleID, t_WORD errorID, Severity severity, t_BYTE parameters[7])
{
    // Set a new error entry
    ErrorEntry *newError = &errorStorage[errorCount];
    newError->moduleID = moduleID;
    newError->errorID = errorID;
    newError->severity = severity;
    newError->timeStamp = GetTickCount();
    newError->occurrenceCount = MaxOcca + 1;
    newError->timeDelta = GetTickCount() - MaxStamp;
    newError->parameters[7] = parameters[7];
    errorCount++;
    printf("error added with success\n");
}

bool ERR_Update(t_WORD moduleID, t_WORD errorID, Severity severity, t_BYTE parameters[7])
{
    errorStorage[errorNumber].timeDelta = GetTickCount() - errorStorage[errorNumber].timeStamp;
    errorStorage[errorNumber].timeStamp = GetTickCount();
    errorStorage[errorNumber].occurrenceCount = errorStorage[errorNumber].occurrenceCount + 1;
    errorStorage[errorNumber].parameters[7] = parameters[7];

    if (severity >= MID)
    {
        if (severity == MID && errorStorage[errorNumber].occurrenceCount > MAX_REPEAT_ERROR_MID)
        {
            errorStorage[errorNumber].severity = HIGH;
        }
        else if (severity == HIGH && errorStorage[errorNumber].occurrenceCount > MAX_REPEAT_ERROR_HIGH)
        {
            errorStorage[errorNumber].severity = VERY_CRITICAL;
        }
        else if (severity == VERY_CRITICAL && errorStorage[errorNumber].occurrenceCount > MAX_REPEAT_ERROR_VERY_CRITICAL)
        {
            errorStorage[errorNumber].severity = NeverComeBack;
            ERR_DeInit();
        }
    }

    return true;
}

void ERR_Handler()
{
    // Perform error handling tasks, such as clearing non-critical errors after a certain number of engine starts
    printf("Error handling started successfully\n");

    for (t_BYTE i = 0; i < errorCount; i++)
    {
        if (errorStorage[i].severity <= INFORMATION)
        {
            // Clear non-critical errors after a certain number of engine starts
            if (engineStartCount >= MAX_RETRY_COUNT_INFO)
            {
                // Swap the current error with the last one
                printf("Information clear\n");
                ERR_Remove(&errorStorage[i]);
                errorCount--;
            }
            if (errorStorage[i].severity <= LOW)
            {
                // Clear non-critical errors after a certain number of engine starts
                if (engineStartCount >= MAX_RETRY_COUNT_LOW)
                {
                    // Swap the current error with the last one
                    printf("Error LOW clear\n");
                    ERR_Remove(&errorStorage[i]);
                    errorCount--;
                }
            }
            if (errorStorage[i].severity <= MID)
            {
                // Clear non-critical errors after a certain number of engine starts
                if (engineStartCount >= MAX_RETRY_COUNT_MID)
                {
                    // Swap the current error with the last one
                    printf("Error MID clear\n");
                    ERR_Remove(&errorStorage[i]);
                    errorCount--;
                }
            }
        }
    }
}

void ERR_Remove(ErrorEntry *errorToRemove)
{
    // Find the position of the error to remove
    t_BYTE positionToRemove = 0;
    while (positionToRemove < errorCount && &errorStorage[positionToRemove] != errorToRemove)
    {
        positionToRemove++;
    }

    // If the error is found, swap it with the last one
    if (positionToRemove < errorCount)
    {
        errorStorage[positionToRemove] = errorStorage[errorCount - 1];
        errorCount--;
    }
    else
    {
        // Error not found in the errorStorage array
        printf("Error not found for removal\n");
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

int main()
{
    // Error test
    t_WORD moduleID = 100;
    t_WORD errorID = 22;
    Severity severity = LOW;
    t_BYTE parameters[8] = {2};

    // Run ERR_Init to initializing
    ERR_Init();

    // Simulate checking if the engine has started
    int engineStarted = 0;
    bool authenticateUser = 0;
    t_DWORD NewError;

    // check for engine start
    while (1)
    {
        // Introduce a timer with a 100 ms delay
        Sleep(100);

        if (!engineStarted)
        {
            // Function to authenticate a user to reset all errors
            if (authenticateUser)
            {
                ERR_DeInit();
            }
        }
        else if (engineStarted)
        {
            // Run ERR_CountEngineStart one time after the engine has started
            ERR_CountEngineStart();

            // if (NewError)
            {
                ERR_Get(moduleID, errorID, severity, &parameters[7]);
            }
        }
    }
}