#!/bin/bash

PORT="/dev/ttyACM0"
BAUD="115200"
MCU="atmega2560"
PROGRAMMER="wiring"
HEXDIR="avr_bin"

if [ $# -ne 1 ]; then
  echo "Use: $0 <filename>/<filename.hex>"
  exit 1
fi

FILE="$1"

if [[ "$FILE" == *.hex ]]; then
  FILE="${FILE%.hex}"
fi

HEXFILE="$HEXDIR/$FILE.hex"

if [ ! -f "$HEXFILE" ]; then
  echo "Error: file $HEXFILE not found"
  exit 2
fi

echo "Flashing $HEXFILE on $MCU via $PORT..."

avrdude -v -p"$MCU" -c"$PROGRAMMER" -P"$PORT" -b"$BAUD" -D -C /etc/avrdude.conf -Uflash:w:"$HEXFILE":i
