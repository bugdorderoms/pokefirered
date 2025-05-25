from pathlib import Path
import re

NUM_SPACES_TO_TAB = 4
dir_path = Path(__file__).parent

EXCLUDE_PATHS = {
    # Directories
    dir_path / 'build',
    dir_path / 'graphics',
    dir_path / 'tools',
    # Files
    dir_path / 'asm' / 'macros' / 'map.inc',
    dir_path / 'constants' / 'gba_constants.inc',
}

def should_skip(file):
    if file in EXCLUDE_PATHS:
        return True

    return any(file.is_relative_to(exclude) for exclude in EXCLUDE_PATHS if exclude.is_dir())

def format_file(file_path, from_tabs_to_spaces):
    if not should_skip(file_path):
        with open(file_path, 'r', encoding='utf-8') as f:
            original_content = content = f.read()
        
        if from_tabs_to_spaces:
            new_content = re.sub(r'\t', ' ' * NUM_SPACES_TO_TAB, content)
        else:
            new_content = re.sub(rf'(?<! ) {{{NUM_SPACES_TO_TAB}}}(?! )', '\t', content)
        
        new_content = new_content.replace('\r\n', '\n').replace('\r', '\n')
        
        if new_content != original_content:
            with open(file_path, 'w', encoding='utf-8', newline='\n') as f:
                f.write(new_content)
            return True
    
    return False

# (tabs → spaces)
for file in dir_path.rglob('*.[hc]'):
    if format_file(file, from_tabs_to_spaces=True):
        print(f"Formatted: {file}")

# (spaces → tabs)
for file in dir_path.rglob('*.[is][nc]*'):
    if format_file(file, from_tabs_to_spaces=False):
        print(f"Formatted: {file}")

print("End!")
