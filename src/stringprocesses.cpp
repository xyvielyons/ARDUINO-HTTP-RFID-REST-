#include "stringprocesses.h" 
#include <Arduino.h>

String StringProcesses::removeFirstSpace(String str) {
  int firstSpaceIndex = str.indexOf(' ');
  if (firstSpaceIndex >= 0) {
    str = str.substring(0, firstSpaceIndex) + str.substring(firstSpaceIndex + 1);
  }
  return str;
}
