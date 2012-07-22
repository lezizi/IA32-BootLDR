A Framework (IA32)
================

A Framework (AF) for IA32 processors.

## What is A Framework?

AF is usually regraded as the runtime environment of APL. However, AF does much more than that atctually.

## What is AF-IA32?

Unlike AF for windows, AF-IA32 is a standalone minimum kernel which can run on IA32 processors without support of any OS.

## What is included in the latest TP1 (technology preview)?

We have realized the basic action needed for AF to communicate with BIOS, video graphic card. It might be the only part of AF-IA32 that is written in C rather than APL. And we have created a notepad application, although it hasn't been an Action Oriented app yet, to demonstrate that Video Buffer and Interrupt Handler is functioning well.

## What is going to take place in TP2 and other TPs to come?

In TP2, we will bring hard disk feature into AF. And in the near future, we will rewritten Video Buffer in APL.