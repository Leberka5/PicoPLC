#!/bin/bash

# only execute like this:
#   ./scripts/flash.sh
#
# for a permissions error regarding
# /dev/ttyACM0: change using chmod

sendRebootSignal() {
  echo "================= [Sending] ==================="
  stty -F /dev/ttyACM0 115200 raw -echo
  echo -e "reboot\n" > /dev/ttyACM0
  if [ ! $? -eq 0 ]; then
      echo "ERROR: sending reboot signal failed!"
      #exit 1
  else
    echo "sent reboot signal"
  fi
}

waitForMount() {
  echo "================= [Waiting] ==================="
  echo "wait for RPI-RP2 drive ..."
  PICO_PATH="/run/media/$(whoami)/RPI-RP2"
  COUNTER=0
  while [ ! -d "$PICO_PATH" ]; do
      sleep 0.5
      COUNTER=$((COUNTER+1))
      if [ $COUNTER -ge 20 ]; then
        echo "ERROR: Pico drive not mounted!"
        #exit 2
      fi
  done
  echo "Pico drive found"
}

compileSrcCode() {
  echo "================= [Compiling] ================="
  ~/.platformio/penv/bin/pio run -e pico > /dev/null 2>&1
  if [ ! $? -eq 0 ]; then
      echo "ERROR: compiling failed!"
      #exit 3
  else
    echo "compiling finished"
  fi
}

copyFirmware() {
  echo "================= [Copying] ==================="
  cp ./.pio/build/pico/firmware.uf2 "$PICO_PATH/firmware.uf2"
  if [ ! $? -eq 0 ]; then
    echo "ERROR: copying failed!"
    #exit 4
  else
    echo "firmware copied successfully"
  fi
}


sendRebootSignal
waitForMount
compileSrcCode
copyFirmware