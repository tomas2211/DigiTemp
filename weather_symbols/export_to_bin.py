import imageio
import numpy as np
import matplotlib.pyplot as plt
import cv2
import cairosvg
from PIL import Image
import io
import glob
import os


def load_svg(fn, h, w):
    img = Image.open(
        io.BytesIO(cairosvg.svg2png(url=fn, output_width=w, output_height=h))
    )
    img_np = np.asanyarray(img).astype(np.float32) / 255.0
    return img_np


def encode_img(img_bin, var_name, bits=4, debug=False):
    assert bits in [2, 4, 8]

    img_lin = img_bin.reshape(-1).astype(np.bool)
    # zrs = np.zeros(-img_lin.shape[0] % 8, dtype=np.bool) # ceil the len to multiple of 8
    # img_lin = np.hstack((img_lin, zrs))
    #
    # img_p = img_lin.reshape(-1,8).astype(np.uint8)
    #
    # for i in range(8):
    #     img_p[:, i] <<= 7 - i
    #
    # img_bytes = np.sum(img_p, axis=1)

    out = []
    ind = 0
    act_col = False
    while ind < img_lin.shape[0]:
        sel = img_lin[ind: (ind + bits ** 2)]

        changes = np.nonzero(sel != act_col)[0]
        if len(changes) == 0:
            out.append(bits ** 2 - 1)
            ind += bits ** 2 - 1
        else:
            chg = changes[0]
            if chg == 0:
                out.append(0)
                ind += 1
                act_col = not act_col
            else:
                out.append(chg)
                ind += chg

    if debug:
        print('Prev size (B):', len(img_lin) / 8)
        print('Compressed to (B): ', (len(out) * bits) / 8)

    coded = []
    pushed = 0
    cur = 0
    for nib in out:
        cur |= nib
        pushed += bits

        if pushed == 8:
            coded.append(cur)
            cur = 0
            pushed = 0

        cur <<= bits

    if pushed != 0:
        coded.append(cur)

    str_out = 'const uint8_t %s[] PROGMEM = {\n\t' % var_name
    for c in coded:
        str_out += '%#04x, ' % c

    str_out = str_out[:-2] + '\n};'
    print(str_out)


PATH_IN = 'flaticon_icons/icons/svg/*.svg'
H,W = (50,50)
fn_list = glob.glob(PATH_IN)

for fn in fn_list:
    # Generate binary images
    img = load_svg(fn, H, W)
    # img_bw = np.sum(img[:, :, :3], axis=2) / 3
    img_bin = (img[:, :, 3] < 0.5).astype(np.uint8) * 255

    folder_in, basename = os.path.split(fn)
    folder_root, _ = os.path.split(folder_in)

    folder_out = os.path.join(folder_root, 'binary')
    if not os.path.exists(folder_out):
        os.mkdir(folder_out)

    basename_out = basename[:-3] + 'png'

    imageio.imwrite(os.path.join(folder_out, basename_out), img_bin)




plt.imshow(img_bw)
plt.show()
