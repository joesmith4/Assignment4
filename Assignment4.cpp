// Copyright 2018 Joseph Smith, Alex Martin
#include <stdio.h>
#include <stdint.h>
#include <thread>
#include <mutex>

std::mutex mtx;


/*
  Function..: CANDecode
  Description..: takes in a message as an unsigned 8 bit integer array, decodes it, and returns the decoded integer
  Parameters..: unsigned 8 bit integer array - the message to decode
*/
int CANDecode(uint8_t message[]) {
// lock the mutex to prevent race conditions
mtx.lock();
// mask the first two bits to properly map to the CAN diagram
message[0] = message[0] & 0x3f;
uint16_t longMessage = message[0];
// preparing to add the second byte for the CAN diagram mapping
longMessage = longMessage << 8;
// adding the second byte to the message
longMessage += message[1];
// decoding the message by bit shifting to the right 5 bits
longMessage = longMessage >> 5;
// printing out the decoded message
printf("Decoded CAN Signal:  %d \n", longMessage);

// unlock the mutex so other threads can utilize it
mtx.unlock();
// casts the decoded message as an int and returns it
return static_cast<int>(longMessage);
}

void CANUpdate(uint8_t message[]) {
// lock the mutex to prevent race conditions
mtx.lock();
// adding 10 to update the message because Priyash said to
message[1] = message[1] +10;
// unlocking the mutex so other threads can utilize it
mtx.unlock();
}

int main() {
// declaring the CAN data array
uint8_t CAN_data[5] = {0x01, 0x40, 0x00, 0x00, 0x00};

// declaring the two threads to decode and update the data
// parameters are function name, and function parameter
std::thread th_CANDecode(CANDecode, CAN_data);
std::thread th_CANUpdate(CANUpdate, CAN_data);

// brings the threads back together before moving onto the next step
th_CANDecode.join();
th_CANUpdate.join();

return(0);
}


