from scipy import misc
import glob

for image_path in glob.glob("./src.png"):
    image = misc.imread(image_path)
    print(image.shape)
    print(image.dtype)

# for offset_global in [0, 16, 32, 48]:
#     for row1, row2 in [(31, 15), (23, 7)]:
#         for offset_local in [0, 8]:
#             for col in range(8):
#                 y1 = row1 - offset_local
#                 y2 = row2 - offset_local
#                 x  = offset_global + col
#                 print(x, y1)

# data = []
# count = 0
# for x_l0 in [0, 16, 32, 48]:
#     for y1_l0, y2_l0 in [(8, 24), (0,16)]:
#         for x_l1 in [0, 8]:
#             for y_l1 in [7,6,5,4,3,2,1,0]:
#                 for x_l2 in [7,6,5,4,3,2,1,0]:
#                     x = x_l0+x_l1+x_l2
#                     y1 = y1_l0+y_l1
#                     y2 = y2_l0+y_l1

#                     print(y2, x)

#                     r1 = image[y1][x][0] >= 128
#                     g1 = image[y1][x][1] >= 128
#                     b1 = image[y1][x][2] >= 128

#                     r2 = image[y2][x][0] >= 128
#                     g2 = image[y2][x][1] >= 128
#                     b2 = image[y2][x][2] >= 128

#                     a = y_l1 & 1
#                     b = y_l1 & 2
#                     c = y_l1 & 4

#                     out = 0
#                     if r1:
#                         out += 1
#                     if g1:
#                         out += 2
#                     if b1:
#                         out += 4
#                     if r2:
#                         out += 8
#                     if g2:
#                         out += 16
#                     if b2:
#                         out += 32
#                     if a:
#                         out += 64
#                     if b:
#                         out += 128
#                     if c:
#                         out += 256

#                     # data.append(out)

# # datalen = len(data)
# # if (datalen > 0):
# #     print("uint16_t data["+str(len(data))+"] = {" + ",".join(map(str,data)) + "};")


data = []
count = 0
for y_l0 in range(8):
    for x_l0 in [48, 32, 16, 0]:
        for y2_l1, y1_l1 in [(0, 16), (8,24)]:
                for x_l1 in [15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0]:
                    x = x_l0+x_l1
                    y1 = y_l0+y1_l1
                    y2 = y_l0+y2_l1

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
