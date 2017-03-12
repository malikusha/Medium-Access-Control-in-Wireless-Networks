# Medium-Access-Control-in-Wireless-Networks
Ad-hoc wireless networks allow computers with radios to communicate with each other without requiring a special coordinator. This project is the simulation of that network.


Wireless nodes can transmit using one of three non-overlapping wireless 2.4 GHz channels: 1, 6, or 11. Each node will randomly start by selecting a channel to listen and transmit on (it uses the same channel for both). Each wireless node will be place randomly on a 100 foot by 100 foot grid at a 1 foot granularity (i.e., a node may be at position 13,5, but not at position 12.3,4.7). Any node within a 10 foot square centered at the node will be able to see traffic from
that transmitter. This also means that when a sender transmits, no other node can transmit on that same frequency if that node’s transmission range would overlap the original sender’s range, since it will lead to a collision. A node may change which wireless channel it uses periodically, but it should not do so simply to avoid collisions or contention.
Each wireless node will randomly choose a message to send and when to send it. All messages in the network are broadcasts and all messages have a randomly generated 4-byte ID field created by the sender. If a node has seen a message with the same ID before, it will ignore the message. Otherwise, the node will keep a record of the messages, including the associated ID, who the node received the message from, what channel it was received on, when it received it, along with the node’s own geographic coordinates. This log should be written to a file, with a unique file for each node, to make debugging easier. The node will then rebroadcast the message once it has an opportunity to do so. With enough nodes, the message is likely to be broadcast across the entire network and be seen by all nodes.
Another type of node also exists in the network: the noise-
maker. This node produces interference on a channel on a
random basis and for a random period of time. It does not
behave cooperatively with other nodes; instead, it prevents all
messages from being delivered while active. Common noisemakers are innocuous devices, such as microwave ovens, some of which are known to produce radio frequency interference for WiFi networks. Each noise- maker should have its own log file and record each time it activates and deactivates, including its location and what channel it used when making noise.
There are several important variables to consider in the experimental design:
• num nodes: The number of regular wireless nodes in the network
• dwell duration: The amount of time a regular wireless node stays on a given wireless channel before considering whether to switch to another wireless channel
• dwell probability: The probability that a regular wireless nodes will choose to stay on the same wireless channel after dwell duration elapses
• transmission time: The amount of time, in milliseconds, it takes for a wireless node to send its message. In practice, this time is really short. However, to be able to actually see what is happening, students may want to set the transmission time to be a higher value. A higher value is also likely to increase the chance of contention, allowing students to determine if their synchronization primitives actually work.
• talk window time: A node must periodically consider whether to send a message or not. The talk window is the amount of time, in milliseconds, for that period. A separate value, talk probability, will determine how likely it is that the node will decide to send something at each new window time.
• talk probability: At each talk window time, this is the probability the node will decide to generate and send a new message. The higher the talk probability, the more messages that will be sent on the network and the more opportunity for collisions and synchronization challenges.                                                                       
• num noisemakers: This option is equivalent to num nodes, but specifies the value for noisemakers, rather than regular nodes.
• dwell noisemakers: This option is equivalent to dwell duration, but specifies the value for noise- makers, rather than regular nodes.
• dwell probability noisemakers: This option is equivalent to dwell probability, but specifies the value for noisemakers, rather than regular nodes.

The description above is taken from handout from the Operating Systems class at WPI taught by Professor Craig Shue


Our approach:

The mutexes:
The nodes are laid out in a 100 by 100 grid. Each node has an action and a shield;
the shield is the crux of the solution. the principle of a shield is that another node
may put up a shield on a node if there is not already a shield; afterwards, any other
node trying to put up a shield will have to wait for the shield to go away before doing so.
For a node to set up its network, it must first place shields on all its neighbors;
this way, they have exclusive rights to check the neighbors nodes. Then, the node
actually checks its neighbors actions; if any of them are currently in a network,
it puts down its shields and waits for the node to stop being in a network. After
all neighbors are available, the node connects with them, sends a message, and
finally tells the nodes to disconnect from the network. This method prevents deadlock
and race conditions from taking place in connecting/disconnecting. When a node wants
to change its channel, it waits until it can put a shield on itself and until it is
no longer in a network; it then decides through rand whether to change channels.
Noisemakers increment a counter in its neighbors nodes when they decide to block;
when a node wants to send a message, if its block counter is greater than zero,
it instead reports that it is blocked and does nothing. If more than one noisemaker
wishes to block, the counter increments for each; this way if one noisemaker stops,
there is still blocking for the node. Messages are generated each time a node decides
to send a new message; they take priority over any messages that must be rebroadcast.
Nodes decide whether to transmit before each transmit attempt, and wait through their
transmission as the transmission occurs. A node will not receive the same message twice.

The semaphores:
For semaphores we made an assumption that the node can transmit the message to one node at a time, if that node is already receiving the message, then we check the next neighbor to transmit to. When checking the neighbor’s state(Transmit, receive or neither), we follow the same principle with shields. 
There are total of 6 semaphores for each node. shield_lock semaphore protects the segment of the code when the shields are established, shield_unlock semaphore protects the segment where the shields are put down, action_lock is a semaphore to protect the code during the time when we are changing the state of the node(i.e. from neither to receiving, from neither to transmitting), action_unlock(from receiving state to neither etc), noise_lock to protect the code when the interference counter changes its value and message_lock to protect the code when the messages are added to or retrieved from the messages queue. 
Here, when “protect the code” is used, it is referred to semaphore actually decreasign its value whenever the new process is trying to run through that piece of code and when it sees that the other thread is running the same code, it cannot access it and puts itself to sleep. All the semaphore values were initialized to 1, since we want only one thread enter our critical pieces of code at a time. So, when the semaphore is negative, that negative number represents the number of threads waiting to enter that piece of code. When semaphore post is called, it increments the semaphore by one and wakes up one of the waiting threads. The other concepts and assumptions are inherited from the mutexes implementation. 
The only important point that is different from the muteness is that for this part we made an assumption that the node can transfer only to one node at a time.








