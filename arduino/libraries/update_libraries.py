# file: update_libraries.py
# date: 2023-05-20
# author: Robert Morley
#
# Automatically updates the project libraries zip files.
#
import os
import zipfile

def get_lib(dir_path, zfile):    
    for dir in os.listdir(dir_path + '/'):
        path_str = dir_path + '/' + dir
        # check if path is directory
        if os.path.isdir(path_str):
            get_lib(path_str, zfile)
        else:
            # get file extension
            file_ext = os.path.splitext(path_str)
            # do not include .md or .zip files
            if file_ext[1] != '.md':
                if file_ext[1] != '.zip':
                    zfile.write(path_str)



def update_lib(lib_dir, zfile):
    print("\tUpdating " + lib_dir + " library " + zfile + " file.")

    file_name = lib_dir + '/' + zfile

    if os.path.exists(file_name):
        os.remove(file_name)

    zlib = zipfile.ZipFile(file_name, 'w', compression=zipfile.ZIP_DEFLATED)

    get_lib(lib_dir, zlib)

    zlib.close()
    print("\tUpdate complete.")
   
# def open_py(name):
#     call(["python", name])

def main():
    print("Starting update.")
    update_lib('./max11635_adc', 'max11635_adc.zip')
    update_lib('./phyduino_pico', 'phyduino_pico.zip')
    update_lib('./Analog_Front_End_Arduino_Library','Analog_Front_End_Arduino_Library.zip')
    print("Update Finished.") 
    

if __name__ == "__main__":
    main()