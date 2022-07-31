#!/bin/sh
echo ""
echo "Sneptest SMS Build Script"
echo "-------------------------"

CC="${HOME}/Code/sdcc-4.1.0/bin/sdcc"
devkitSMS="${HOME}/Code/devkitSMS"
SMSlib="${devkitSMS}/SMSlib"
ihx2sms="${devkitSMS}/ihx2sms/Linux/ihx2sms"

CFLAGS="--std-c11 -mz80 --peep-file ${devkitSMS}/SMSlib/src/peep-rules.txt -I ${SMSlib}/src"

rm -r work
mkdir -p work

build_sneptest ()
{
    echo "Compiling..."
    eval $CC $CFLAGS -c source/main.c -o work/main.rel || exit 1
    eval $CC $CFLAGS -c source/input_tests.c -o work/input_tests.rel || exit 1

    echo "Linking..."
    eval $CC -o work/sneptest.ihx -mz80 --no-std-crt0 --data-loc 0xC000 ${devkitSMS}/crt0/crt0_sms.rel work/*.rel ${SMSlib}/SMSlib.lib || exit 1

    echo "Generating ROM..."
    eval $ihx2sms work/sneptest.ihx sneptest.sms || exit 1

    echo "Done"
}

build_sneptest


