
 
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<semaphore.h>
#include<pthread.h>

sem_t s;
int seqNo = 0, timedOut = 0;
pthread_t t1,t2,t3;
int count = 0;
struct sendPacket {
       int sequence;
       int number;
};

struct receivedPacket {
       int sequence;
       int reply;
};
void *timer (void *c)
{
  count++;
  int td = *((int *) c);
  printf ("Started Timer %d for %d seconds\n", count, td);
  sleep (td);
  timedOut = 1;
  sem_post (&s);
  pthread_exit (NULL);
}


void *sender (void *arg)
{
  int cid = *((int *) arg);
  struct sendPacket send;
  int td = 5;
  while (1)
  {
      if (!timedOut)
      {
        td=5;
        printf("\nEnter natural number: ");
        scanf("%d", &send.number);
        send.sequence = seqNo;
        write (cid, &send, sizeof (send));

        if(send.number<=0){
          printf("ERR!!! NOT NATURAL NUMBER\n\n terminated \n");
          pthread_cancel (t2);
          pthread_exit (NULL);
        }
        seqNo = (seqNo + 1) % 2;
      }
      else if (timedOut)
      {
        seqNo = (seqNo + 1) % 2;
        printf ("\nAcknowledgement not Received. Timer timed out.\n");
        printf ("Re-Sending Sequence number %d with message %d\n", send.sequence, send.number);
        write (cid, &send, sizeof (send));
        td=10;
      }
      timedOut = 0;
      pthread_create (&t3, NULL, timer, (void *)&td);
      sem_wait (&s);
      pthread_join (t3, NULL);
  }
  pthread_exit (NULL);
}


void *receiver (void *arg)
{
  int cid = *((int *) arg);
  struct receivedPacket received;
  int ack;
  while (1)
  {
      read (cid, &received, sizeof (received));
      
      if (received.sequence == seqNo)
      {
        printf ("\nAcknowledgement %d Received\nReply: %s\n\n", received.sequence, (received.reply ? "PRIME" : "COMPOSITE") );
        pthread_cancel (t3);
        timedOut = 0;
        sem_post (&s);
      }
  }
  pthread_exit (NULL);
}


int
main ()
{
  int cid = socket (AF_INET, SOCK_STREAM, 0);
  perror ("socket");

  struct sockaddr_in c;
  c.sin_family = AF_INET;
  c.sin_port = htons (5555);
  inet_aton ("127.0.0.1", &c.sin_addr);
  connect (cid, (struct sockaddr *) &c, sizeof (c));
  perror ("connect");

  sem_init(&s, 0, 1);
  
  pthread_create (&t1, NULL, sender, (void *) &cid);
  pthread_create (&t2, NULL, receiver, (void *) &cid);
  pthread_join (t1, NULL);
  pthread_join (t2, NULL);
  sem_destroy(&s);
  return 0;
}

