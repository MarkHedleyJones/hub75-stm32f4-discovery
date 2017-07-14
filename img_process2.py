from scipy import misc
import glob

for image_path in glob.glob("./sign.png"):
    image = misc.imread(image_path)

data = []
count = 0
for y_l0 in reversed(range(8)):
    for y_l1 in [32, 64, 96]:
        for y1_l2, y2_l2 in [(16, 0), (24, 8)]:
            for x_l0 in reversed(range(64)):

                x = x_l0
                y1 = y_l0+y_l1+y1_l2
                y2 = y_l0+y_l1+y2_l2

                r1 = image[y1][x][0] >= 128
                g1 = image[y1][x][1] >= 128
                b1 = image[y1][x][2] >= 128

                r2 = image[y2][x][0] >= 128
                g2 = image[y2][x][1] >= 128
                b2 = image[y2][x][2] >= 128

                a = (7-y_l0) & 1
                b = (7-y_l0) & 2
                c = (7-y_l0) & 4

                out = 0
                if r1:
                    out += 1
                if g1:
                    out += 2
                if b1:
                    out += 4
                if r2:
                    out += 8
                if g2:
                    out += 16
                if b2:
                    out += 32
                if a:
                    out += 64
                if b:
                    out += 128
                if c:
                    out += 256
                data.append(out)

datalen = len(data)
if (datalen > 0):
    print("uint16_t data["+str(len(data))+"] = {" + ",".join(map(str,data)) + "};")
