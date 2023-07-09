#模块名：Cable_tracking.py
#功能: 电缆识别
#- By: leo - 周六 8月 3 2019
#导入内部模块：
from pyb import LED,Timer
import sensor, image, time, math, struct

#实现功能：
    #柱子计数
    #柱子识别并发送中心坐标及宽度
    #黄色色块识别并发送中心坐标及面积
    #停止之后等待飞控信号进行拍照

#导入自定义模块
import Message

#全局变量：
green_led = LED(2)
pass_flag = [0, 0, 0]
robs = 0 #经过的立柱数量
Yellow_yes = 0 #拍照黄标成功标志
QR_code_yes = 0  #拍照二维码成功标志
state = 0 #状态机
blink = 0 #闪烁标志
cnt_1 = 0 #计数器1
cnt_2 = 0 #计数器2

#柱子检测区域
ROIS = {
    'right':  (130, 10,  20, 110), # 纵向取样-右侧       1
    'middle':    (70,  10,  20, 110), # 中央     2
    'left':   (10,  10,  20, 110), # 纵向取样-左侧       3
}

Black_threshold = (0, 30, -10, 127, -80, 127)
#Black_threshold = (0, 21, -4, 18, -12, 9)
#Yellow_threshold = (40, 100, -128, 127, 41, 127) #黄色色块阈值
#Yellow_threshold = (43, 100, -35, 26, 58, 127)
Yellow_threshold = (31, 77, -14, 29, 34, 89) #室外最宽容

#类定义
class cable():
    sta = 0 #识别标志位 1-> 1号立柱 2-> 黄色条码 3-> 2号立柱 0 -> 无效数据
    pause = 0 #请求飞机暂停
    turn = 0 #转向
    cx = 0 #标志物坐标
    cy = 0
    area = 0 #标志物面积 或者是柱子宽度

cable = cable()

#整个控制函数(3.0)
def Cable_track_3(img, Fly_word): #传入参数：图像及飞控发来的信息
    global state, robs, cnt_2, blink, green_led, Yellow_yes, QR_code_yes, pass_flag
    Rob_width = 60
    cable.sta = 0

    if (Fly_word == 2):
        OPMV_reset() #飞机与OPMV同时复位

    #控制逻辑：
    if (state == 0):#状态0 找第一个柱子
        B_rob_sta= Black_robs_detect(img) #找柱子
        if B_rob_sta['sta'] == 1:
            cable.sta = 1 #确认第一根柱子存在,更新信息
            cable.cx = B_rob_sta['cx']
            cable.cy = B_rob_sta['cy']
            cable.area = B_rob_sta['width']
            cable.pause = 1 #请求暂停

        if (Fly_word or B_rob_sta['width'] >= Rob_width):
            pass_flag[0] = 1 #确认通过
            cable.pause = 0 #取消暂停
            state = 1


    elif (state == 1): #状态1 找黄块
        Y_code_sta = Yellow_code_detect(img) #找色块
        if Y_code_sta['sta']:
            cable.sta = 2 #确认黄块存在,更新其信息，直到飞控发送Ok命令
            cable.cx = Y_code_sta['cx']
            cable.cy = Y_code_sta['cy']
            cable.area = Y_code_sta['size']
            cable.pause = 1 #请求暂停

        if cable.sta == 2 and cable.area >= 2000:
            blink = 1
        if cnt_2 <= 50 and blink: # 闪烁50次
            green_led.toggle()
            cnt_2 += 1
        else:
            green_led.off()

        if Fly_word == 1 and not Yellow_yes: #如果收到飞控OK的信息,拍照
            #take_pic('Y_code') #拍照
            #print('Y_code pic save ok'
            Yellow_yes = 1
            pass_flag[1] = 1 #确认通过
            cable.pause = 0 #取消暂停
            state = 2

    elif (state == 2): #状态2 找柱子
        B_rob_sta= Black_robs_detect(img) #找柱子
        if B_rob_sta['sta'] == 1:
            cable.sta = 3 #确认第二根柱子存在
            cable.cx = B_rob_sta['cx']
            cable.cy = B_rob_sta['cy']
            cable.area = B_rob_sta['width']
            cable.pause = 1 #请求暂停

        if Fly_word == 1:
            #take_pic('QR_code') #拍照
            #print('QR_code pic save ok')
            QR_code_yes = 1
            pass_flag[2] = 1 #确认通过
            cable.pause = 0 #取消暂停
            state = 3

    print('state:', state)
    Message.UartSendData(Message.LineDataPack(cable.sta, cable.pause, cable.turn, cable.cx, cable.cy, cable.area))


