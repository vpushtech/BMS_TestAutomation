import pandas as pd
import can
import struct
import time
import re
import os
import tkinter as tk
from tkinter import filedialog, scrolledtext
import sys
import threading

CAN_TX = 0x1C0
LAST_CAN_ID = 0x714
RAM_BASE_ADDRESS = 0x20000000

stop_rx_flag = False

def get_time_stamp():
    t = time.time()
    local_time = time.strftime("%H:%M:%S", time.localtime(t))
    milliseconds = int((t - int(t)) * 1000)
    return local_time, milliseconds

def log_tx(msg):
    local_time, ms = get_time_stamp()
    data_str = " ".join(f"{b:02X}" for b in msg.data)
    log_file.write(f"{local_time}:{ms:04d} Tx 1 0x{msg.arbitration_id:03X} s {msg.dlc} {data_str}\n")


def log_rx(msg):
    local_time, ms = get_time_stamp()
    data_str = " ".join(f"{b:02X}" for b in msg.data)
    log_file.write(f"{local_time}:{ms:04d} Rx 1 0x{msg.arbitration_id:03X} s {msg.dlc} {data_str}\n")

def sendCanFrame(Address, length, dataType, value):
    offset = Address - RAM_BASE_ADDRESS
    payload0 = offset.to_bytes(2, 'little', signed=False)
    payload1 = int(length).to_bytes(1, 'little', signed=False)

    if dataType == 'U16':
        payload2 = bytes([0xAA])
        payload3 = int(value).to_bytes(4, 'little', signed=False)

    elif dataType == 'I16':
        payload2 = bytes([0xBB])
        payload3 = int(value).to_bytes(4, 'little', signed=True)

    elif dataType == 'I32':
        payload2 = bytes([0xCC])
        payload3 = struct.pack('<f', float(value))

    elif dataType == 'U32':
        payload2 = bytes([0xDD])
        payload3 = int(value).to_bytes(4, 'little', signed=False)

    elif dataType == 'U8':
        payload2 = bytes([0xEE])
        payload3 = int(value).to_bytes(4, 'little', signed=False)

    else:
        return

    payload = payload0 + payload1 + payload2 + payload3

    msg = can.Message(arbitration_id=CAN_TX,data=payload,is_extended_id=False,dlc=8)
    bus.send(msg)
    log_tx(msg)


def wait_for_last_can_id():
    while True:
        rx_msg = bus.recv(timeout=0.1)
        if rx_msg:
            log_rx(rx_msg)
            if rx_msg.arbitration_id == LAST_CAN_ID:
                break

def Read_ExcelData(filepath):
    df = pd.read_excel(filepath, sheet_name='TestValues')
    addresses = df.iloc[1]
    lengths = df.iloc[2]
    data_types = df.iloc[3]
    value_rows = df.iloc[4:30]
    count = 0

    for _, value_row in value_rows.iterrows():
        for col_idx in range(2, 51):
            Address = addresses.iloc[col_idx]
            Length = lengths.iloc[col_idx]
            DataType = data_types.iloc[col_idx]
            Value = value_row.iloc[col_idx]

            actualAddress = int(Address, 16)
            sendCanFrame(actualAddress, Length, DataType, Value)
        count += 1
        local_time, ms = get_time_stamp()
        print(f"{local_time}:{ms:04d} : Sent Row {count}")
        wait_for_last_can_id()

def receive_can_frames():
    global stop_rx_flag
    count = 0
    print("CAN RX STARTED")
    while not stop_rx_flag:
        rx_msg = bus.recv(timeout=0.2)
        if rx_msg:
            log_rx(rx_msg)
            count += 1
            print(f"{hex(rx_msg.arbitration_id)}   Count={count}")
    print("CAN RX STOPPED")

class CANMatrixProcessor:

    def __init__(self, excel_file):
        self.can_matrix = {}
        self.signal_order = []
        self.first_can_id = None
        self.load(excel_file)

    def load(self, excel_file):

        df = pd.read_excel(excel_file, sheet_name='V_4', header=None)
        header_row = None
        for idx, row in df.iterrows():
            if not pd.isna(row[1]) and str(row[1]).strip() == 'STD ID':
                header_row = idx
                break
        if header_row is None:
            header_row = 7
        data_df = df.iloc[header_row + 1:].reset_index(drop=True)
        data_df.columns = df.iloc[header_row]

        current_id = None

        for _, row in data_df.iterrows():
            if pd.notna(row['STD ID']):
                current_id = int(str(row['STD ID']), 16)
                if current_id not in self.can_matrix:
                    self.can_matrix[current_id] = []
                    if self.first_can_id is None:
                        self.first_can_id = current_id

            if pd.isna(row['Signal Name']):
                continue
            sig = {
                'signal_name': row['Signal Name'],
                'data_type': str(row['data type']).strip(),
                'factor': float(row['factor']) if not pd.isna(row['factor']) else 1.0,
                'offset': float(row['offset']) if not pd.isna(row['offset']) else 0.0
            }
            if pd.notna(row['Byte Position']):
                bp = str(row['Byte Position'])

                if '-' in bp:
                    s, e = map(int, bp.split('-'))
                    sig['start_byte'] = s
                    sig['num_bytes'] = e - s + 1
                else:
                    sig['start_byte'] = int(float(bp))
                    sig['num_bytes'] = 1

            if pd.notna(row['Bit position']):
                sig['bit_pos'] = int(float(row['Bit position']))

            self.can_matrix[current_id].append(sig)
            self.signal_order.append(sig['signal_name'])

    def get_signals(self, can_id):
        return self.can_matrix.get(can_id, [])

    def get_signal_order(self):
        return self.signal_order

    def get_first_can_id(self):
        return self.first_can_id

