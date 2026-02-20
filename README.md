# UART CLI (ATmega328P)

Interrupt-driven UART Command Line Interface using a circular RX buffer.

## Overview

This project implements a non-blocking UART CLI for the ATmega328P running at 16MHz.

The UART uses an interrupt-driven circular buffer to safely handle incoming data without blocking the main loop.

## Features

- Interrupt-driven UART RX
- Circular buffer (ring buffer) implementation
- Non-blocking main loop
- LED control via CLI
- Command parsing using string comparison

## Commands

led on      - Turn LED on (PB5 / Pin 13)  
led off     - Turn LED off  
status      - Print LED state  

## Architecture

UART RX Interrupt  
        ↓  
Circular Buffer (head/tail)  
        ↓  
Main Loop reads stream  
        ↓  
CLI Parser  
        ↓  
Application Logic  

## Hardware

- ATmega328P
- 16 MHz clock
- 9600 baud rate
- LED connected to PB5 (Arduino Pin 13)

## Key Embedded Concepts Demonstrated

- Ring buffer design
- Interrupt-safe data handling
- Stream-based command parsing
- Memory-aware coding
- Modular firmware architecture ready

## Future Improvements

- Add numeric parameter parsing (e.g., blink 500)
- Add overflow counter command
- Split into multiple source files
