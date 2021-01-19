"""Create .gradient files from gradient images.
Made for mandelbrot_c
Ahmet Nur YILDIRIM
18.01.2021"""


import sys
from PIL import Image
import os.path


def rgb_to_hex(r: int, g: int, b: int) -> str:
    hex_string = ""
    for i in (r, g, b):
        hex_string += str(hex(i))[2:] if i >= 16 else "0" + str(hex(i))[2:]
    return hex_string


def get_hex_values_of_gradient_image(filename: str, colorcount: int) -> list:
    color_list = []
    with Image.open(filename) as im:
        print(filename, im.format, f"{im.size}x{im.mode}")
        increment = int(im.size[0] / colorcount)
        for i in range(colorcount):
            r, g, b, a = im.getpixel((i * increment, int(im.size[1] / 2)))
            color_list.append(rgb_to_hex(r, g, b))
    return color_list


def main():
    for infile in sys.argv[1:]:
        try:
            gradient_file = os.path.splitext(infile)[0] + ".gradient"
            with open(gradient_file, "w") as _gfile:
                _gfile.write("\n".join(get_hex_values_of_gradient_image(infile, 256)))
        except OSError:
            pass


if __name__ == '__main__':
    main()
