import sensor, image, time, math, struct
import json
from pyb import LED,Timer
from struct import pack, unpack
import Message,Cable_tracking
#初始化镜头
sensor.reset()
#sensor.set_pixformat(sensor.RGB565)#设置相机模块的像素模式
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQVGA)
sensor.skip_frames(time=3000)#时钟
sensor.set_auto_whitebal(False)#若想追踪颜色则关闭白平衡
sensor.set_saturation(3)
clock = time.clock()#初始化时钟

Message.Ctr.IsDebug = 0
#主循环
while(True):
    img = sensor.snapshot()
    Message.UartReadBuffer()
    print('Fly_word:' ,Message.Ctr.Fly_word)
    Message.Ctr.Fly_word = 0
    Cable_tracking.Cable_track_3(img, Message.Ctr.Fly_word)
