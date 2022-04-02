# Stop-and-Wait-ARQ

This is a C program to implement Stop and Wait Automatic Repeat Request which is used in Data Link layer and Transport layer.
There is a sender and a receiver between which message is transferred. 

Every packet is numbered. The sender sends a packet which has a number. The receiver checks whether the number is prime or composite and sends back the reply along with acknowledgement (1 or 0) for the next packet. The sequence number required for Stop and Wait ARQ is 1 and 0 and this fulfills all the test cases. Whenever the receiver does not receive the acknowledgement packet (within time-out time) for the next frame, it guesses that the transmission must have failed and re-sends the previous frame. If the receiver receives duplicate frame then the reciver discards the duplicate frame and again re-sends the acknowledgement for the expected packet.

The sender after sending the packet starts a timer and waits for ( 2 * Round Trip Time ) i.e twice the time a packet takes to move to and from between sender and receiver.  

Files

sender (client): stwt_c.c

receiver (server): stwt_s.c

to run the files

1. open 2 terminals preferably side by side
2. in first terminal compile the receiver using : gcc -o stwt_s.c receiver
3. in the second terminal compile the sender using: gcc -o stwt_c.c sender
4. run the executable file in first terminal using : ./receiver
5. run the executable file in second terminal using : ./sender

We will get the output as shown below:

![output 10](https://user-images.githubusercontent.com/73298219/161391691-bacd45b1-34fb-4915-bc7f-09b6df4236a8.png)

To stop the sender press: Ctrl + C
