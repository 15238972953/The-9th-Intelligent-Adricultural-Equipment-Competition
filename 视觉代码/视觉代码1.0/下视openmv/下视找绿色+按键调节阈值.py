# Untitled - By: 23094 - 周二 7月 13 2021

import sensor, image, time, lcd, pyb, math, struct
from pyb import Pin, Timer, LED, UART

# 色块数据类
class Block_data(object):
    def __init__(self,color_threshold):
        self.flag = [0,0,0,0,0]
        self.color_threshold = color_threshold
        self.color = 0
        self.x = 0
        self.y = 0
    def data_pack(self):
        # 0xAA:帧头;
        t_data = bytearray([0xAA,self.flag[0],self.flag[0]])
        return t_data

# 串口接收控制类
class Receive(object):
    def __init__(self,mode,uart):
        self.uart_buf  = []
        self.state = 0
        self.uart = uart
        self.WorkMode = mode
    #读取串口缓存
    def UartReadBuffer(self):
        i = 0
        Buffer_size = self.uart.any()
        while i<Buffer_size:
            self.ReceivePrepare(self.uart.readchar())
            i = i + 1
    #串口通信协议接收
    def ReceivePrepare(self,data):
        if self.state==0:
            if data == 0xAA:#帧头
                self.uart_buf.append(data)
                self.state = 1
            else:
                self.state = 0
        elif self.state==1:
            if data==0 or data==1 or data==2 or data==3 or data==4 or data==5:
                self.uart_buf.append(data)
                self.state = 2
            else:
                self.state = 0
        elif self.state==2:
            self.state = 0
            self.uart_buf.append(data)
            self.ReceiveAnl()
            self.uart_buf=[]#清空缓冲区，准备下次接收数据
        else:
            self.state = 0
    #串口数据解析
    def ReceiveAnl(self):
        #校验
        if self.uart_buf[1] != self.uart_buf[2]:
            return
        #校验通过 设置模块工作模式
        self.WorkMode = self.uart_buf[1]


# 低通滤波
# 输入参数：旧值（列表） 新值 比例值（小于1）
# 作用：数据平滑
def low_pass_filter(o_value,n_value,ratio):
    o_value[0] = (1-ratio)*o_value[0]+ratio*n_value
    o_value[1] = (1-ratio)*o_value[1]+ratio*o_value[0]
    o_value[2] = (1-ratio)*o_value[2]+ratio*o_value[1]
    o_value[3] = int(o_value[2])


# 标志位判断函数
# 输入参数：旧值（列表） 新值
# 作用：防止丢帧影响数据输出
def flag_judge(o_value,n_value):
    #删除第一个旧值
    del o_value[0]
    #把新值加入
    o_value.insert(3,n_value)
    if o_value[4]==0:
        # 输出标志位无效时，需连续四个标志位有效，输出标志位才有效
        if o_value[0]==o_value[1]==o_value[2]==o_value[3]==1:
            o_value[4] = 1
        else:
            o_value[4] = 0
    else:
        # 输出标志位有效时，需连续四个标志位都无效，输出标志位才无效
        if o_value[0] or o_value[1] or o_value[2] or o_value[3]:
            o_value[4] = 1
        else:
            o_value[4] = 0

# 输入参数：色块对象的列表
# 作用：找到色块对象列表中最大的指定色块
def FindMax_blob(blobs):
    max_size=1
    if blobs:
        max_blob = 0
        for blob in blobs:
            blob_size = blob.w()*blob.h()
            #色块尺寸判断
            if ( (blob_size > max_size) & (blob_size > 50)   ) :
                #色块更新
                max_blob=blob
                #最大尺寸更新
                max_size = blob.w()*blob.h()
        return max_blob

# 输入参数：色块数据对象 串口对象
# 作用：色块检测
def BlockCheck(Blk_dat,uart_o,img,Roi):
    #img = sensor.snapshot(line_filter = LineFilter)#拍一张图像
    rt_blobs = img.find_blobs([Blk_dat.color_threshold],roi=Roi, pixels_threshold=3, area_threshold=3, merge=True, margin=5)
    max_blob=FindMax_blob(rt_blobs)#找到最大的那个
    if max_blob:
        img.draw_cross(max_blob.cx(), max_blob.cy())#物体中心画十字
        img.draw_rectangle(max_blob.rect())#画矩形框
        #获取坐标转换
        Blk_dat.x = max_blob.cx()-80
        Blk_dat.y = max_blob.cy()-60
        flag_judge(Blk_dat.flag,1)
    else:
        flag_judge(Blk_dat.flag,0)
    uart_o.write(Blk_dat.data_pack())

