#!/usr/bin/python3
# -*- coding: utf-8 -*-
# filename: frame_test.py

'''
Created on 2023-7-5
@author: huanglin
  #!/usr/bin/env python
'''

#import pexpect
import os
import sys
import platform    # For getting the operating system name
import subprocess  # For executing a shell command
import fcntl
import struct
import datetime
import re
import subprocess



# 参考 ~/gitbak/gitbak.py
# example.sh

# 解释          报文
kFrame = (
    ("预连接请求 [1] LINK-Request", "681e008105220300000000004271010000003c07e70705030f1811000080fa16"),
    ("预连接响应 [129] LINK-Response", "683000010522030000000000c21e81008007e7070b02102b3b000007e7070b02102c01000207e7070b02102c01000277ad16"),
    ("安全请求 [16] SECURITY-Request", "683500430502000020114310f42b10001105020003001002002000020020040200000110087406efe4e8900cda5d3ec56df93d3a2b8b16")
        )
kResultFile = 'result.txt'



# 一个解析的结果
class SplitResult:
    index = 0
    explain = ""
    frame = ""
    cmd = ""
    return_code = -1     # 运行./split698的结果码

    def __init__(self, index, explain, frame, cmd):
        self.index = index
        self.explain = explain
        self.frame = frame
        self.cmd = cmd
        
    def set_return_code(self, return_code):
        self.return_code = return_code

    def __str__(self) :
        return "index={} return_code={} explain=[{}] cmd=[{}]".format(self.index, self.return_code, self.explain, self.cmd)



def date_time_str():
    days= ["monday","tuesday","wednesday","thursday","friday","saturday","sunday"]
    now = datetime.datetime.now()
    wd = datetime.date.weekday(now)
    return now.strftime("%Y-%m-%d %H:%M:%S " + days[wd])



# 开始的消息
#zydz gitlab backup
#date: 2020-5-21 8:00:00
def banner():
    print("split698 tests frames.")   
    print("date: %s" % date_time_str())



def print_result(return_code) :
    if (0 == return_code) :
        print('ok')
    else:
        print('FAIL ', return_code)



def summary(total, fail_list, start):
    print('summary:')
    print('fail: %d / %d' % (len(fail_list), total))
    for item in fail_list:
        print('    {}'.format(item))

    print("date: %s" % date_time_str())
    start_ms0 = start.replace(microsecond=0)
    end = datetime.datetime.now().replace(microsecond=0)
    elapse = end - start_ms0
    print("elapse: %s (= %d seconds)" % ( elapse, elapse.total_seconds() ))









def run_cmd(frame):
    # cmd = 'echo "{}" | ./split698 >> {}; cat {}'.format(frame, kResultFile, kResultFile)
    cmd = 'echo "{}" | ./split698'.format(frame, kResultFile, kResultFile)
    return cmd


def clear_result_file():
    cmd = 'echo > {}'.format(kResultFile)
    subprocess.run(cmd, shell=True, cwd=".")


def print_blanks():
    print('\n\n\n\n\n')


def process_one_frame(index, explain, frame):
    print_blanks()
    print('{}\r\nexplain={}\r\nframe={}'.format(index, explain, frame))
    # print('{} explain={} frame={}'.format(index, explain, frame))
    cmd = run_cmd(frame)
    print('run_cmd: {}'.format(cmd))
    #subprocess.run(cmd, shell=True, cwd=".")
    process = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, cwd='.')
    output = process.stdout.read()
    output_decode = output.decode()
    # 去掉\r\n，替换成\n。否则vim下行尾出现^M
    output_return = output_decode.replace("\r\n", "\n")
    output_strip = output_return.strip()
    # 避免命令结束时，打印一个空白行出来
    if (output_strip) :
        print(output_strip)

    return_code = process.poll()
    if return_code is not None:
        #print_result(return_code)
        # Process has finished, read rest of the output 
        #for output in process.stdout.readlines():
        for output in process.stdout.read():
            print(output.decode().replace("\r\n", "\n").strip())
        sr = SplitResult(index, explain, frame, cmd)
        # return_code = process.returncode
        sr.set_return_code(return_code)
        return sr
    

def print_result(return_code) :
    if (0 == return_code) :
        print('ok')
    else:
        print('FAIL ', return_code)


def loop_all(frame_tuple):
    start = datetime.datetime.now()
    fail_list = []      # 元素为SplitResult
    cnt = 0
    for explain, frame in frame_tuple:
        cnt += 1
        # print('{} explain={} frame={}'.format(cnt, explain, frame))

        split_result = process_one_frame(cnt, explain, frame)
        print_result(split_result.return_code)
        if (0 < split_result.return_code) :
            fail_list += [split_result]
    print_blanks()
    summary(cnt, fail_list, start)



if __name__ == '__main__':
    banner()
    # clear_result_file()
    loop_all(kFrame)
    print("\nGoodbye frame_test.py\n\n\n")   # Print the result of the command.
    pass

