Problem 1
-------------------
**Host A wants to send a 2,000 KB file to Host B. The Round Trip Time (RTT) of the Duplex Link between Host A and B is 160ms. Packet size is 1KB. One handshake between A and B is needed before data packets can start transferring which takes 2xRTT. Calculate the total required time of file transfer in the following cases. The transfer is considered complete when the final packet reaches B:**


Problem 2
---------------------
**Suppose a 50-Mbps point-to-point link is being setup between Earth and a new lunar base. The distance from the moon to Earth is approximately 385,000 km, and data travels over the link at the speed of light 3 x 108 m/s.**

Problem 3
---------------------------




Problem 4
------------------------
**Consider a more realistic version of Problem 1. This is a simple protocol for file transfer between A and B. After some initial negotiation, A sends data packets of size 1 KB to B; B then replies with an acknowledgment. A always waits for each ACK before sending the next data packet; this is known as stop-and-wait. Packets that are overdue are presumed lost and are retransmitted**

(a).In the absence of any packet losses or duplications, when we are expecting the Nth packet we get the Nth packet, and so we can keep track of N locally at the receiver.

(b).The scheme outlined here is the stop-and-wait algorithm of Section 2.5; as is indicated there, a header with at least one bit of sequence number is needed (to distinguish between receiving a new packet and a duplication of the previous packet)

(c).With out-of-order delivery allowed, packets up to 1 minute apart must be distinguishable via sequence number. Otherwise a very old packet might arrive and be accepted as current. Sequence numbers would have to count as high as bandwidth × 1minute /packet size
