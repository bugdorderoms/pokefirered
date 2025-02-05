from PIL import Image
from pathlib import Path

def get_coords_size(image, start_y=0, get_x=False):
    for y in range(64):
        for x in range(64):
            y_pixel = abs(start_y - y)
            coords = (y_pixel, x) if get_x else (x, y_pixel) # Loop vertically if get_x, otherwise loop horizontally

            if image.getpixel(coords) != 0: # Get first pixel with non-transparent color
                return y
            
    return 0 # Fail safe

def get_draw_boundary(image, get_x=False):
    second_pixels_start_pos = get_coords_size(image, start_y=64 - 1, get_x=get_x)
    return (get_coords_size(image, get_x=get_x) + second_pixels_start_pos, second_pixels_start_pos)

file_path = Path(__file__).parent
output = []

for file in file_path.glob('*.PNG'):
    file_name = file.stem

    if file_name in ['front', 'back']:
        img = Image.open(file)

        # Get pic coords size and y offset
        output.append(f'{file_name}:\n')
        ret = get_draw_boundary(img)
        output.append(f'.{file_name}PicSize = MON_COORDS_SIZE({64 - get_draw_boundary(img, get_x=True)[0]}, {64 - ret[0]}),\n')
        output.append(f'        .{file_name}PicYOffset = {ret[1]},\n\n')

with open(file_path / 'output.txt', 'w') as f:
    f.writelines(output)