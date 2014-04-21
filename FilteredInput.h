/*********
  Written by: Christopher Andrews
  Version 0.0.1
  
  Released under GPL, MIT
*********/


#include "Arduino.h"

template< size_t N >
  class FilterStream{
    public:
      FilterStream( Stream &stream, char **data, void (*func)( char ) ) : stream( stream ), data( data ), func( func ), buffer(), dataExists( false ) {}
        
      void flush(){
        if( !dataExists ) return;
        char n = N;
        while( --n ){
          memmove( buffer, buffer + 1, N - 1 );
          buffer[ N - 1 ] = 0;
          test();
        }
        dataExists = false;
      }
        
      void test(){
        for( char **ptr = data, *current ; current = ( char* ) pgm_read_word( ptr ) ; ++ptr ){
          
          char *bcur = buffer, *ccur = current, ch, fail = 0;
          
          while( ch = pgm_read_byte( ccur++ ) ) 
            if( *bcur++ != ch && ( fail = 1 ) ){ break; }
            
          if( !fail ){
             func( ptr - data );
             memset( buffer, ' ', strlen_P( current ) );
          }
        }
      }
      
      bool run( void ){
        
        if( !stream.available() ) return false;
        
        for( char ch = stream.read() ; ch != -1 ; ch = stream.read() ){
          
          if( isAlpha( ch ) ){                 // || isNumber() ){
            
            dataExists = true;
            memmove( buffer, buffer + 1, N - 1 );
            buffer[ N - 1 ] = ch;
            test();
          }
        }
        return true;
      }
    protected:
      inline static bool notCapsAlpha( char c ) { return c < 'A' || c > 'Z'; }
      inline static bool isAlpha( char &c ) { return notCapsAlpha( c ) ? ( c >= 'a' && c <= 'z' ) : (c = tolower( c ), true); }
      //inline static bool isNumber( char c ) { return c >= '0' || c <= '9'; }
    
      Stream &stream;
      char **data;
      void (*func)( char );
      char buffer[ N + 1 ];
      bool dataExists;
};
