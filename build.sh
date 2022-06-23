#!/bin/sh
echo ""
echo "Sneptest SMS Build Script"
echo "-------------------------"

sdcc="${HOME}/Code/sdcc-4.1.0/bin/sdcc"
devkitSMS="${HOME}/Code/devkitSMS"
SMSlib="${devkitSMS}/SMSlib"
ihx2sms="${devkitSMS}/ihx2sms/Linux/ihx2sms"

rm -r work
mkdir -p work

echo ""
echo "Compiling..."
for file in main
do
    echo " -> ${file}.c"
    ${sdcc} -c -mz80 --peep-file ${devkitSMS}/SMSlib/src/peep-rules.txt -I ${SMSlib}/src \
        -o "work/${file}.rel" "source/${file}.c" || exit 1
done

echo "Linking..."
${sdcc} -o work/sneptest.ihx -mz80 --no-std-crt0 --data-loc 0xC000 ${devkitSMS}/crt0/crt0_sms.rel work/*.rel ${SMSlib}/SMSlib.lib || exit 1

echo ""
echo "Generating ROM..."
${ihx2sms} work/sneptest.ihx sneptest.sms || exit 1

echo ""
echo "Done"