#整个控制函数(2.0)
def Cable_track_2(img, Fly_word): #传入参数：图像及飞控发来的信息
    global state, robs, cnt_2, blink, green_led, Yellow_yes, QR_code_yes, pass_flag
    Rob_width = 60
    #刷新各种标志位的状态
    cable.sta = 0
    Y_code_sta = Yellow_code_detect(img)
    B_rob_sta= Black_robs_detect(img)
    #count_robs(img) #数柱子
    if (Fly_word == 2):
        OPMV_reset() #飞机与OPMV同时复位

    #控制逻辑：
    if (state == 0):#状态0 根据看到的情况做判断
        if B_rob_sta['sta'] == 1 and B_rob_sta['width'] >= 20 and not pass_flag[0]:
            state = 1

        elif B_rob_sta['sta'] == 1 and B_rob_sta['width'] >= 20 and Yellow_yes and not QR_code_yes and not pass_flag[2]:
            state = 3 #等待稳定，拍照

        elif Y_code_sta['sta'] == 1 and not Yellow_yes and not pass_flag[1]:
            state = 2 #等待稳定，拍照

    if (state == 1): #1号柱子分支
        cable.sta = 1 #确认第一根柱子存在,更新信息
        cable.cx = B_rob_sta['cx']
        cable.cy = B_rob_sta['cy']
        cable.area = B_rob_sta['width']

        cable.pause = 1 #请求暂停
        if (B_rob_sta['width'] >= Rob_width):
            pass_flag[0] = 1 #确认通过,一号柱子的判断不再进
            cable.pause = 0 #取消暂停
            state = 0

    if (state ==2): #有一次看到黄色30cm左右，开始闪烁，闪烁完成之后拍照
        cable.sta = 2 #确认黄块存在,更新其信息，直到飞控发送Ok命令
        cable.cx = Y_code_sta['cx']
        cable.cy = Y_code_sta['cy']
        cable.area = Y_code_sta['size']

        cable.pause = 1 #请求暂停
        if cable.sta == 2 and cable.area >= 2000:
            blink = 1
        if cnt_2 <= 50 and blink: # 闪烁50次
            green_led.toggle()
            cnt_2 += 1
        else:
            green_led.off()

        if Fly_word == 1 and not Yellow_yes: #如果收到飞控OK的信息 且 只拍一次，拍完才进入状态4，否则循环在状态2
            #take_pic('Y_code') #拍照
            #print('Y_code pic save ok'
            Yellow_yes = 1

            pass_flag[1] = 1 #确认通过,黄色判断不再进
            cable.pause = 0 #取消暂停
            state = 0

    if (state == 3): #看到柱子之后，直到飞控发来OK信息 且 只拍一次，否则循环在状态3
        cable.sta = 3 #确认第二根柱子存在
        cable.cx = B_rob_sta['cx']
        cable.cy = B_rob_sta['cy']
        cable.area = B_rob_sta['width']
        cable.pause = 1 #请求暂停

        if Fly_word == 1:
            #take_pic('QR_code') #拍照
            #print('QR_code pic save ok')

            QR_code_yes = 1
            pass_flag[2] = 1 #确认通过,黄色判断不再进
            cable.pause = 0 #取消暂停
            state = 0
    print(pass_flag)
    # if (robs == 2):#两个柱子之后就发转向结束了，不再进入任何状态
        # turn = 1
        # state = 4
    Message.UartSendData(Message.LineDataPack(cable.sta, cable.pause, cable.turn, cable.cx, cable.cy, cable.area))

#复位
def OPMV_reset():
    global state, robs, cnt_2, blink, green_led, cnt_1, Yellow_yes, QR_code_yes, pass_flag
    for i in range(3):
        pass_flag[i] = 0
    robs = 0 #经过的立柱数量
    Yellow_yes = 0 #拍照黄标成功标志
    QR_code_yes = 0  #拍照二维码成功标志
    state = 0 #状态机
    blink = 0 #闪烁标志
    cnt_1 = 0 #计数器1
    cnt_2 = 0 #计数器2
    cable.sta = 0
    cable.pause = 0
    cable.turn = 0
    cable.cx = 0
    cable.cy = 0
    cable.area = 0

