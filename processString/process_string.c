#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stm32f10x.h>
#include "process_string.h"

int getValueInt(char* jsonData,uint32_t iStart,uint32_t iEnd,char* fieldName,uint32_t* outputNumber)
{
  char fieldPlus[20]={0};
  char bufferOutputnumber[11]={0};
  memset(fieldPlus,0,20);
  memset(bufferOutputnumber,0,11);
  
  uint32_t patternLen=0;  
  uint32_t i=0,m=0,n=0;   
  strcat(fieldPlus,"\"");
  strcat(fieldPlus,fieldName);
  strcat(fieldPlus,"\":");  
  patternLen =strlen(fieldPlus);    
  i = iStart;  
  while (i <= (iEnd - patternLen))
  {
    m = 0;
    // if first character of search string matches    
    if(jsonData[i+m]==fieldPlus[m])
    {
      while((m<patternLen) && (jsonData[i+m] == fieldPlus[m])) 
      {        
        m++;
      }
      if(m==patternLen)
      {   
        n = 0;
        while((m+i < iEnd) && (n < MAX_OUTPUT_LEN) && (jsonData[m+i] != ',') && (jsonData[m+i] != '}'))
        {
          bufferOutputnumber[n]=jsonData[i+m];
          m++;
          n++;
        }
        *outputNumber=atol(bufferOutputnumber);        
        return 0;
      }         
    }
    i++;
  }
  return -1;  
}


void free_buffer(char* buffer, uint16_t len)
{
  uint16_t i=0;
  for(i=0;i<len;i++)
  {
    buffer[i] = 0;    
  }
}