# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/manue/esp/v5.2.2/esp-idf/components/bootloader/subproject"
  "D:/2-Universidad/1-TFG/5-Dev/2-EVT/sdspi/build/bootloader"
  "D:/2-Universidad/1-TFG/5-Dev/2-EVT/sdspi/build/bootloader-prefix"
  "D:/2-Universidad/1-TFG/5-Dev/2-EVT/sdspi/build/bootloader-prefix/tmp"
  "D:/2-Universidad/1-TFG/5-Dev/2-EVT/sdspi/build/bootloader-prefix/src/bootloader-stamp"
  "D:/2-Universidad/1-TFG/5-Dev/2-EVT/sdspi/build/bootloader-prefix/src"
  "D:/2-Universidad/1-TFG/5-Dev/2-EVT/sdspi/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/2-Universidad/1-TFG/5-Dev/2-EVT/sdspi/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/2-Universidad/1-TFG/5-Dev/2-EVT/sdspi/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
