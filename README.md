# Sensor-detect
  Detect your cpu's temperature and warning you when overheated. This project is under linux and gtk3. If the version of your gtk is more than 3.14, you will get some warnings about GtkStutusIcon. Because it has been deprecated since version 3.14. But you can ignore these warning.

# Building
1. Modify the macro "WARNING_TEMPERATURE" with a number which is an overheated flag in the file "main.c"
2. chmod +x ./make.sh
3. ./make.sh
4. ./main

# Usage
1. Move mouse to the icon, you will get the current temperature of your cpu.
2. When the current temperature number is bigger than the "WARNING_TEMPERATURE" you set, a warning bell will ring.
3. Right click the tray icon to quit this program.
