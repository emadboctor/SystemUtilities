#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>


int main( int argc, char** argv )
{
   int rc = 0 ;

   struct addrinfo aiHints = { 0 } ;
   struct addrinfo *aiList = NULL ;

   char szServerHostName[ 100 ] = { '\0' } ;
   char szIPAddress[ 100 ] = { '\0' } ;
   

   if ( argc < 2 ) 
   {
      printf( "Usage:  %s <hostname to resolve> \n", argv[ 0 ] ) ;
      rc = -1 ;
      goto exit ;
   }

   strcpy( szServerHostName, argv[ 1 ] ) ;


   printf( "Will get address info for %s\n", szServerHostName  ) ;


   aiHints.ai_family = AF_INET ;
   aiHints.ai_socktype = SOCK_STREAM ;
   aiHints.ai_protocol = IPPROTO_TCP ;
   aiHints.ai_flags    = AI_CANONNAME ;

   for ( int i = 0 ; 
         i  < 5 ;
         i++ ) 
   {
      rc = getaddrinfo( szServerHostName, NULL, &aiHints, &aiList ) ;

      if ( EAI_AGAIN !=rc )
      {
         break ;
      }
      
      sleep ( 1 ) ;
   }

   if ( rc ) 
   {
      printf( "getaddrinfo failed, rc is: %d\n", rc ) ;
      goto exit ;
   }

   printf( "Canonical hostname of %s is %s\n", szServerHostName, aiList->ai_canonname ) ;

   for ( int i = 0 ;
         i < 5 ;
         i++ ) 
   {
      rc = getnameinfo( aiList->ai_addr,
                        sizeof( *(aiList->ai_addr) ),
                        szIPAddress,
                        sizeof( szIPAddress),
                        NULL,
                        0,
                        NI_NUMERICHOST ) ;
      if ( EAI_AGAIN != rc ) 
      {
         break ;
      }
   }

   if( rc ) 
   {
      printf( "getnameinfo failed, rc is: %d\n", rc ) ;
      goto exit ;
   }

   printf( "IP Address of the canonical hostname of %s is %s\n", szServerHostName, szIPAddress ) ;
   


exit:
   return rc ;
}