class CANLogProcessor:

    def __init__(self, log_file, output_csv, matrix):
        self.log_file = log_file
        self.output_csv = output_csv
        self.matrix = matrix
        self.rows = []

    def parse(self, line):
        pattern = r'(\d+:\d+:\d+:\d+)\s+Rx\s+\d+\s+(0x[0-9A-F]+)\s+\w+\s+\d+\s+([0-9A-F\s]+)'
        m = re.match(pattern, line)
        if not m:
            return None

        return m.group(1), int(m.group(2), 16), bytes.fromhex(m.group(3))

    def extract_bin_signal(self, sig, data):
        abs_bit = sig.get('bit_pos', 0)

        byte_index = abs_bit // 8
        bit_index = abs_bit % 8

        if byte_index >= len(data):
            return 0

        return (data[byte_index] >> bit_index) & 0x01

    def process(self):
        cycle = {}
        first_id = self.matrix.get_first_can_id()
        with open(self.log_file) as f:
            for line in f:
                parsed = self.parse(line)
                if not parsed:
                    continue
                ts, cid, data = parsed
                if cid == first_id and cycle:
                    self.rows.append(cycle.copy())
                    cycle.clear()

                if 'timestamp' not in cycle:
                    cycle['timestamp'] = ts

                for sig in self.matrix.get_signals(cid):
                    name = sig['signal_name']
                    dtype = sig['data_type'].upper()

                    if dtype == 'BIN':
                        value = self.extract_bin_signal(sig, data)
                        cycle[name] = value
                        continue

                    if 'start_byte' not in sig:
                        continue
                    sb = sig['start_byte']
                    nb = sig['num_bytes']
                    if sb + nb > len(data):
                        continue

                    raw = int.from_bytes(data[sb:sb + nb],'little',signed=('I' in dtype))

                    cycle[name] = raw * sig['factor'] + sig['offset']
        if cycle:
            self.rows.append(cycle)

    def save(self):
        df = pd.DataFrame(self.rows)
        cols = ['timestamp'] + [c for c in df.columns if c != 'timestamp']
        df = df[cols]
        df.to_csv(self.output_csv, index=False)
        print("\nCSV GENERATED SUCCESSFULLY")

class RedirectText:
    def __init__(self, widget):
        self.widget = widget

    def write(self, string):
        self.widget.insert(tk.END, string)
        self.widget.see(tk.END)

    def flush(self):
        pass

def run_process():
    global stop_rx_flag
    stop_rx_flag = False

    try:
        excel_path = excel_var.get()
        matrix_path = matrix_excel.get()
        output_folder = output_var.get()
        testmode = testmode_var.get()

        output_csv = os.path.join(output_folder, "test_csv_file.csv")
        log_path = os.path.join(output_folder, "test_log_file.log")

        global log_file, bus

        log_file = open(log_path, "a")
        bus = can.Bus(interface='pcan',channel='PCAN_USBBUS1',bitrate=500000)

        if testmode:
            print("TESTMODE ENABLED")
            Read_ExcelData(excel_path)
        else:
            print("TESTMODE DISABLED - RX ONLY MODE")
            receive_can_frames()

        log_file.close()
        bus.shutdown()

        matrix = CANMatrixProcessor(matrix_path)
        processor = CANLogProcessor(log_path, output_csv, matrix)
        processor.process()
        processor.save()
        print("\nPROCESS COMPLETED SUCCESSFULLY")

    except Exception as e:
        print("ERROR:", e)


def stop_rx():
    global stop_rx_flag
    stop_rx_flag = True
    print("STOP BUTTON PRESSED")


def start_process():
    threading.Thread(target=run_process, daemon=True).start()

root = tk.Tk()
root.title("CAN Automation Tool")
root.geometry("850x620")

excel_var = tk.StringVar()
matrix_excel = tk.StringVar()
output_var = tk.StringVar()
testmode_var = tk.IntVar(value=1)


def build_row(label, var, cmd):
    frame = tk.Frame(root)
    frame.pack(fill=tk.X, padx=10, pady=5)
    tk.Label(frame, text=label,font=("Segoe UI", 10, "bold"),width=20,anchor="w").pack(side=tk.LEFT)
    tk.Entry(frame, textvariable=var, width=55).pack(side=tk.LEFT, padx=5)
    tk.Button(frame, text="Browse",width=10,bg="#2F806B",fg="white",command=cmd).pack(side=tk.LEFT, padx=5)


build_row("Testing Input Excel:", excel_var,lambda: excel_var.set(filedialog.askopenfilename(filetypes=[("Excel Files", "*.xlsx *.xls")])))
build_row("CAN Matrix Excel:", matrix_excel,lambda: matrix_excel.set(filedialog.askopenfilename(filetypes=[("Excel Files", "*.xlsx *.xls")])))
build_row("CSV Output Folder:", output_var,lambda: output_var.set(filedialog.askdirectory()))

def testmode_changed():
    if testmode_var.get() == 0:
        stop_button.pack(pady=5)
    else:
        stop_button.pack_forget()


testmode_checkbox = tk.Checkbutton(root,text="TESTMODE",variable=testmode_var,font=("Segoe UI", 11, "bold"),command=testmode_changed)
testmode_checkbox.pack(pady=5)

tk.Button(root,text="RUN",bg="green",fg="white",font=("Arial", 11, "bold"),width=18,command=start_process).pack(pady=5)
stop_button = tk.Button(root,text="STOP",bg="red",fg="white",font=("Arial", 11, "bold"),width=18,command=stop_rx)
stop_button.pack_forget()

log_box = scrolledtext.ScrolledText(root, height=15)
log_box.pack(fill=tk.BOTH, expand=True)
sys.stdout = RedirectText(log_box)

root.mainloop()
