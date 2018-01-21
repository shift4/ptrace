/********************************************************************
 * File: instrument.c
 *
 * Instrumentation source -- link this with your application, and
 *  then execute to build trace data file (trace.txt).
 *
 * Author: M. Tim Jones <mtj@mtjones.com>
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct func
{
    char addr[20];
    char funcname[50];
};
static struct func funcs[2000];

/* Function prototypes with attributes */
void main_constructor( void )
	__attribute__ ((no_instrument_function, constructor));

void main_destructor( void )
	__attribute__ ((no_instrument_function, destructor));

void __cyg_profile_func_enter( void *, void * ) 
	__attribute__ ((no_instrument_function));

void __cyg_profile_func_exit( void *, void * )
	__attribute__ ((no_instrument_function));

static struct func getfunc( unsigned long )
	__attribute__ ((no_instrument_function));

static FILE *fp;

static struct func getfunc(unsigned long addr)
{
    int i = 0;
    while (strlen(funcs[i].addr) != 0 && strtoul(funcs[i].addr,0,16) != addr){
        i++;
    }
    return funcs[i];
}

void main_constructor( void )
{
    int i;

    i = 0;
    memset(funcs, 2000, sizeof(struct func));
    fp = fopen("calls", "r");
    while (!feof(fp))
    {
        fscanf(fp, "%s %s", funcs[i].addr, funcs[i].funcname);
        i++;
    }
    fclose(fp);

  fp = fopen( "trace.log", "w" );
  if (fp == NULL) exit(-1);
    setvbuf(fp,NULL,_IOLBF,1);
}


void main_deconstructor( void )
{
  fclose(fp);
}


void __cyg_profile_func_enter( void *this, void *callsite )
{
    struct func f;
    f = getfunc((unsigned long)this);
    if(strlen(f.addr)==0){
        fprintf(fp, "E %p\n", this);
    }else{
        fprintf(fp, "E %s\n", f.funcname);
    }
}


void __cyg_profile_func_exit( void *this, void *callsite )
{
    struct func f;
    f = getfunc((unsigned long)this);
    if(strlen(f.addr)==0){
        fprintf(fp, "X %p\n", this);
    }else{
        fprintf(fp, "X %s\n", f.funcname);
    }
}
