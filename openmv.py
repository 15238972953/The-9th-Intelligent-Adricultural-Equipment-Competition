import sensor, image, time, math
import json
from pyb import UART

thresholds = [(63,90,-60,-15,-45,45),  #绿色
              (85,100,-22,2,-1,87)]    #黄色

sensor.reset() # 初始化摄像头
sensor.set_pixformat(sensor.RGB565) # 格式为 RGB565.
sensor.set_framesize(sensor.QQVGA) # 使用 QQVGA 速度快一些
sensor.skip_frames(time = 2000) # 跳过2000s，使新设置生效,并自动调节白平衡
sensor.set_auto_gain(False) # 关闭自动自动增益。默认开启的，在颜色识别中，一定要关闭白平衡。
sensor.set_auto_whitebal(False)
#关闭白平衡。白平衡是默认开启的，在颜色识别中，一定要关闭白平衡。
clock = time.clock() # 追踪帧率

uart = UART(3, 115200)
uart.init(115200, bits=8, parity=None, stop=1)  #8位数据位，无校验位，1位停止位

#定义一个函数，返回最大色块的坐标，因为在寻找同一颜色的色块时，可能存在多个同一颜色的色块，故我们只需要找到最大的色块即可
def find_max(blobs):
    max_size=0
    for blob in blobs:
       if blob.pixels() > max_size:
            max_blob=blob
            max_size = blob.pixels()
    return max_blob #寻找最大色块并返回最大色块的坐标

while(True):
    clock.tick() # Track elapsed milliseconds between snapshots().
    img = sensor.snapshot() # 从感光芯片获得一张图像

    blobs0 = img.find_blobs([thresholds[0]])
    blobs1 = img.find_blobs([thresholds[1]])
    #find_blobs(thresholds, invert=False, roi=Auto),thresholds为颜色阈值，
    #是一个元组，需要用括号［ ］括起来。invert=1,反转颜色阈值，invert=False默认
    #不反转。roi设置颜色识别的视野区域，roi是一个元组， roi = (x, y, w, h)，代表
    #从左上顶点(x,y)开始的宽为w高为h的矩形区域，roi不设置的话默认为整个图像视野。
    #这个函数返回一个列表，[0]代表识别到的目标颜色区域左上顶点的x坐标，［1］代表
    #左上顶点y坐标，［2］代表目标区域的宽，［3］代表目标区域的高，［4］代表目标
    #区域像素点的个数，［5］代表目标区域的中心点x坐标，［6］代表目标区域中心点y坐标，
    #［7］代表目标颜色区域的旋转角度（是弧度值，浮点型，列表其他元素是整型），
    #［8］代表与此目标区域交叉的目标个数，［9］代表颜色的编号（它可以用来分辨这个
    #区域是用哪个颜色阈值threshold识别出来的）。
    if blobs0:
        max_blob=find_max(blobs0)
        # Draw a rect around the blob.
        img.draw_rectangle(max_blob[0:4]) # rect
        #用矩形标记出目标颜色区域
        img.draw_cross(max_blob[5], max_blob[6]) # cx, cy
        #在目标颜色区域的中心画十字形标记
        data = bytearray([1])
        uart.write(data)
        print(1)
    elif blobs1:
        max_blob=find_max(blobs1)
        # Draw a rect around the blob.
        img.draw_rectangle(max_blob[0:4]) # rect
        #用矩形标记出目标颜色区域
        img.draw_cross(max_blob[5], max_blob[6]) # cx, cy
        #在目标颜色区域的中心画十字形标记
        data = bytearray([2])
        uart.write(data)
        print(2)
    else:
        data = bytearray([3])
        uart.write(data)
        print(3)
