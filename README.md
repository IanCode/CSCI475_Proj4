# CSCI475-Proj1
Implementing a Xinu ready queue for Oracle VM VirtualBox ~5.1.14.

This repository is being used as a collaborative workspace for project group memebers.

Project 1
Authors: Jack Burns and Ian White

In this project we implemented Xinu's Ready Queue. Our Queue and Qentry structs were fairly basic, each consisting of a few pointers and a pid value. The main issue we ran into during the project was using malloc() in enqueue. This caused the pid for the head of the queue to be exremely large, and thus lead to various runtime errors. It also caused issues when calling free() in dequeue. One strategy that helped with debugging was adding various kprintf() statements in our functions to identify when values were being changed. Most of the essential functionality was implemented in queue.c, where it was necessary to make sure that each qentry pointed to the right place, whether it was the head of the queue, the tail of the queue, or simply another qentry. Resched required us to access the process table and work with the currpid global variable, and ready also worked with the process table and call a few queue.c methods. This project helped us to understand how different processes run concurrently. 
