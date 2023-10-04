#
# file: arduino.py
# date: 2023-05-25
# author: Robert Morley
#
# Holds functions for updating Arduino library implementations.
#
# version: 1.0.0.0
# 
#
import os
import platform
import zipfile
import shutil

# Gets the name of the library from the .properties file.
def get_library_name(root_dir, replace_ws = True):
  lib_prop_path = os.path.join(root_dir, 'library.properties')
  name = " "

  if os.path.exists(lib_prop_path):         # Make sure file exists
    lib_prop = open(lib_prop_path, "r")     # Open in read mode.

    for line in lib_prop:                   # Scan file for "name" 
      line = line.strip()
      key_pair = line.split('=')            # Split lines by '='
      
      if key_pair[0] == "name":
        name = key_pair[1]

        if replace_ws:                      # Replace spaces with '_'
          name = name.replace(" ", "_")

    lib_prop.close()  
  else:  
    print(f"Can't find library.properties in {root_dir}")

  return name
# Finds where Arduino library directory is located.
def find_library_dir():
  os_type = platform.system() # Get OS name.

  if os_type == 'Windows':
    # Get User path
    user_dir = user_path = os.path.expanduser('~') 
    
    # Possible Arduino library directories
    lib_paths = [R"Documents\Arduino\libraries", R"OneDrive - University to Toronto\Documents\Arduino\libraries", R"OneDrive\Documents\Arduino\libraries"]
  
    # C:\Users\rmorl\OneDrive\Documents\Arduino\libraries
    # Search for Arduino libraries directory location.
    for alib_dir in lib_paths:
      alib = os.path.join(user_dir, alib_dir)

      if os.path.exists(alib + '/'):
        # print("Directory found.")
        return alib
  else:
    print(f"OS not supported : {os_type}")

  return " "
# Get file list for Arduino Library.
def get_library_files(root_dir):
  file_list = []

  for dir_path, dir_names, file_names in os.walk(root_dir):                             # this loop though directies recursively 
    # print(list(exclude_ext))
    files = [os.path.join(dir_path, f) for f in file_names if not f.endswith(('.zip','.md'))]

    if files:                                                                           # Only add lists that are not empty
      file_list.extend(files)                                                           # Add files to list.

  return file_list                                                                      # Return library file list.
# Create .zip file for Arduino library.
def zip_library(root_dir, zip_name):
  name = get_library_name(root_dir, False)
  print(f"\tCreating .zip file for {name} library ...")
  # Create Zip file path
  zip_path = os.path.join(root_dir, zip_name)

  # erase zip file if it already exists
  if os.path.exists(zip_path) :
    os.remove(zip_path)

  # Open zip file.
  zip_lib = zipfile.ZipFile(zip_path, 'w', compression=zipfile.ZIP_DEFLATED)
  
  # Get library file list for Arduino list.
  zip_list = get_library_files(root_dir)

  for zf in zip_list:   # Add files to zip files.
    print(f"\t\tArchieving: {zf}")
    zip_lib.write(zf)

  zip_lib.close()     # clost zip file.
  print(f"\t{zip_name} created.")
# Updates local Arduino library directory during development.
def update_library(root_dir, lib_name):
  print(f"\tUpdating {lib_name.replace('_',' ')} Arduino IDE Library")
  # Get Arduino libraries directory path
  libraries_root = find_library_dir()

  if libraries_root == " ":
    print("\tArduino libraries directory not found.")
    return

  dest_root = os.path.join(libraries_root, lib_name)
  
  print(f"\t\tArduino Library Path: {dest_root}")

  if os.path.exists(dest_root + "\\"):

    lib_files = [f for f in os.listdir(root_dir) if not f.endswith(('.zip','.md'))]
    
    for item in lib_files:
      src = os.path.join(root_dir, item)
      dest = os.path.join(dest_root, item)

      if os.path.isdir(src):
        shutil.copytree(src, dest, copy_function=shutil.copy2, dirs_exist_ok=True) # Over existing directory.
      else:
        shutil.copy2(src, dest)

      print(f"\t\tUpdated: {dest}")

    print("\tLibrary update complete.")
  else:
    print(f"\t{dest_root} library is not installed")
