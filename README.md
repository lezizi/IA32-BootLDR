A Framework (IA32)
================

A Framework (AF) for IA32 processors.

## What is A Framework?

A Framework is a pack of Basic Actions and Strategies needed in an Action Oriented application. To put it simply, it does what the runtime environment of a generic programming language does.

## What is AF-IA32?

Unlike A Framework for Windows or other operating systems, AF-IA32 is a standalone kernel which connects your application with IA32 processors, and PC hardware.

## What is included in the latest TP3 (technology preview III)?

We have implemented the basic Actions needed to communicate with BIOS, video graphic card and ATA hard disk drive. It might be the only part of AF-IA32 that is written in C rather than APL. And we have created a notepad application, although it hasn't been an Action Oriented application yet, to demonstrate that all the C code written is functioning well.
In TP3, Paging is enabled.

## What is going to take place in TP4 and other TPs to come?

TP3 may come with a HDD boot-up support. And in the near future, we will add a code loader.
