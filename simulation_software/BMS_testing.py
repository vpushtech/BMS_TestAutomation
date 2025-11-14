import pandas as pd
import can
import time
import struct
import unittest

can_id = 0x1C0
can_rxid = 0x1E0
base_address = 0x20000000
actual_address = None
length = None
value = None
data_type =None
OV = 3650
UV = 2500
OC = 20
OT = 60
UT = -10
cnt=0
sent_volVal = None
sent_currVal = None
sent_tempVal = None
over_vol = None
under_vol = None
over_current = None
under_cur = None
over_temp = None
under_temp = None

def send_canFrame():
    global cnt,sent_volVal,sent_currVal,sent_tempVal
    if cnt == 0:
        sent_volVal = value
    elif cnt == 1:
        sent_currVal = value
    elif cnt == 2:
        sent_tempVal = value
   
    byte_offset = actual_address - base_address
    offset_bytes = byte_offset.to_bytes(2,byteorder='little',signed=True)
    length_byte = int(length).to_bytes(1,byteorder='little',signed=True)

    if data_type == 'I16':
        payload = int(value).to_bytes(4,byteorder='little',signed=True)
    elif data_type == 'U16':
        payload = int(value).to_bytes(4,byteorder='little',signed=False)
    elif data_type == 'F32':
        payload = struct.pack('<f', float(value))

    reserved_byte = bytes([0xFF])

    data = offset_bytes + length_byte + payload + reserved_byte
    msg= can.Message(arbitration_id=can_id,data=data,is_extended_id=False,dlc=8)
    bus.send(msg)

def ReceiveCAN_Response():
    global over_vol,under_vol,over_current,under_cur,under_temp,over_temp
    rx_msg = bus.recv()
    if rx_msg is not None:
        if rx_msg.arbitration_id == 0x1D0:
            pass
        if rx_msg.arbitration_id == 0x1E0:
            over_vol = rx_msg.data[0]
            print(f"over_voltage: {over_vol}")
            under_vol = rx_msg.data[1]
            print(f"Under_voltage: {under_vol}")
            over_current = rx_msg.data[2]
            print(f"over_current: {over_current}")
            under_cur = rx_msg.data[3]
            print(f"under_cur: {under_cur}")
            over_temp = rx_msg.data[4]
            print(f"over_temp: {over_temp}")
            under_temp = rx_msg.data[5]
            print(f"under_temp: {under_temp}")

class TestBMS(unittest.TestCase):
    """BMS Test Cases"""
    def test_over_voltage(self):
        """Test for over voltage protection"""
        if over_vol is not None:
            self.assertEqual(over_vol, sent_volVal >= OV)

    def test_under_voltage(self):
        """Test for under voltage protection"""
        if under_vol is not None:
            self.assertEqual(under_vol, sent_volVal <= UV)
   
    def test_over_current(self):
        """Test for over current protection"""
        if over_current is not None:
            self.assertEqual(over_current, sent_currVal >= OC)

    def test_over_temp(self):
        """Test for over temperature protection"""
        if over_temp is not None:
            self.assertEqual(over_temp, sent_tempVal >= OT)

    def test_under_temp(self):
        """Test for under temperature protection"""
        if under_temp is not None:
            self.assertEqual(under_temp, sent_tempVal <= UT)

def Read_ExcelData(file_path):
    global actual_address,length,value,data_type,cnt
    df = pd.read_excel(file_path, sheet_name='Sheet1', skiprows=3, nrows=8)

    for col_idx in range(6,25):
        for index, row in df.iterrows():
            if pd.notna(row.get('Address')) and str(row.get('Address', '')).startswith('0x'):
                address = row.get('Address', 'N/A')
                length = row.get('Length', 'N/A')
                data_type = row.get('Type', 'N/A')
                variable_name = row.get('Name _of_variable', 'N/A')
                value = row.iloc[col_idx] if col_idx < len(row) else 'N/A'
                actual_address = int(address,16)
                print(value)
                send_canFrame()
                cnt = cnt+1
                time.sleep(0.05)

                if(cnt == 3):
                    cnt=0
                    ReceiveCAN_Response()
                    suite = unittest.TestLoader().loadTestsFromTestCase(TestBMS)
                    unittest.TextTestRunner(verbosity=2).run(suite)


    bus.shutdown()

if __name__ == "__main__":
    file_path = r"C:\Users\PandurangaKarapothul\Downloads\BMS_Excel.xlsx"
    #bus = can.ThreadSafeBus(interface='pcan',channel='PCAN_USBBUS1')
    bus = can.Bus(interface='pcan',channel='PCAN_USBBUS1',bitrate=500000)
    bus.set_filters([{"can_id":can_rxid,"can_mask":0x7FF,"extended":False }])
    Read_ExcelData(file_path)