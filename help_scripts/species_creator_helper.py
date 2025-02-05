import glob
import os
from header import *

# This script generates the files pokedex.h and cry_ids.h on the include/constants folder,
# pics_and_icons.h and footprints.h on the src/data/pokemon/graphics folder,
# and cry_tables.inc and cries_sound_data.inc on the sound folder
# all these are generated based on the include/constants/species.h file

# Start of code

# Go to project's root
file_path = CheckPath(__file__)

# Create Pokedex.h, cry_ids.h, cry_tables.inc and cries_sound_data.inc

# List here cries that are uncompressed
uncomppressed_cries = [
    "Klefki"
]

pokedex = []
cries = []
cry_table_normal = []
cry_table_reverse = []
cry_sound_data = []

# Append a new cry to cry_tables.inc and cries_sound_data.inc
def AppendToCryFiles(name, num):
    # Cry none is't present on the tables
    if name != 'NONE':
        cries.append(f'#define CRY_ID_{name} {str(num - 1)}\n')

        name = name.lower()
        formatedName = FormatExtension(name)

        if formatedName in uncomppressed_cries:
            uncomp = '_uncomp'
        else:
            uncomp = ''

        if not os.path.exists(f'{file_path}/sound/direct_sound_samples/cries/{name}{uncomp}.aif'):
            comment = True # Comment this cry if the sound doesn't exists in the cries folder
        else:
            comment = False

        # Write to cries_sound_data.inc
        text = '\n'

        if comment:
            text += '/*\n'

        text += f'\t.align 2\nCry_{formatedName}::\n\t.incbin "sound/direct_sound_samples/cries/{name}{uncomp}.bin"\n'

        if comment:
            text += '*/\n'

        cry_sound_data.append(text)

        # Write to cry_tables.inc
        text = f'Cry_{formatedName}'

        if comment:
            # Put cry as the Unown cry and add the correct cry commented on the right, so the other cries don't get reordered
            text = 'Cry_Unown @ ' + text
            uncomp = '' # Since the cry will be replaced by the Unown cry, it can't have the _uncomp prefix

        cry_table_normal.append(f'\tcry{uncomp} {text}\n')
        cry_table_reverse.append(f'\tcry_reverse{uncomp} {text}\n')

# Append the defines of the first and the last species of a region
def AppendRegionDefinesToPokedex(currentRegion, firstDexSpecies, lastDexSpecies):
    if firstDexSpecies != 'NONE':
        pokedex.append(f'\n#define DEX_START_{currentRegion} NATIONAL_DEX_{firstDexSpecies}\n'
                       f'#define DEX_END_{currentRegion} NATIONAL_DEX_{lastDexSpecies}\n')

with open(f'{file_path}/include/constants/species.h', 'r') as speciesFile:
    num = 0
    forms = False
    lastSpecies = 'NONE'
    firstDexSpecies = 'NONE'
    currentRegion = 'none'

    for line in speciesFile:
        line = line.strip()

        # Start of species forms. Ends pokedex.h edition, start adding cry of species's forms
        if line.find('#define FORMS_START') == 0:
            AppendRegionDefinesToPokedex(currentRegion, firstDexSpecies, lastSpecies)
            pokedex.append(f'\n#define NATIONAL_DEX_END NATIONAL_DEX_{lastSpecies}\n')
            cries.append('\n// Forms cry ids\n')
            cry_table_normal.append('\n\t@ Forms cry start\n')
            cry_table_reverse.append('\n\t@ Forms cry start\n')
            forms = True

        if line.startswith('// Region: '): # Start of region, copy string to output files
            AppendRegionDefinesToPokedex(currentRegion, firstDexSpecies, lastSpecies)
            firstDexSpecies = 'NONE'
            currentRegion = line.removeprefix('// Region: ').upper() # Get region's name
            pokedex.append(f'\n{line}\n')
            cries.append(f'\n{line}\n')

        elif line.find('#define SPECIES_') == 0:
            # Isolate species's name, removing prefix and suffix
            species = line.removeprefix('#define SPECIES_').split()[0]

            if forms:
                # Check if form has an alternate cry by checking if it has: // Form cry. At end of its define
                if line.endswith('// Form cry'):
                    AppendToCryFiles(species, num)
                    num += 1

            else: # Base species defines
                pokedex.append(f'#define NATIONAL_DEX_{species} {str(num)}\n')
                AppendToCryFiles(species, num)
                lastSpecies = species

                if firstDexSpecies == 'NONE' and species != 'NONE':
                    firstDexSpecies = species

                num += 1

donotmodifytext = GetDontModifyHeader(__file__)

