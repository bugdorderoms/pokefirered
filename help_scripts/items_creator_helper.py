import os
import glob
from header import *

# This script generates the files gen_x.h on src/data/item/tms folder, based on the include/constants/tms.h file
# and pokeballs_graphics.h on the src/data/item folder based on the pokeball graphics in graphics/interface/ball folder
# and icons.h on the src/data/item folder based on the item graphics in graphics/items/icons
# and its palettes in graphics/items/icon_palettes folders

# Start of code

# Go to project's root
file_path = CheckPath(__file__)

# Create each gen_X file

donotmodifyheader = GetDontModifyHeader(__file__)

tms = []

def CreateGenFile(gen):
    output = open(f'{file_path}/src/data/item/tms/{gen.lower()}.h', 'w')
    output.write(f'// {donotmodifyheader}\n\n#ifdef __INTELLISENSE__\n'
                 f'const struct Item gTmsInfo_{FormatExtension(gen)}[] =\n'
                 '{\n#endif\n')
    output.writelines(tms)
    output.write(f'\n#ifdef __INTELLISENSE__\n'
                 '};\n#endif\n')
    output.close()
    tms.clear()

with open(f'{file_path}/include/constants/tms.h', 'r') as file:
    gen = 'none'

    for line in file:
        line = line.strip()

        if 'NUM_TECHNICAL_MACHINES' in line:
            if gen != 'none':
                CreateGenFile(gen)

        elif 'TMS_GEN' in line:
            gen = line.split(' ')[3]

        elif line.startswith('#define ITEM_'):
            tm = line.split(' ')[1].strip()
            num = tm.split('_')[1]
            move = tm.removeprefix(f'ITEM_{num}_')
            tms.append(f'\n\t[{tm}] ='
                       '\n\t{\n'
                       f'\t\t.name = _("{num}"),\n'
                       f'\t\t.holdEffectParam = MOVE_{move},\n\t\t.description = (u8*)&gBattleMoves[MOVE_{move}].description,\n'
                       f'\t\t.price = 3000,\n\t\t.pocket = POCKET_TM_CASE,\n\t\t.type = ITEM_TYPE_PARTY_MENU,\n'
                       f'\t\t.iconPic = sItemIcon_Tm,\n\t\t.iconPalette = NULL, // Handled by gTypesInfo'
                       '\n\t},\n')

# Create pokeballs_graphics.h

pokeballs = []

# Append ball open graphics
pokeballs.append(f'static const u32 sOpenPokeballGfx[] = INCBIN_U32("graphics/interface/ball_open.4bpp.lz");\n\n')

# Append all pokeballs graphics and palettes
for path in glob.glob(f'{file_path}/graphics/interface/ball/*'):
    if path.endswith('.png'):
        name = os.path.basename(path.removesuffix('.png'))
        pokeballs.insert(len(pokeballs) - 2, f'static const u32 sInterfaceGfx_{FormatExtension(name)}Ball[] = INCBIN_U32("graphics/interface/ball/{name}.4bpp.lz");\n')
    elif path.endswith('.gbapal'):
        name = os.path.basename(path.removesuffix('.gbapal'))
        pokeballs.append(f'static const u32 sInterfacePal_{FormatExtension(name)}Ball[] = INCBIN_U32("graphics/interface/ball/{name}.gbapal.lz");\n')
        pokeballs.append('\n')

pokeballs.pop()
file = open(f'{file_path}/src/data/item/pokeballs_graphics.h', 'w')
file.writelines(pokeballs)
file.close()

# Create icons.h

icons = []
palettes = []

# Append list end graphics
icons.append(f'const u32 gItemIcon_ReturnToFieldArrow[] = INCBIN_U32("graphics/interface/return_to_field_arrow.4bpp.lz");\n')
palettes.append(f'const u32 gItemIconPalette_ReturnToFieldArrow[] = INCBIN_U32("graphics/interface/return_to_field_arrow.gbapal.lz");\n')

dir = f'{file_path}/graphics/items'

# Append all icons
for path in glob.glob(f'{dir}/icons/*.png'):
    name = os.path.basename(path.removesuffix('.png'))
    icons.append(f'static const u32 sItemIcon_{FormatExtension(name)}[] = INCBIN_U32("graphics/items/icons/{name}.4bpp.lz");\n')

# Append all palettes
for path in glob.glob(f'{dir}/icon_palettes/*.pal'):
    name = os.path.basename(path.removesuffix('.pal'))
    palettes.append(f'static const u32 sItemIconPalette_{FormatExtension(name)}[] = INCBIN_U32("graphics/items/icon_palettes/{name}.gbapal.lz");\n')

# Write to icons.h
file = open(f'{file_path}/src/data/item/icons.h', 'w')
# Write icons
file.write(f'// {donotmodifyheader}\n\n// Icons\n')
file.writelines(icons)
# Write palettes
file.write('\n// Palettes\n')
file.writelines(palettes)
file.close()

print('End!')