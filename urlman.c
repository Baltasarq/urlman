// urlman (c) Baltasar 2018 MIT License <baltasarq@gmail.com>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

const char * Name = "UrlMan";
const char * Version = "v0.1";
const char * URLBegin = "url=";
const long Serial = 2018;
const char * Help = "Launches files containing URLs in plain or "
                    "Microsoft .url format.\n";

const int MaxBuffer = 8192;
const int MaxLines = 11;

/** Removes the \n at the end of of the line.
  * @param url the input line
  */
void remove_cr(char * url);

/** Extracts the url from the file in question.
  * @param fn the name of the file.
  * @param url a buffer to be filled with the url.
  */
void parse_file(const char * fn, char * url);

/** Extracts the url from the read lines.
  * @param num_lines the number of lines read.
  * @param fn an array of MaxLines pointers to strings.
  * @param url a buffer to be filled with the url.
  */
void parse_lines(int num_lines, char ** lines, char *url);

int main(int argc, char ** argv)
{
    printf( "%s %s %ld\n\n", Name, Version, Serial );
    
    if ( argc == 2 ) {
        char url[MaxBuffer];
        char cmd[MaxBuffer];
        
        parse_file( argv[ 1 ], url );
        remove_cr( url );
        
        if ( strlen( url ) > 0 ) {
            sprintf( cmd, "xdg-open \"%s\" &", url );
            printf( "Launching:\n\t'%s'\n", cmd );
            system( cmd  );
        } else {
            fprintf( stderr, "ERROR: url not found\n" );
        }
    } else {
        printf( "\n%s\n", Help );
    }

    printf( "End.\n" );
    return EXIT_SUCCESS;
}

void parse_file(const char * fn, char * url)
{
    char buffer[MaxBuffer];
    char ** lines = malloc( sizeof( char* ) * MaxLines  );
    FILE * f = fopen( fn, "rt" );
    
    // Init lines structure
    for(int i = 0; i < MaxLines; ++i) {
        lines[ i ] = NULL;
    }
    
    if ( f != NULL ) {
        int num_lines = 0;
        
        while ( num_lines < MaxLines
             && fgets( buffer, MaxBuffer, f ) != NULL )
        {
                lines[ num_lines ] = strdup( buffer );
                ++num_lines;
        }

        if ( num_lines == 0
          || ( num_lines == 1
            && ( lines[ 0 ] != NULL
              || strlen( lines[ 0 ] ) == 0 ) ) )
        {
            *url = 0;
        }
        else
        if ( num_lines == 1 ) {
            strcpy( url, lines[ 0 ] );
        } else {
            parse_lines( num_lines, lines, url );
        }
    } else {
        *url = 0;
    }
     
    // Free individual lines
    for(int i = 0; i < MaxLines; ++i) {
        free( lines[ i ] );
    }
    
    free( lines );
    return;
}

void parse_lines(int num_lines,  char ** lines, char * url)
{
    // Mark error, just in case
    *url = 0;
   
    // Look for the url
    for(int i = 0; i < num_lines; ++i) {
        char * line = lines[ i ];
        const int URLBeginLength = strlen( URLBegin );
        const int LineLength = strlen( line );
        
        if ( line != NULL
          && LineLength > URLBeginLength )
        {
            // Lower case
            for(int i = 0; i < URLBeginLength; ++i) {
                line[ i ] = tolower( line[ i ] );
            }
            
            // Compare
            if ( !strncmp( line, URLBegin, URLBeginLength ) ) {
                strcpy( url, line + URLBeginLength );
            }
        }
    }
    
    return;
}

void remove_cr(char * s)
{
    if ( s != NULL ) {
        const int Length = strlen( s );
        
        for(int i = Length - 1; i >= 0; --i) {
            const char ch = s[ i ];
            
            if ( isspace( ch ) ) {
                s[ i ] = 0;
            } else {
                break;
            }
        }
    }
    
    return;
}
