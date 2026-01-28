import pandas as pd
import can
import time

def get_time_stamp():
    t = time.time()
    local_time = time.strftime("%H:%M:%S", time.localtime(t))
    milliseconds = int((t - int(t)) * 1000)
    return local_time, milliseconds

def log_rx(msg):
    local_time, ms = get_time_stamp()
    data_str = " ".join(f"{b:02X}" for b in msg.data)
    log_file.write(
        f"{local_time}:{ms:04d} Rx 1 0x{msg.arbitration_id:03X} s {msg.dlc} {data_str}\n"
    )

def wait_for_last_can_id():
    while True:
        rx_msg = bus.recv()
        log_rx(rx_msg)
        print(hex(rx_msg.arbitration_id))

if __name__ == "__main__":
    log_filename = "BusMaster_log.log"
    log_file = open(log_filename, "a")
    local_time, ms = get_time_stamp()
    log_file.write(f"""
-***BUSMASTER Ver 3.2.2***
***PROTOCOL CAN***
***NOTE: PLEASE DO NOT EDIT THIS DOCUMENT***
***[START LOGGING SESSION]***
***START DATE AND TIME *** {local_time}:{ms:04d}
***<Time><Tx/Rx><Channel><CAN ID><Type><DLC><DataBytes>***
""")
    
    bus = can.Bus(interface='pcan',channel='PCAN_USBBUS1',bitrate=500000)
    wait_for_last_can_id()
    local_time, ms = get_time_stamp()
    log_file.write(f"""
***END DATE AND TIME {local_time}:{ms:04d} ***
***[STOP LOGGING SESSION]***
""")

    log_file.close()
    bus.shutdown()
