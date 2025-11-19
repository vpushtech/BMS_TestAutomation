import pandas as pd
import re
import os

class CANMatrixProcessor:
    def __init__(self, excel_file_path):
        self.excel_file_path = excel_file_path
        self.can_matrix = {}
        self.signal_info = {}
        self.load_can_matrix_from_excel()
    
    def load_can_matrix_from_excel(self):
        try:
            df = pd.read_excel(self.excel_file_path, sheet_name='V_2', header=None)
            current_std_id = None
            
            for idx, row in df.iterrows():
                if pd.isna(row[1]) and pd.isna(row[4]):
                    continue
                
                if not pd.isna(row[1]) and str(row[1]).startswith('0x'):
                    current_std_id = int(row[1], 16)
                    self.can_matrix[current_std_id] = []
                
                signal_name = row[4] if not pd.isna(row[4]) else ''
                if signal_name and signal_name not in ['nan', 'Signal Name']:
                    signal_data = {
                        'std_id': current_std_id,
                        'byte_position': row[2] if not pd.isna(row[2]) else '',
                        'bit_position': row[3] if not pd.isna(row[3]) else '',
                        'signal_name': signal_name.strip(),
                        'data_type': row[5] if not pd.isna(row[5]) else '',
                        'factor': float(row[6]) if not pd.isna(row[6]) else 1,
                        'offset': float(row[7]) if not pd.isna(row[7]) else 0,
                    }
                    
                    byte_pos = self.parse_byte_position(signal_data['byte_position'])
                    signal_data['start_byte'] = byte_pos[0] if byte_pos else 0
                    signal_data['bit_pos'] = int(signal_data['bit_position']) if signal_data['bit_position'] else 0
                    
                    self.can_matrix[current_std_id].append(signal_data)
                    self.signal_info[signal_name.strip()] = signal_data
            
            print(f"Loaded {len(self.signal_info)} signals from {len(self.can_matrix)} CAN IDs")
            
        except Exception as e:
            print(f"Error loading CAN matrix: {e}")
    
    def parse_byte_position(self, byte_pos_str):
        if not byte_pos_str: return None
        try:
            if '-' in byte_pos_str:
                return tuple(map(int, byte_pos_str.split('-')))
            return (int(byte_pos_str), int(byte_pos_str))
        except:
            return None
    
    def get_signals_for_can_id(self, can_id):
        return self.can_matrix.get(can_id, [])
    
    def get_all_signal_names(self):
        return list(self.signal_info.keys())

class CANLogProcessor:
    def __init__(self, log_file, output_csv, can_matrix_processor):
        self.log_file = log_file
        self.output_csv = output_csv
        self.can_matrix = can_matrix_processor
        self.processed_data = []
    
    def create_signal_template(self):
        all_signals = self.can_matrix.get_all_signal_names()
        return {signal: 0 for signal in all_signals}, all_signals

    def parse_log_line(self, line):
        pattern = r'(\d+:\d+:\d+:\d+)\s+([RT]x)\s+(\d+)\s+(0x[0-9A-F]+)\s+(\w+)\s+(\d+)\s+([0-9A-F\s]+)'
        match = re.match(pattern, line.strip())
        if match:
            timestamp, tx_rx, channel, can_id, msg_type, dlc, data_str = match.groups()
            return {
                'timestamp': timestamp,
                'tx_rx': tx_rx,
                'can_id': int(can_id, 16),
                'dlc': int(dlc),
                'data_bytes': bytes.fromhex(data_str.replace(' ', ''))
            }
        return None

    def extract_signal_value(self, signal_info, data_bytes):
        try:
            data_type = signal_info['data_type']
            factor = signal_info['factor']
            offset = signal_info['offset']
            start_byte = signal_info['start_byte']
            bit_pos = signal_info['bit_pos']
            
            if data_type == 'BIN':
                if start_byte < len(data_bytes):
                    return (data_bytes[start_byte] >> bit_pos) & 0x01
                return 0
            
            elif data_type in ['U8', 'U16', 'U32', 'I8', 'I16', 'I32']:
                num_bytes = 1 if '8' in data_type else (2 if '16' in data_type else 4)
                
                if start_byte + num_bytes <= len(data_bytes):
                    byte_slice = data_bytes[start_byte:start_byte + num_bytes]
                    if len(byte_slice) == num_bytes:
                        signed = data_type.startswith('I')
                        value = int.from_bytes(byte_slice, 'little', signed=signed)                       
                        return (value /factor) + offset
            return 0
            
        except Exception as e:
            return 0

    def process_can_message(self, message):
        can_id = message['can_id']
        data_bytes = message['data_bytes']
        extracted_signals = {}
        
        for signal_info in self.can_matrix.get_signals_for_can_id(can_id):
            signal_name = signal_info['signal_name']
            extracted_signals[signal_name] = self.extract_signal_value(signal_info, data_bytes)
        
        return extracted_signals

    def process_log_file(self):
        template_dict, signals_list = self.create_signal_template()
        current_set = template_dict.copy()
        current_timestamp = None
        processed_sets = []
        message_count = 0
        
        with open(self.log_file, 'r') as file:
            for line in file:
                if not line.strip() or line.startswith('***'):
                    continue
                
                message = self.parse_log_line(line)
                if message and message['tx_rx'] == 'Rx':
                    message_count += 1
                    
                    if message['can_id'] == 0x01 and current_timestamp:
                        current_set['timestamp'] = current_timestamp
                        processed_sets.append(current_set.copy())
                        current_set = template_dict.copy()
                    
                    current_set.update(self.process_can_message(message))
                    current_timestamp = message['timestamp']
        
        if current_timestamp:
            current_set['timestamp'] = current_timestamp
            processed_sets.append(current_set)
        
        self.processed_data = processed_sets
        print(f"Processed {message_count} messages")

    def save_to_csv(self):
        if not self.processed_data:
            print("No data to save!")
            return
        
        df = pd.DataFrame(self.processed_data)
        template_dict, all_signals = self.create_signal_template()
        
        for signal in all_signals:
            if signal not in df.columns:
                df[signal] = 0
        
        df = df[['timestamp'] + all_signals]
        df.to_csv(self.output_csv, index=False)
        print(f"Generated CSV with {len(df)} records")

def main():
    EXCEL_FILE = r"D:\log_file_data\VPUSH_TECH_BMS_CAN_MATRIX.xlsx"
    LOG_FILE = r"D:\log_file_data\24_hr_regressive_testing_01.log"
    OUTPUT_CSV = r"D:\log_file_data\bms_data.csv"
    
    if not os.path.exists(EXCEL_FILE):
        print(f"Excel file not found: {EXCEL_FILE}")
        return
    
    if not os.path.exists(LOG_FILE):
        print(f"Log file not found: {LOG_FILE}")
        return
    
    try:
        can_matrix = CANMatrixProcessor(EXCEL_FILE)
        log_processor = CANLogProcessor(LOG_FILE, OUTPUT_CSV, can_matrix)
        log_processor.process_log_file()
        log_processor.save_to_csv()
        print("CSV generation successful")        
    except Exception as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    main()