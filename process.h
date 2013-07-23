/*
 * 
 *  Copyright (c) 2012
 *  name : mhogo mchungu 
 *  email: mhogomchungu@gmail.com
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 * 
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 * 
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PROCESS
#define PROCESS

#ifdef __cplusplus
extern "C" {
#endif
	
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <stdarg.h>
#include <sys/types.h>

#include <sys/time.h>
#include <sys/resource.h>
	
typedef struct{
	/*
	 * if this variable is set,then it is expect to be in the same format the last argument of execv() expect
	 */
	char * const * args ;
	/*
	 * this variable is set will cause the program to be executed will run with the identify of this user.
	 */
	uid_t user_id ;
	/*
	 * If this variable is set,then it is expected to be in the format the last argument of execve() expect
	 */
	char * const * env ;
	/*
	 * If this variable is set,the process will be terminate if its still running after timeout seconds
	 * The default signal sent is sigterm
	 */
	int timeout ;
	/*
	 * if this variable is set,then this signal will be sent when the timeout above expires
	 */
	int signal ;
	/*
	 * priority of forked process will run with
	 */
	int priority ;
}ProcessStructure ;

typedef struct ProcessType_t * process_t ;

#define ProcessVoid ( ( process_t ) 0 ) 

/*
 * use this variable to terminate variadic functions
 * we dont use NULL here because NULL is defined simply as 0 and things break in C++
 */
#define ENDLIST ( ( const char * ) 0 )

/*
 * An example of how to use the library to call ls with arguments and get its output while closing its std error 
 * 
 *  process_t p = Process( "/bin/ls" ) ; 
 *  ProcessSetArgumentList( p,"-l","-h",ENDLIST ) ;  
 *  ProcessStart( p ) ; 
 *  char * c = NULL ;
 *  ProcessGetOutPut( p,&c,STDOUT ) ; 
 *  ProcessDelete( &p ) ;
 *  if( c ){ 
 *	  printf("%s\n",c );
 *	  free( c ) ;
 * }
 * 
 */

/*
 * Takes a pointer to a function to be called when memory allocation can not take place
 * ie if the system has run out of memory and malloc() or realloc() has failed.
 * This function is optional and "ProcessVoid" will be returned on memory exaustion if the function
 * isnt set.
 */
void ProcessExitOnMemoryExaustion( void (*)( void ) ) ;

/*
 * create a handle to a process that will be started.
 * Path to executable to be started must be in full path format
 */
process_t Process( const char * path ) ;

/*
 * This structure can be used an an alternative way to set various options.
 * There exist a separate function to set each one of them
 */
ProcessStructure * ProcessArgumentStructure( process_t ) ;

/*
 * call fork() and then exec and run a program specified when handle p was created.
 * return value: pid of the child process 
 */
pid_t ProcessStart( process_t p ) ;

/*
 * pass data to the child process,the child process will get the data from reading its stdin.
 * ProcessSetOption( p,WRITE_STD_IN ) must be called first for this to work. * 
 */
size_t ProcessWrite( process_t p,const char * data,size_t len ) ;

/*
 * close the writing connection to the child process.This maybe necessary if a child process
 * blocks waiting for more data in its std in until EOF is received. 
 */
void ProcessCloseStdWrite( process_t p ) ;

/*
 * remember to clean after yourself
 */
void ProcessDelete( process_t * ) ;

/*
 * send a forked process sigterm to terminate it.
 */
int ProcessTerminate( process_t ) ;

/*
 * run the child process with privileges of a user with UID of uid
 */
void ProcessSetOptionUser( process_t,uid_t uid ) ;

/*
 * set the nice value the forked process will run with 
 */
void ProcessSetOptionPriority( process_t,int priority ) ;

/*
 * send a forked process sigkill to kill it
 */
int ProcessKill( process_t ) ;

/*
 * the forked process is started by calling, execl.
 * this function can be used to set argument list *
 */
void ProcessSetArguments( process_t p,char * const argv[] ) ;

/*
 * the forked process is started with arguments list given as const char * and the
 * last entry ust be null.
 * example:
 * process_t p = Process( "/usr/bin/du" ) ;
 * ProcessSetArgumentList( p,"-s","-c","-h",NULL ) ;
 * ProcessStart( p ) ;
 * ProcessDelete( &p ) ;
 */
void ProcessSetArgumentList( process_t p,... ) ;

/*
 * set the child process to start with the given enviromental variables
 */
void ProcessSetEnvironmentalVariable( process_t p,char * const * env ) ;

/*
 * get state of the process handled by handle p 
 */
#define HAS_NOT_START 1
#define RUNNING 2
#define FINISHED 3
#define CANCELLED 4
int ProcessState( process_t p ) ;

/*
 * wait for "timeout" seconds and then send the forked process a signal "signal"
 */
void ProcessSetOptionTimeout( process_t p,int timeout,int signal ) ;

/*
 * waitpid() for forked process to exit and get its exit status.
 * If the exit status is not waiter for,waitpid() with WNOHANG argument will be called when calling ProcessDelete
 * to make sure the forked process doesnt turn to a zombie.
 */
int ProcessExitStatus( process_t ) ;

/*
 * get contents of std out/std error from the process.
 * remember to free() the return buffer when done with it.
 * 
 * return value is the size of the data.  
 * this function must be called after ProcessStart()
 */

#define STDOUT 1 
#define STDERROR 2
size_t ProcessGetOutPut( process_t,char ** data,int stdio ) ;

/*
 * read size number of bytes from the ourput of the forket process.
 */
ssize_t ProcessGetOutPut_1( process_t,char * buffer,int size,int stdio ) ;

#ifdef __cplusplus
}
#endif

#endif
