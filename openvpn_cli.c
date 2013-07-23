

#include "process.h"

#include <grp.h>
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
	
	int fd = open( log_path,O_CREAT|O_TRUNC|O_WRONLY,S_IRUSR|S_IWUSR ) ;
	
	if( fd == -1 ){
		puts( "failed to open log file" ) ;
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

static int userIsPrivileged( void )
{
	int st = 0 ;
	
	const char ** entry ;
	const char * name   ;
	
	uid_t uid = getuid() ;
	
	struct group * grp ;
	
	struct passwd * pass ;
	
	if( uid == 0 ){
		return 1 ;
	}
	
	pass = getpwuid( uid ) ;
	
	if( pass == NULL ){
		return 0 ;
	}
	
	grp = getgrnam( "openvpn-cli" ) ;
	
	if( grp == NULL ){
		return 0 ;
	}
	
	name  = ( const char * )pass->pw_name ;
	entry = ( const char ** )grp->gr_mem ;
	
	while( *entry != NULL ){
		if( strcmp( *entry,name ) == 0 ){
			st = 1 ;
			break ;
		}else{
			entry++ ;
		}
	}
	
	return st ;
}

static int writeToLogFile( const char * msg )
{	
	struct passwd * pass = getpwuid( getuid() ) ;
	
	if( pass == NULL ){
		return 1 ;
	}
	
	char log_path[ 1032 ] ;
	snprintf( log_path,1032,"/home/%s/.openvpn/output.log",pass->pw_name ) ;
	
	seteuid( getuid() ) ;
	int fd = open( log_path,O_CREAT|O_TRUNC|O_WRONLY,S_IRUSR|S_IWUSR ) ;
	seteuid( 0 ) ;
	
	if( fd == -1 ){
		puts( "failed to open log file" ) ;
	}else{
		puts( msg ) ;
		fchmod( fd,444 ) ;
		write( fd,msg,strlen( msg ) ) ;
		close( fd ) ;
	}
	
	return 1 ;
}

int main( int argc,char * argv[] )
{
	if( argc != 2 ){
		return writeToLogFile( "argument to openvpn .ovpn or .config file is missing" ) ;		
	}
	
	if( !userIsPrivileged() ){
		return writeToLogFile( "insufficient privileges to run this application.\nMake sure you are a member of group \"openvpn-cli\" and try again" ) ;
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
