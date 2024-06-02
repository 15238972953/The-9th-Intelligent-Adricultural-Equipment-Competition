# Untitled - By: 23094 - 周二 7月 13 2021

import sensor, image, time, lcd, pyb, math, struct
from pyb import Pin, Timer, LED, UART
from machine import I2C
from vl53l1x import VL53L1X
# 色块数据类
class Block_data(object):
    def __init__(self,color_threshold):
        self.flag   = [0,0,0,0,0]
        self.color_threshold = color_threshold
        self.error_x= [0,0,0,0]
        self.diss   = [0,0,0,0]
    def data_pack(self,dis,txm):
        # 0xAA:帧头; 0x01:色块数据; 0x07:有效位数; 0x00:和校验位
        t_data = bytearray([0xAA,0x02,0x06,self.flag[4],self.error_x[3]>>8,self.error_x[3],dis>>8,dis,txm,0x00])
        sum = 0
        for i in range(0,len(t_data)-1):
            sum += t_data[i]
        t_data[-1]=sum
        return t_data

# 串口接收飞控数据类
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

# 串口接收条形码数据类
class Receive_txm(object):
    def __init__(self,uart):
        self.uart_buf  = []
        self.state  = 0
        self.uart   = uart
        self.txmdata= 0
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
            if data == 0x41:#帧头
                self.uart_buf.append(data)
                self.state = 1
            else:
                self.state = 0
        elif self.state>=1 and self.state<=6:
            if data == 0x0D:
                self.state = 0
                self.uart_buf.append(data)
                self.ReceiveAnl()
                self.uart_buf=[]#清空缓冲区，准备下次接收数据
            else:
                self.uart_buf.append(data)
                self.state = self.state + 1
        else:
            self.state = 0
    #串口数据解析
    def ReceiveAnl(self):
        if (self.uart_buf[0] == 0x41) and (self.uart_buf[-1] == 0x0D):
            #校验通过 设置模块工作模式
            self.txmdata = (self.uart_buf[3]-0X30)*10+(self.uart_buf[4]-0X30)

# 低通滤波
# 输入参数：旧值(4列表)、新值、新值比例（取值（0,1],1:全信新值）
# 作用：数据平滑
def low_pass_filter(o_value,n_value,ratio = 0.5):
    o_value[0] = (1-ratio)*o_value[0] + ratio*n_value
    o_value[1] = (1-ratio)*o_value[1] + ratio*o_value[0]
    o_value[2] = (1-ratio)*o_value[2] + ratio*o_value[1]
    o_value[3] = int(o_value[2])

# 标志位判断函数
# 输入参数：旧值(5列表)、新值
# 作用：防止丢帧影响数据输出
#   o_value[0]:新加入的值；o_value[4]：输出的值
def flag_judge(o_value,n_value,flag=1):
    if flag:
        o_value[3] = o_value[2]
        o_value[2] = o_value[1]
        o_value[1] = o_value[0]
        o_value[0] = n_value
        if o_value[4]==0 :
            # 输出标志位无效时，需连续四个标志位有效，输出标志位才有效
            if o_value[0]==o_value[1]==o_value[2]==o_value[3]==1 :
                o_value[4] = 1
            else:
                o_value[4] = 0
        else:
            # 输出标志位有效时，需连续四个标志位都无效，输出标志位才无效
            if o_value[0] or o_value[1] or o_value[2] or o_value[3] :
                o_value[4] = 1
            else:
                o_value[4] = 0
    else:
        o_value[0] = n_value
        o_value[1] = n_value
        o_value[2] = n_value
        o_value[3] = n_value
        o_value[4] = n_value

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

"""
# 输入参数：色块数据对象 串口对象
# 作用：色块检测
def BlockCheck(Blk_dat,uart_o,img,Roi,dis,txm):
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
    uart_o.write(Blk_dat.data_pack(dis,txm))
"""
def abs_value(w):
    if w<0:
        w = -w
    return w

def my_max(w):
    max_num = w[0]
    for i in w:
        if(i>max_num):
            max_num = i
    return max_num

def my_min(w):
    min_num = w[0]
    for i in w:
        if(i<min_num):
            min_num = i
    return min_num

# 输入参数：色块对象列表、尺寸限幅(默认参数) 指定的尺寸assign_size，方向(0:宽 1:高)
# 作用：找到指定宽度或者高度下的最大色块
def FindMax_assign_w_or_h_blob(blobs,min_bsize=1,max_bsize=500,assign_size=10,direction=1):
    max_size=1
    if blobs :
        max_blob = 0
        # 指定高度
        if direction:
            for blob in blobs:
                blob_size = blob.w()
                #色块尺寸判断
                if ((blob_size>max_size) & (blob_size>min_bsize) & (blob_size<max_bsize) & (blob.h()==assign_size)) :
                    #色块更新
                    max_blob=blob
                    #最大尺寸更新
                    max_size = blob.w()
        # 指定宽度
        else:
            for blob in blobs:
                blob_size = blob.h()
                #色块尺寸判断
                if ((blob_size>max_size) & (blob_size>min_bsize) & (blob_size<max_bsize) & (blob.w()==assign_size)) :
                    #色块更新
                    max_blob=blob
                    #最大尺寸更新
                    max_size = blob.h()
        return max_blob

