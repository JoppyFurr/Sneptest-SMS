#!/bin/sh

SDCC=/home/joppy/Code/sdcc3.6/bin/sdcc
IHX2SMS=/home/joppy/Code/sdcc3.6/bin/ihx2sms
DKSMS=/home/joppy/Code/devkitSMS/

echo "Compiling..."
${SDCC} -c -mz80 --peep-file ${DKSMS}/SMSlib/src/peep-rules.txt main.c

echo "Linking..."
${SDCC} -o sneptest.ihx -mz80 --no-std-crt0 --data-loc 0xC000 ${DKSMS}/crt0/crt0_sms.rel main.rel SMSlib.lib

echo "Generating SMS ROM..."
${IHX2SMS} sneptest.ihx sneptest.sms
