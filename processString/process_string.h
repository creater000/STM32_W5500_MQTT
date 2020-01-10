#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stm32f10x.h>


#define MAX_OUTPUT_LEN             32
#define EXIST_FIELD                 0 
#define NOT_EXIST_FIELD            -1

#define FIELD_CONTROL_OUPUT        "CMD"

int getValueInt(char* jsonData,uint32_t iStart,uint32_t iEnd,char* fieldName,uint32_t* outputNumber);
void free_buffer(char* buffer, uint16_t len);