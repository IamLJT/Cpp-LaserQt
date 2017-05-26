import json
import queue
import socket
import sys
import time
import xlrd

HOST, PORT = "localhost", 2000
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

excelReadOnly = xlrd.open_workbook('path.xlsx')
table = excelReadOnly.sheets()[0]
for i in range(table.nrows):
    x_start = float(table.cell(i, 0).value)
    y_start = float(table.cell(i, 1).value)
    x_end = float(table.cell(i, 2).value)
    y_end = float(table.cell(i, 3).value)
    path_index = i + 1
    flag = int(table.cell(i, 6).value)

    k = (y_end - y_start) / (x_end - x_start)
    step = (x_end - x_start) / 100
    q = queue.Queue()
    for j in range(100):
        q.put([round(x_start + j * step, 3), round(y_start + j * k * step, 3), path_index, flag])
    while not q.empty():
        sock.sendto(bytes(json.dumps(q.get()), "utf-8"), (HOST, PORT))
        time.sleep(0.1)
sock.sendto(bytes(json.dumps([0, 0, 15, 0]), "utf-8"), (HOST, PORT))
