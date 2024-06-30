# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "E:/Espressif/frameworks/esp-idf-v5.1.2/components/bootloader/subproject"
  "E:/Espressif/frameworks/esp-idf-v5.1.2/workspace/sample_project/build/bootloader"
  "E:/Espressif/frameworks/esp-idf-v5.1.2/workspace/sample_project/build/bootloader-prefix"
  "E:/Espressif/frameworks/esp-idf-v5.1.2/workspace/sample_project/build/bootloader-prefix/tmp"
  "E:/Espressif/frameworks/esp-idf-v5.1.2/workspace/sample_project/build/bootloader-prefix/src/bootloader-stamp"
  "E:/Espressif/frameworks/esp-idf-v5.1.2/workspace/sample_project/build/bootloader-prefix/src"
  "E:/Espressif/frameworks/esp-idf-v5.1.2/workspace/sample_project/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "E:/Espressif/frameworks/esp-idf-v5.1.2/workspace/sample_project/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "E:/Espressif/frameworks/esp-idf-v5.1.2/workspace/sample_project/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