# 输入参数：色块数据对象 串口对象
# 作用：色块检测
def BlockCheck(Blk_dat,uart_o,img,R_oi,dis,txm):
    r_b_r = {}
    for roi_direct in R_oi.keys():
        #字典是一个无序、可变和有索引的集合。在 Python 中，字典用花括号编写，拥有键和值。
        r_b_r[roi_direct] = {
            'cx': -1,
            'w': -1,
            'blob_flag': 0
        }
    #通过使用 items() 函数遍历键和值
    for roi_direct, roi in R_oi.items():
        blobs=img.find_blobs([Blk_dat.color_threshold], roi=roi, merge=True, pixels_area=10)
        largest_blob = FindMax_assign_w_or_h_blob(blobs,max_bsize=30)
        if largest_blob :
            r_b_r[roi_direct]['cx'] = largest_blob.cx()
            r_b_r[roi_direct]['w']  = largest_blob.w()
            r_b_r[roi_direct]['blob_flag'] = 1
            # 在色块周围画一个矩形。
            img.draw_rectangle(largest_blob.rect())
            # 将此区域的像素数最大的颜色块画矩形和十字形标记出来
            img.draw_cross(largest_blob.cx(),largest_blob.cy())
    line_cx = []
    line_w = []
    for roi_direct, roi in R_oi.items():
        if r_b_r[roi_direct]['blob_flag']==1:
            line_cx.append(r_b_r[roi_direct]['cx'])
            line_w.append(r_b_r[roi_direct]['w'])
    w_error = 2
    cx_error = 5
    if len(line_w)>=3:
        average_w = 0
        for i in line_w:
            average_w = average_w + i
        average_w = (average_w - my_max(line_w)- my_min(line_w))/(len(line_w)-2)
        average_cx = 0
        for i in line_cx:
            average_cx = average_cx + i
        average_cx = (average_cx - my_max(line_cx)- my_min(line_cx))/(len(line_w)-2)
        i_num = 0
        sc_w = 0
        sc_cx = 0
        for i in range(0,len(line_w)):
            if((abs_value(average_w-line_w[i])<=w_error) and (abs_value(average_cx-line_cx[i]))<=cx_error):
                i_num = i_num + 1
                sc_w = sc_w + line_w[i]
                sc_cx = sc_cx + line_cx[i]
        if i_num!=0:
            sc_w =  10*sc_w/i_num
            sc_cx = sc_cx//i_num
            sc_dis = 2292.537//sc_w
            low_pass_filter(Blk_dat.error_x,sc_cx-160,1)
            low_pass_filter(Blk_dat.diss,sc_dis,1)
            flag_judge(Blk_dat.flag,1)
            img.draw_line(sc_cx,0,sc_cx,240)
            img.draw_string(0, 0, str(Blk_dat.diss[3]))
        else:
            flag_judge(Blk_dat.flag,0)
            if not Blk_dat.flag[4]:
                low_pass_filter(Blk_dat.diss,0,1)
                low_pass_filter(Blk_dat.error_x,0,1)
    #没找到
    else:
        flag_judge(Blk_dat.flag,0)
        if not Blk_dat.flag[4]:
            low_pass_filter(Blk_dat.diss,0,1)
            low_pass_filter(Blk_dat.error_x,0,1)
    uart_o.write(Blk_dat.data_pack(dis,txm))


Black_threshold =(0, 50)      #黑色
# 色块数据类实例化
block1=Block_data(Black_threshold)
#初始化串口 波特率 500000
uart1 = UART(1,115200)
uart3 = UART(3,500000)
# 串口接收控制类实例化
txm=Receive_txm(uart1)
Receive_ctrl=Receive(1,uart3)
i2c = I2C(4)
distance = VL53L1X(i2c)
#led_R = LED(1) # 红led
#led_G = LED(2) # 绿LED
#led_B = LED(3) # 蓝LED
#led_R.on()#亮
#led_G.on()#亮
#led_B.on()#亮

#感兴趣区域
ROIS = {
    'up'    :(0,  40, 320, 10),
    'up_m'  :(0,  80, 320, 10),
    'middle':(0, 120, 320, 10),
    'down_m':(0, 160, 320, 10),
    'down'  :(0, 200, 320, 10),
}

#初始化镜头
sensor.reset()
#设置相机模块的像素模式
sensor.set_pixformat(sensor.GRAYSCALE)
#设置相机分辨率320*240
sensor.set_framesize(sensor.QVGA)
#时钟
sensor.skip_frames(time=2000)
#追踪颜色,关闭白平衡
#sensor.set_auto_whitebal(False)
#初始化时钟
clock = time.clock()
#主循环
while(True):
    clock.tick()
    img = sensor.snapshot().lens_corr(1.8)
    txm.UartReadBuffer()
    Receive_ctrl.UartReadBuffer()
    dis = distance.read()
    if Receive_ctrl.WorkMode==1:
        #寻色块
        BlockCheck(block1,uart3,img,ROIS,dis,txm.txmdata)
    #print(clock.fps())
    #print(txm.txmdata)
    print(dis)
    #print(block1.diss[3])

