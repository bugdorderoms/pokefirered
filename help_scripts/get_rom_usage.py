from pathlib import Path
from tkinter.messagebox import showinfo

# Max ROM size in bytes (32 MB)
max_size = 32 * 1024 * 1024

# Get the actual ROM size
rom_size = (Path(__file__).parent / 'pokefirered_rev1.gba').stat().st_size

# Calc %age used
percentage_used = (rom_size / max_size) * 100

showinfo('ROM info:', f'Size: {rom_size} B\nUsed size: {rom_size / (1024 * 1024):.2f} MB of 32 MB\n%age used: {percentage_used:.2f}%')
