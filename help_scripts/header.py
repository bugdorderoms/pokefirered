import os

def CheckPath(file):
    file_path = os.path.dirname(os.path.realpath(file))
    requiredPath = '\\help_scripts'

    if not file_path.endswith(requiredPath):
        print(f'Please run this script from the {requiredPath} folder')
        quit()
    else:
        file_path = file_path.removesuffix(requiredPath)

    return file_path

# Function for capitalization and format of the text
def FormatExtension(name, separator='_'):
    extensions = name.replace(separator, '_').split('_')
    extension = ''
    
    for ex in extensions:
        extension += ex.capitalize()

    return extension

def GetDontModifyHeader(file):
    return f'DO NOT MODIFY THIS FILE. IT IS AUTO GENERATED FROM: {os.path.basename(file)}'
