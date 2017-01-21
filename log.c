#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define LINE_SIZE 1024

#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[39m"


// --------------------------------------------------------------------
// AS IS
const char *
code( const char * msg )
{
	return msg;
}

// --------------------------------------------------------------------
// split
char *
split_space( char * line )
{
	line = strchr(line , ' ');
	if( line == NULL ) return NULL;
	*line = '\0';
	line ++;

	return line;
}

// --------------------------------------------------------------------
int
readlog( FILE *fp, char filename[] )
{
	const char  *pattern = "()<>=";
	const char *tpattern = ")(><=";
	char *time, *mode;
	char *line;
	char *pat, *tpat;
	char *chan;
	char *nick;
	size_t fileline;
	char *r;

	char buf[LINE_SIZE];

	fileline = 0;
	while( fgets(buf, LINE_SIZE, fp) != NULL ){
		fileline ++;
		r = strrchr(buf, '\n');
		if( *r == '\n' ) *r = '\0';
		line = buf;

// time <chan:name> priv
// time - name part
// time ! name quit
// time + name (user) to chan
// time name -> name
		time = line;
		if( (line = split_space( line ) ) == NULL) continue;

		printf( BOLD BLUE "%s:%s ", filename, time );
		mode = line;
		if( (line = split_space( line ) ) == NULL) continue;


//  [mode]		       [line]
//  [chan] [name]	       [line]
//
		pat = strchr( pattern, mode[0] );
		if( pat != NULL ) {
// "<$vchan:$nick>             $params[1]"   @ss_privmsg
// "($vchan:$nick)             $params[1]"   @ss_privmsg
// "=$nick=                    $params[1]"   @ss_privmsg
// "<$vchan:$nick>             $params[1]"   @ss_notice
// "($vchan:$nick)             $params[1]"   @ss_notice
// "=$nick=                    $params[1]"   @ss_notice
// ">$vchan:$'nick[$serverno]< $params[1]"   @sp_privmsg
// ">$chan<                    $params[1]"   @sp_privmsg
// ">$vchan:$'nick[$serverno]< $params[1]"   @sp_notice
// ">$chan<                    $params[1]"   @sp_notice
			chan = &mode[1];
			tpat = strrchr( chan, tpattern[pat-pattern] );
			if( tpat == NULL ) {
				goto DUMP;;
			}
			tpat = '\0';
			tpat++;

			nick = strchr( chan, ':' );

			printf( BOLD RED "%c", *pat );
			if( nick ) {
				nick = '\0';
				nick++;
				printf( "%s:%s", chan, nick );
			}
			else {
				printf( "%s", chan );
			}
			printf( "%c" RESET, *tpat );
			printf( " " );

			printf( WHITE "%s" RESET, line );
		}
		else {
DUMP:
// [mode] [name] [mask] [opt] [line]
			printf( BOLD GREEN "%s %s", mode, line );

		}
			printf( "\n" );
	}

	return 0;
}


int
main( int argc, char *argv[] )
{
		int i;
		
	FILE *fp;
	for(i=1; i<argc; i++){
		if ((fp = fopen( argv[i], "r")) == NULL) {
			perror("file open error!!\n");
			exit(EXIT_FAILURE);
		}
		readlog( fp, argv[i] );

		fclose(fp);
	}
	
	return 0;
}


