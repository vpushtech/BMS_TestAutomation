import pandas as pd
import can
import time
import struct

CAN_TX = 0x1C0
CAN_RX = 0x1E0

def sendCanFrame(dataType,value,length):
    print(f"{value} {length}")

    payload1 = int(length).to_bytes(1,byteorder='little',signed=False)
    if dataType == 'U16':
        payload2 = int(value[0]).to_bytes(2,byteorder='little',signed=False)
        payload3 = int(value[1]).to_bytes(2,byteorder='little',signed=False)
        payload = payload1+payload2+payload3
        
    elif dataType == 'I16':
        payload2 = int(value[0]).to_bytes(2,byteorder='little',signed=True)
        payload3 = int(value[1]).to_bytes(2,byteorder='little',signed=True)
        payload = payload1+payload2+payload3

    elif dataType == 'F32':
        payload2 = struct.pack('<f', float(value))
        payload = payload1+payload2
    
    print(payload.hex())
    msg = can.Message(arbitration_id=CAN_TX,data=payload,is_extended_id=False,dlc=5)
    bus.send(msg)
    Receive_CANframe()

def Receive_CANframe():
    rx_msg = bus.recv()
    if rx_msg is not None:
        if rx_msg.arbitration_id == 0x1E0:
            pass

def Read_ExcelData(filepath):
    value = [None] * 2
    totalBytes=0
    i=0
    df = pd.read_excel(filepath, sheet_name='Sheet1', skiprows=3, nrows=96)
    df['CAN_ID'] = df['CAN_ID'].ffill()

    for col_idx in range(7,9):
        for index, row in df.iterrows():
            length = row.get('Length')
            dataType = row.get('Data_Type')
            if length == 2:
                value[i] = row.iloc[col_idx] if col_idx < len(row) else 'N/A'
                totalBytes += length
                i+=1
            elif length == 4:
                fvalue = row.iloc[col_idx] if col_idx < len(row) else 'N/A'

            if totalBytes == 4:
                totalBytes=i=0
                sendCanFrame(dataType,value,length)
                value = [None] * 2
            elif length == 4:
                sendCanFrame(dataType,fvalue,length)
            
            time.sleep(0.5)

if __name__ == "__main__":
    filepath = r"C:\Users\PandurangaKarapothul\Downloads\BMS_Excel.xlsx"
    bus = can.Bus(interface='pcan',channel='PCAN_USBBUS1',bitrate=500000)
    bus.set_filters([{"can_id":CAN_RX,"can_mask":0x7FF,"extended":False }])
    Read_ExcelData(filepath)
    bus.shutdown()