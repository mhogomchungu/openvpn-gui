

#include "process.h"

#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <pwd.h>
#include <signal.h>
#include <sys/stat.h> 

static process_t p ;

static void _exitProcess( int r )
{
	if( r ){;}
	//ProcessTerminate( p ) ;
	//sleep( 1 ) ;
	ProcessKill( p ) ;
}

static void _printProcessOutput( process_t p )
{
	char * e ;
	int s ;
	
	struct passwd * pass = getpwuid( getuid() ) ;
	
	if( pass == NULL ){
		return ;
	}
	
	char log_path[ 1032 ] ;
	snprintf( log_path,1032,"/home/%s/.openvpn/output.log",pass->pw_name ) ;
	
	int fd = open( log_path,O_CREAT|O_TRUNC|O_WRONLY,S_IRUSR|S_IWUSR) ;
	if( fd == -1 ){
		return ;
	}
	
	fchmod( fd,444 ) ;
	while( 1 ){
		e = NULL ;
		s = ProcessGetOutPut( p,&e,STDOUT ) ;
		if( e ){
			write( fd,e,s ) ;
			free( e ) ;
		}else{
			break ;
		}
	}
	
	close( fd ) ;
}

int main( int argc,char * argv[] )
{
	if( argc != 2 ){
		printf( "argument to openvpn .ovpn or .config file is missing" ) ;
		return 1 ;
	}
	
	/*
	 * below signal handling part doesnt seem to be necessary
	 */
	
	struct sigaction sa ;
	memset( &sa,'\0',sizeof( struct sigaction ) ) ;
	sa.sa_handler = _exitProcess ;
	sigaction( SIGHUP,&sa,NULL ) ;
	sigaction( SIGTERM,&sa,NULL ) ;
	
	const char * opt_file = argv[ 1 ] ;
	
	p = Process( "/usr/sbin/openvpn" ) ;
	
	ProcessSetOptionUser( p,0 ) ;
	
	ProcessSetArgumentList( p,opt_file,ENDLIST ) ;
	
	ProcessStart( p ) ;
	
	_printProcessOutput( p ) ;
	
	int r = ProcessExitStatus( p ) ;
	
	ProcessDelete( &p ) ;
	
	return r ;
}