# Write to pokedex.h
pokedex_file = open(f'{file_path}/include/constants/pokedex.h', 'w')
pokedex_file.write(f'#ifndef GUARD_CONSTANTS_DEX_H\n#define GUARD_CONSTANTS_DEX_H\n\n// {donotmodifytext}\n')
pokedex_file.writelines(pokedex)
pokedex_file.write('\n#endif // GUARD_CONSTANTS_DEX_H\n')
pokedex_file.close()
# Write to cry_ids.h
cries_file = open(f'{file_path}/include/constants/cry_ids.h', 'w')
cries_file.write(f'#ifndef GUARD_CONSTANTS_CRY_IDS_H\n#define GUARD_CONSTANTS_CRY_IDS_H\n\n// {donotmodifytext}\n')
cries_file.writelines(cries)
cries_file.write('\n#endif // GUARD_CONSTANTS_CRY_IDS_H\n')
cries_file.close()
# Write to cry_tables.inc
cry_tables_file = open(f'{file_path}/sound/cry_tables.inc', 'w')
cry_tables_file.write(f'@ {donotmodifytext}\n\n\t.align 2\ngCryTable::\n')
cry_tables_file.writelines(cry_table_normal)
cry_tables_file.write('\n\t.align 2\ngCryTableReverse::\n')
cry_tables_file.writelines(cry_table_reverse)
cry_tables_file.close()
# Write to cries_sound_data.inc
cries_data_file = open(f'{file_path}/sound/cries_sound_data.inc', 'w')
cries_data_file.write(f'/* {donotmodifytext} */\n')
cries_data_file.writelines(cry_sound_data)
cries_data_file.close()

# Create footprints.h and pics_and_icons.h
frontpics = []
backpics = []
palettes = []
shiny_palettes = []
icons = []
footprints = []

# Append blank footprint image
footprints.append(f'static const u8 sMonFootprint_None[] = INCBIN_U8("graphics/pokedex/blank_footprint.1bpp");\n')

dir = f'{file_path}/graphics/pokemon'

# Append to its respective files all sprites and palettes found
for path in glob.glob(dir + '/**', recursive=True):
    extension = path.removeprefix(dir + '\\').replace('\\', '/')
    formatedExtension = FormatExtension(extension, '/')

    # Append front pic
    if os.path.exists(f'{path}\\front.png'):
        frontpics.append(f'static const u32 sMonFrontPic_{formatedExtension}[] = INCBIN_U32("graphics/pokemon/{extension}/front.4bpp.lz");\n')
    
    # Append palette
    if os.path.exists(f'{path}\\normal.pal'):
        palettes.append(f'static const u32 sMonPalette_{formatedExtension}[] = INCBIN_U32("graphics/pokemon/{extension}/normal.gbapal.lz");\n')

    # Append back pic
    if os.path.exists(f'{path}\\back.png'):
        backpics.append(f'static const u32 sMonBackPic_{formatedExtension}[] = INCBIN_U32("graphics/pokemon/{extension}/back.4bpp.lz");\n')
    
    # Append shiny palette
    if os.path.exists(f'{path}\\shiny.pal'):
        shiny_palettes.append(f'static const u32 sMonShinyPalette_{formatedExtension}[] = INCBIN_U32("graphics/pokemon/{extension}/shiny.gbapal.lz");\n')

    # Append icon
    if os.path.exists(f'{path}\\icon.png'):
        icons.append(f'static const u8 sMonIcon_{formatedExtension}[] = INCBIN_U8("graphics/pokemon/{extension}/icon.4bpp");\n')

    # Append footprint
    if os.path.exists(f'{path}\\footprint.png'):
        footprints.append(f'static const u8 sMonFootprint_{formatedExtension}[] = INCBIN_U8("graphics/pokemon/{extension}/footprint.1bpp");\n')

# Write to pics_and_icons.h
output = open(f'{file_path}/src/data/pokemon/graphics/pics_and_icons.h', 'w')
# Write front pics
output.write(f'// {donotmodifytext}\n\n// Front pics\n')
output.writelines(frontpics)
# Write back pics
output.write('\n// Back pics\n')
output.writelines(backpics)
# Write normal palettes
output.write('\n// Palettes\n')
output.writelines(palettes)
# Write shiny palettes
output.write('\n// Shiny palettes\n')
output.writelines(shiny_palettes)
# Write icons
output.write('\n// Icons\n')
output.writelines(icons)
output.close()

# Write to footprints.h
output = open(f'{file_path}/src/data/pokemon/graphics/footprints.h', 'w')
output.write(f'// {donotmodifytext}\n\n')
output.writelines(footprints)
output.close()

print('End!')