#阈值调节类
class threshold_adjust(object):
    #   按键1、2键值
    key = [0,0]
    #   按键计数，用于按键消抖
    key_count = 0
    def __init__(self,file_path,threshold_type):
        # file_path：      包含阈值的文件路径
        # threshold_type： 阈值的类型 0：GRAYSCALE(灰度)   1:RGB565(彩色)
        self.file_path = file_path
        self.threshold_type = threshold_type
        with open(file_path, 'r') as f:
            w = f.readline()
        self.adjust_threshold = [int(x) for x in w.split(',')]
    # 阈值加
    def threshold_add(self):
        if self.threshold_type == 0:
            self.adjust_threshold[self.key[1]%2] = self.adjust_threshold[self.key[1]%2] + 1
            if self.adjust_threshold[self.key[1]%2] >= 255:
                self.adjust_threshold[self.key[1]%2] = 255
        else:
            self.adjust_threshold[self.key[1]%6] = self.adjust_threshold[self.key[1]%6] + 1
            #L(亮度)限幅
            if ((self.key[1]%6) <= 1) & (self.adjust_threshold[self.key[1]%6] >= 100):
                self.adjust_threshold[self.key[1]%6] = 100
            #A B限幅
            elif self.adjust_threshold[self.key[1]%6] >= 127:
                self.adjust_threshold[self.key[1]%6] = 127
    # 阈值减
    def threshold_dec(self):
        if self.threshold_type == 0:
            self.adjust_threshold[self.key[1]%2] = self.adjust_threshold[self.key[1]%2] - 1
            if self.adjust_threshold[self.key[1]%2] <= 0:
                self.adjust_threshold[self.key[1]%2] = 0
        else:
            self.adjust_threshold[self.key[1]%6] = self.adjust_threshold[self.key[1]%6] - 1
            #L(亮度)限幅
            if ((self.key[1]%6) <= 1) & (self.adjust_threshold[self.key[1]%6] <= 0):
                self.adjust_threshold[self.key[1]%6] = 0
            #A B限幅
            elif self.adjust_threshold[self.key[1]%6] <= -128:
                self.adjust_threshold[self.key[1]%6] = -128
    # 阈值显示
    def threshold_show(self,img):
        if self.threshold_type == 0:
            img.draw_string( 8, 0, "min", color =128)
            img.draw_string(42, 0, "max", color =128)
            img.draw_string( 0, 10, "["+str(self.adjust_threshold[0]), color = 128)
            img.draw_string(32, 10, ",", color = 128)
            img.draw_string(42, 10, str(self.adjust_threshold[1]), color = 128)
            img.draw_string(66, 10, "]", color = 128)
            x = 20+35*(self.key[1]%2)
            img.draw_arrow(x, 30, x, 20, color = 128, size = 4, thickness = 2)
        else:
            show_color = (255,0,0)
            img.draw_string( 8, 0, "Lmin", color = show_color)
            img.draw_string(50, 0, "Lmax", color = show_color)
            img.draw_string( 0, 10, "["+str(self.adjust_threshold[0]), color = show_color)
            img.draw_string(40, 10, ",", color = show_color)
            img.draw_string(50, 10, str(self.adjust_threshold[1]), color = show_color)
            img.draw_string(82, 10, "]", color = show_color)
            img.draw_string( 8, 30, "Amin", color = show_color)
            img.draw_string(50, 30, "Amax", color = show_color)
            img.draw_string( 0, 40, "["+str(self.adjust_threshold[2]), color = show_color)
            img.draw_string(40, 40, ",", color = show_color)
            img.draw_string(50, 40, str(self.adjust_threshold[3]), color = show_color)
            img.draw_string(82, 40, "]", color = show_color)
            img.draw_string( 8, 60, "Bmin", color = show_color)
            img.draw_string(50, 60, "Bmax", color = show_color)
            img.draw_string( 0, 70, "["+str(self.adjust_threshold[4]), color = show_color)
            img.draw_string(40, 70, ",", color = show_color)
            img.draw_string(50, 70, str(self.adjust_threshold[5]), color = show_color)
            img.draw_string(82, 70, "]", color = show_color)
            if (self.key[1]%6) <= 1 :
                y = 20
            elif (self.key[1]%6) <= 3 :
                y = 50
            else:
                y = 80
            x = 25+40*(self.key[1]%2)
            img.draw_arrow(x, y+9, x, y, color = show_color, size = 4, thickness = 2)
    def threshold_save(self):
        thresholdsave=[str(i) for i in self.adjust_threshold]
        w1=",".join(thresholdsave)
        with open(self.file_path, 'w+') as f:
            f.write(w1)

