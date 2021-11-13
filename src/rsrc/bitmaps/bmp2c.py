#
# Convert an image for the C256
#

from PIL import Image
from optparse import OptionParser

def emit(output, count, pixel_count, last_pixel):
    if count % 16 == 0:
        if count == 0:
            output.write("\t0x{0:02X}, 0x{1:02X}".format(pixel_count, last_pixel))
        else:
            output.write(",\n\t0x{0:02X}, 0x{1:02X}".format(pixel_count, last_pixel))
    else:
        output.write(", 0x{0:02X}, 0x{1:02X}".format(pixel_count, last_pixel))

parser = OptionParser()
parser.add_option("-i", "--input", dest="input", help="Source image file")
parser.add_option("-o", "--output", dest="output", default="image.h", help="Destination for image and color data.")

(options, args) = parser.parse_args()

if options.input:
    with open(options.output, "w") as output_file:
        with Image.open(options.input) as im:
            output_file.write("unsigned char splashscreen_lut[] = {\n")
            palette = im.getpalette()
            while palette:
                r = palette.pop(0)
                g = palette.pop(0)
                b = palette.pop(0)
                output_file.write("\t0x{0:02X}, 0x{1:02X}, 0x{2:02X}, 0x00,\n".format(b, g, r))
            output_file.write("}\n\n")

            last_pixel = -1
            pixel_count = 0
            count = 0

            output_file.write("unsigned char splashscreen_pix[] = {\n")
            (w, h) = im.size
            for v in range(0, h):
                for u in range(0, w):
                    pixel = im.getpixel((u, v))

                    if last_pixel != pixel:
                        if last_pixel != -1:
                            emit(output_file, count, pixel_count, last_pixel)
                            count = count + 2
                        pixel_count = 1
                        last_pixel = pixel

                    else:
                        pixel_count = pixel_count + 1
                        if pixel_count == 256:
                            emit(output_file, count, pixel_count-1, last_pixel)
                            count = count + 2
                            pixel_count = 1
                            last_pixel = pixel

            emit(output_file, count, pixel_count, last_pixel)
            output_file.write(", 0x00, 0x00\n}\n")
else:
    parser.print_help()
