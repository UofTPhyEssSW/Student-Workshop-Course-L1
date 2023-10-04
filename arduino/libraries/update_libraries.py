# file: update_libraries.py
# date: 2023-05-20
# author: Robert Morley
#
# Automatically updates the project Arduino library zip files 
# and any locally installed Arduino Libraries.
#
#
import arduino

def main():
    user_libs = ['./max11635_adc', './phyduino_pico', './Analog_Front_End_Arduino_Library']

    print("Starting update.")
    print("Compressing Arduino Libraries ...\n")

    for lib in user_libs:
        lib_zip = (lib.replace("./", "") + ".zip").lower()
        arduino.zip_library(lib, lib_zip)
        print(" ")

    print("Arduino Library Compression complete.\n")
    print("Updating local Arduino Libraries ...\n")

    for lib in user_libs:
        lib_name = arduino.get_library_name(lib)
        arduino.update_library(lib, lib_name) 
        print(" ")
    print("Local Arduino Library updates complete.") 
    print(" ")

if __name__ == "__main__":
    main()