# 按键1初始化
SW1 = Pin('P9', Pin.IN, Pin.PULL_UP)
# 从文件里面读取阈值
file_path1 = "RGB565_threshold.txt"
threshold1 = threshold_adjust(file_path1,1)
# 色块数据类实例化
block1=Block_data(tuple(threshold1.adjust_threshold))
# 串口初始化
uart = UART(3,500000)#初始化串口 波特率 500000
# 串口接收控制类实例化
Receive_ctrl=Receive(1,uart)
lcd.init()
#初始化镜头
sensor.reset()
#设置相机模块的像素模式
sensor.set_pixformat(sensor.RGB565)
#设置相机分辨率160*120
sensor.set_framesize(sensor.QQVGA2)
sensor.set_windowing((120,160))#选择出自己想要的元素
#55 90
Roi=(55,60,20,20)
#时钟
sensor.skip_frames(time=2000)
#追踪颜色,关闭白平衡
sensor.set_auto_whitebal(False)
#初始化时钟
clock = time.clock()
#主循环
def mode_1():
    #时钟初始化
    Receive_ctrl.UartReadBuffer()
    if Receive_ctrl.WorkMode==1:
        #寻色块
        BlockCheck(block1,uart,img,Roi)
    img.rotation_corr(x_rotation=0, y_rotation=0, z_rotation=180)
    lcd.display(img)

while(True):
    clock.tick()
    img = sensor.snapshot().lens_corr(1.8)
    if threshold1.key_count == 0:
        if not SW1.value():
            threshold1.key_count = pyb.millis()
            # 由显示状态到调节状态转换，初始化LCD与按键
            if threshold1.key[0] == 0:
                threshold1.key[0] = 1
                # 释放串口引脚
                uart.deinit()
                # 初始化lcd屏幕。
                #lcd.init()
                # 按键2、3、4初始化
                SW2 = Pin('P5', Pin.IN, Pin.PULL_UP)
                SW3 = Pin('P4', Pin.IN, Pin.PULL_UP)
                SW4 = Pin('P1', Pin.IN, Pin.PULL_UP)
            # 由调节状态到显示状态转换，释放LCD与按键按键引脚
            else:
                threshold1.key[0] = 0
                threshold1.threshold_save()
                #lcd.deinit()
                uart = UART(3,500000)
                block1.color_threshold=tuple(threshold1.adjust_threshold)
        # 调节模式
        if threshold1.key[0] == 1:
            if not SW2.value():
                threshold1.key_count = pyb.millis()
                threshold1.key[1] = threshold1.key[1] + 1
            elif  not SW3.value():
                threshold1.key_count = pyb.millis()
                threshold1.threshold_add()
            elif  not SW4.value():
                threshold1.key_count = pyb.millis()
                threshold1.threshold_dec()
    else:
        if pyb.elapsed_millis(threshold1.key_count)>150:
            threshold1.key_count = 0
    # 正常工作状态
    if threshold1.key[0] == 0:
        mode_1()
    # 调节状态
    else:
        img.binary([tuple(threshold1.adjust_threshold)])
        threshold1.threshold_show(img)
        img.rotation_corr(x_rotation=0, y_rotation=0, z_rotation=180, x_translation=0, y_translation=0)
        lcd.display(img) # 拍照并显示图像。
    print(clock.fps())

