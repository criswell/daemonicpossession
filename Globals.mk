# Global makefile settings

DEBUG=-g -D DAEMONIC_DEBUG

CC=gcc
CPP=g++

INSTALL=install

CFLAGS=-c -Wall -Wno-write-strings -Wno-parentheses
CP_FLAGS=-c -Wall -Wno-write-strings -Wno-parentheses

STATIC_BASE=-static
STATIC_CC=-static-libgcc
STATIC_CPP=-static-libstdc++