#数柱子函数，调用返回当前经过的柱子数
def count_robs(img):
    global ROIS, cnt_1, robs
    roi_blobs_result = {}  # 在各个ROI中寻找色块的结果记录
    for roi_direct in ROIS.keys():#数值复位
        roi_blobs_result[roi_direct] = {
            'cx': -1,
            'cy': -1,
            'blob_flag': False
        }
    for roi_direct, roi in ROIS.items():
        blobs=img.find_blobs([Black_threshold], roi=roi, merge=True,area_threshold = 2000, x_stride = 15)
        if len(blobs) == 0:
            continue

        largest_blob = max(blobs, key=lambda b: b.pixels())
        x,y,width,height = largest_blob[:4]

        if not(width >=5 and height >= 50):
            # 根据色块的长宽进行过滤
            continue

        roi_blobs_result[roi_direct]['cx'] = largest_blob.cx()
        roi_blobs_result[roi_direct]['cy'] = largest_blob.cy()
        roi_blobs_result[roi_direct]['blob_flag'] = True
        img.draw_rectangle((x,y,width, height), color=(0,255,255))

    if not roi_blobs_result['left']['blob_flag'] and\
        not roi_blobs_result['middle']['blob_flag'] and\
        roi_blobs_result['right']['blob_flag']:
        if cnt_1 == 0 or cnt_1 == 3:
            cnt_1 += 1
    elif not roi_blobs_result['left']['blob_flag'] and\
        roi_blobs_result['middle']['blob_flag'] and\
        not roi_blobs_result['right']['blob_flag']:
        if cnt_1 == 1 or cnt_1 == 4:
            cnt_1 += 1
    elif roi_blobs_result['left']['blob_flag'] and\
        not roi_blobs_result['middle']['blob_flag'] and\
        not roi_blobs_result['right']['blob_flag']:
        if cnt_1 == 2 or cnt_1 == 5:
            cnt_1 += 1
            robs += 1
    print('%d robs passed by' % robs)
    return robs

#黄色色块检测
def Yellow_code_detect(img):
    Yellow_code_sta = {'sta' : 0, 'cx' : 0, 'cy' : 0, 'size' : 0}
    maxblob = 0
    for Y_blobs in img.find_blobs([Yellow_threshold], x_stride = 10, y_stride = 10, merge = True, margin = 10):
        if maxblob < Y_blobs.area():
            maxblob = Y_blobs.area()
            max_blob = Y_blobs
    if(maxblob != 0):
        if(max_blob.area() >= 100):
            img.draw_rectangle(max_blob.rect())
            Yellow_code_sta['sta'] = 1
            Yellow_code_sta['cx'] = max_blob.cx()
            Yellow_code_sta['cy'] = max_blob.cy()
            Yellow_code_sta['size'] = max_blob.area()
            print('Yellow area' , max_blob.area())
    return Yellow_code_sta

#立柱检测
def Black_robs_detect(img):
    B_img = img.copy() #复制图像
    B_img.to_grayscale() #转灰度
    B_img.binary([(0, B_img.get_histogram().get_threshold().value())], invert = True).close(6) #二值化
    Black_rob_sta = {'sta' : 0, 'cx' : 0, 'cy' : 0, 'width' : 0}
    maxblob = 0
    for B_blobs in B_img.find_blobs([(0, 10)], area_threshold = 200, x_stride = 5):
        if maxblob < B_blobs.area():
            maxblob = B_blobs.area()
            max_blob = B_blobs
    if(maxblob != 0):
        if(max_blob.y() <= 10 and max_blob.w() >= 20 and max_blob.w() <= 90 and  max_blob.h() >= 100 and max_blob.area() <= 9600):
            img.draw_rectangle(max_blob.rect(), color = (255, 0, 0), thickness = 4)
            Black_rob_sta['sta'] = 1
            Black_rob_sta['cx'] = max_blob.cx()
            Black_rob_sta['cy'] = max_blob.cy()
            Black_rob_sta['width'] = max_blob.w()
    return Black_rob_sta
#拍照函数
def take_pic(name):
    sensor.set_pixformat(sensor.GRAYSCALE)
    sensor.set_framesize(sensor.VGA) #拍三张图片保存到本地，分辨率640*480
    sensor.skip_frames(time = 500)
    for i in range(3):
        string = name+str(i)+".jpg"
        sensor.snapshot().save(string)
        time.sleep(500)
    sensor.set_pixformat(sensor.RGB565)
    sensor.set_framesize(sensor.QQVGA)
    sensor.skip_frames(time = 500)

#测试部分
if __name__ == "__main__":
    sensor.reset()
    sensor.set_pixformat(sensor.RGB565)
    sensor.set_framesize(sensor.QQVGA)
    sensor.skip_frames(time = 2000)
    sensor.set_saturation(3)
    Fly_word = 1
    while(True):
        img = sensor.snapshot()
        Cable_track_2(img, Fly_word)
        #print('\n'.join(['%s:%s' % item for item in cable.__dict__.items()]))


