# file: check_libraries.py
# date: 2023-05-20
# author: Robert Morley
#
# Automatically updates the project Arduino library zip files 
# and any locally installed Arduino Libraries.
#
#
import arduino
import os
import filecmp

def print_list(list):
    for item in list:
        print(f"\t{item}")

def compare_lib_dirs(src):
    arduino_root = arduino.find_library_dir()

    l_name = arduino.get_library_name(src)
    dest = os.path.join(arduino_root, l_name)
    
    src_list = arduino.get_library_files(src)
    
    cmp_error = False
    filecmp.clear_cache()               # Clear cache to look for new changes.

    for src_item in src_list:
        rel_path = src_item.replace(src + "\\",'')
     
        dest_item = os.path.join(dest, rel_path)
     
        if os.path.exists(dest_item):
            file_ok = filecmp.cmp(src_item, dest_item, shallow=False)

            if not file_ok:
                print(f"\tChecking file : {rel_path} [MISMATCH]")
                cmp_error = True
            # else:
            #     print(f"\tChecking file : {rel_path} [OK]")
        else:
            print(f"\tChecking file : {rel_path} [DOES NOT EXIST in Arduino Library]")
            cmp_error = True

    return cmp_error    


def main():
    user_libs = ['./max11635_adc', './phyduino_pico', './Analog_Front_End_Arduino_Library']

    for src in user_libs:
        name = arduino.get_library_name(src, replace_ws=False) 

        print(f"Checking \"{name}\" Arduino library ...")
        cmp_err = compare_lib_dirs(src)    

        if cmp_err:
            print(f"\"{name}\" Arduino library does not match source directory.")
        else :
            print(f"\"{name}\" Arduino library up to date.")

        print()

if __name__ == "__main__":
    main()
