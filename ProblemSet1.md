Problem 1
-------------------
**Host A wants to send a 2,000 KB file to Host B. The Round Trip Time (RTT) of the Duplex Link between Host A and B is 160ms. Packet size is 1KB. One handshake between A and B is needed before data packets can start transferring which takes 2xRTT. Calculate the total required time of file transfer in the following cases. The transfer is considered complete when the final packet reaches B:**

![](http://s12.postimg.org/udwuybhr1/img001.jpg)


Problem 2
---------------------
**Suppose a 50-Mbps point-to-point link is being setup between Earth and a new lunar base. The distance from the moon to Earth is approximately 385,000 km, and data travels over the link at the speed of light 3 x 108 m/s.**

![](http://s10.postimg.org/mlqypbjuh/img002.jpg)

Problem 3
---------------------------
![]()




Problem 4
------------------------
**Consider a more realistic version of Problem 1. This is a simple protocol for file transfer between A and B. After some initial negotiation, A sends data packets of size 1 KB to B; B then replies with an acknowledgment. A always waits for each ACK before sending the next data packet; this is known as stop-and-wait. Packets that are overdue are presumed lost and are retransmitted**

(a).In the absence of any packet losses or duplications, when we are expecting the Nth packet we get the Nth packet, and so we can keep track of N locally at the receiver.

(b).The scheme outlined here is the stop-and-wait algorithm of Section 2.5; as is indicated there, a header with at least one bit of sequence number is needed (to distinguish between receiving a new packet and a duplication of the previous packet)

(c).With out-of-order delivery allowed, packets up to 1 minute apart must be distinguishable via sequence number. Otherwise a very old packet might arrive and be accepted as current. Sequence numbers would have to count as high as bandwidth × 1minute /packet size

Problem 5 
-----------------
**Consider a go-back-n sliding window algorithm running over a 40-km point-topoint fiber link.**

![]()


Problem	6
---------------
**Draw a time line diagram for the sliding window algorithm with SWS = RWS = 3 frames, for the following two situations. Use a timeout interval of about 2 × RTT.**

![]()


Problem 7 
---------------
**Suppose that we attempt to run the sliding window algorithm with SWS = RWS = 3 and with MaxSeqNum = 5. The N th packet DATA[N] thus actually contains N mod 5 in its sequence number field. Assume no out-of-order arrivals. Give an example in which the algorithm becomes confused PLEASE DRAW IT OUT; that is, a scenario for example which the receiver expects DATA[5] and accepts DATA[0]—which has the same transmitted sequence number—in its stead. Note that this implies MaxSeqNum ≥ 6 is necessary and MaxSeqNum = 6 is sufficient.**

![]()

Problem	8
---------------
**Consider the following networked computers connected by Bridge X and Y. Bridge X has interface 1,2 and3. Bridge Y has interface 1 and 2. Assume at the beginning the address tables of Bridge X and Y are all empty. Write down the address tables of Bridge X and Y after the following communication finished.**

![]()

Problem 9 - 10
--------------------------

![]()

