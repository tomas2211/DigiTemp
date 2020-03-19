import imageio
import numpy as np
import matplotlib.pyplot as plt
import cv2
import cairosvg
from PIL import Image
import io


img = Image.open(
    io.BytesIO(cairosvg.svg2png(url='175855-weather-icon-compilation/svg/mist.svg',
                                output_width=74, output_height=74))
)


img_np = np.asanyarray(img).astype(np.float32) / 255.0
# img = imageio.imread('175855-weather-icon-compilation/png/drizzle.png').astype(np.float32) / 255.0
# img = cv2.resize(img, dsize=(25, 25), interpolation=cv2.INTER_CUBIC)

img_np = img_np[:,:,3] #np.sum(img_np[:,:,3],axis=2) / 3.0

img_bin = img_np > 0.5

plt.imshow(img_bin)
# plt.colorbar()
plt.show()

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


bits = 4

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

print('Prev size (B):', len(img_lin) / 8)
print('Compressed to (B): ',(len(out) * bits) / 8)

assert bits in [2,4,8]

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

str_out = 'const uint8_t sun[] PROGMEM = {\n\t'
for c in coded:
    str_out += '%#04x, ' % c

str_out = str_out[:-2] + '\n};'

print(str_out)