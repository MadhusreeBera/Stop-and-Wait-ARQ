
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
struct sendPacket {
       int sequence;
       int reply;
};

struct receivedPacket {
       int sequence;
       int number;
};
int isPrime(int n){
  if(n<2) return 0;
  if(n==2) return 1;
  if(n%2 == 0) return 0;
  int i;
  for(i = 3; i*i<=n; i+=2)
    return (n%i == 0 ? 0 : 1);
  return 1;
}
int main ()
{
  struct sockaddr_in s, c;
  int  ack = 0, seed = 10001000, delay;
  int sid = socket (AF_INET, SOCK_STREAM, 0);
  perror ("socket");
  
  s.sin_family = AF_INET;
  s.sin_port = htons (5555);
  inet_aton ("127.0.0.1", &s.sin_addr);
  bind (sid, (struct sockaddr *) &s, sizeof (s));
  perror ("bind");

  listen (sid, 1);
  perror ("listen");

  int l = sizeof (c);
  int cid = accept (sid, (struct sockaddr *) &c, &l);
  perror ("accept");
  
  struct receivedPacket rs;
  struct sendPacket ss;
  srand (seed);
  while (1)
    {
      read (cid, &rs, sizeof (rs));
      if (rs.sequence == ack)
      {
        if(rs.number <= 0)
        { 
          printf("Client closed connection\n");
          break;
        }
	      printf ("\nReceived sequence %d and message %d\n", rs.sequence, rs.number);
		    ack = (ack + 1) % 2;
        ss.sequence = ack;
	    }
      else
		    printf ("\nReceived DUPLICATE frame: %d\n", rs.sequence);
	    
      ss.reply = isPrime(rs.number);
	    delay = (rand() % 10) + 1;
     
      sleep (delay);
      printf("REPLY: %s", (ss.reply ? "PRIME" : "COMPOSITE"));
      write (cid, &ss, sizeof (ss));
      
      printf ("\nAcknowledgement %d sent after %d seconds\n", ack, delay);
    }
  return 0;
}

