import pandas as pd
import re
import os

class CANLogProcessor:
    def __init__(self, log_file, output_csv):
        self.log_file = log_file
        self.output_csv = output_csv
        self.processed_data = []
        
    def create_signal_template(self):
        """Create a template with ALL signals as columns"""
        signals = [
            'cell_voltage_1', 'cell_voltage_2', 'cell_voltage_3', 'cell_voltage_4',
            'cell_voltage_5', 'cell_voltage_6', 'cell_voltage_7', 'cell_voltage_8',
            'cell_voltage_9', 'cell_voltage_10', 'cell_voltage_11', 'cell_voltage_12',
            'cell_voltage_13', 'cell_voltage_14', 'cell_voltage_15', 'cell_voltage_16',
            'mincell_voltage', 'maxcell_voltage', 'pack_voltage', 'stack_voltage',
            'temp_1', 'temp_2', 'temp_3', 'temp_4', 'temp_5', 'temp_6', 'temp_7', 'temp_8',
            'OCC', 'OCD', 'OCC_AFE', 'OCD_AFE', 'SCD', 'HW_SCD', 'OV', 'UV',
            'OV_AFE', 'UV_AFE', 'OTC', 'OTD', 'UTC', 'UTD', 'OTC_AFE', 'UTC_AFE', 
            'OTC_AFE', 'OTD_AFE', 'Afe_communication', 'Deep_discharge', 'Thermal_Runaway',
            'open_wire_detection', 'DFET_failure', 'CFET_failure', 'Reserved1', 'Reserved2',
            'Reserved3', 'Reserved4', 'Reserved5', 'SoC', 'Cell_balancing_Status', 'Pack_cuurent',
            'SoH', 'max_cell_temp', 'min_cell_temp', 'SOP', 'SW_Version', 'DataProccessing_taskPeriod',
            'DataProccessing_PeakStackUsage', 'StateProccessing_taskPeriod', 'StateProccessing_PeakStackUsage',
            'SoC_TaskPeriod', 'SoC_PeakStackUsage', 'WDG_TaskPeriod', 'WDG_PeakStackUsage',
            'Reset_counter_value', 'AFE_Status', 'Reserved6', 'Reserved7'
        ]
        return {signal: 0 for signal in signals}, signals
    
    def parse_log_line(self, line):
        """Parse a single line from the log file"""
        pattern = r'(\d+:\d+:\d+:\d+)\s+([RT]x)\s+(\d+)\s+(0x[0-9A-F]+)\s+(\w+)\s+(\d+)\s+([0-9A-F\s]+)'
        match = re.match(pattern, line.strip())
        if match:
            timestamp, tx_rx, channel, can_id, msg_type, dlc, data_str = match.groups()
            return {
                'timestamp': timestamp,
                'tx_rx': tx_rx,
                'channel': int(channel),
                'can_id': int(can_id, 16),
                'msg_type': msg_type,
                'dlc': int(dlc),
                'data_bytes': bytes.fromhex(data_str.replace(' ', ''))
            }
        return None
    
    def process_can_message(self, message):
        """Process a single CAN message and extract signals"""
        can_id, data_bytes = message['can_id'], message['data_bytes']
        extracted = {}
        
        if len(data_bytes) >= 8:
            if can_id in [0x01, 0x02, 0x03, 0x04]:
                base_signal = (can_id - 0x01) * 4
                for i in range(4):
                    extracted[f'cell_voltage_{base_signal + i + 1}'] = int.from_bytes(data_bytes[i*2:i*2+2], 'little')
            
            elif can_id == 0x05:
                signals = ['mincell_voltage', 'maxcell_voltage', 'pack_voltage', 'stack_voltage']
                for i, sig in enumerate(signals):
                    extracted[sig] = int.from_bytes(data_bytes[i*2:i*2+2], 'little')
            
            elif can_id in [0x06, 0x07]:
                base_temp = (can_id - 0x06) * 4
                for i in range(4):
                    temp_val = int.from_bytes(data_bytes[i*2:i*2+2], 'little', signed=True)
                    extracted[f'temp_{base_temp + i + 1}'] = temp_val * 0.01
            
            elif can_id == 0x08 and len(data_bytes) >= 3:
                binary_signals = [
                    ['OCC', 'OCD', 'OCC_AFE', 'OCD_AFE', 'SCD', 'HW_SCD', 'OV', 'UV'],
                    ['OV_AFE', 'UV_AFE', 'OTC', 'OTD', 'UTC', 'UTD', 'OTC_AFE', 'UTC_AFE'],
                    ['OTC_AFE', 'OTD_AFE', 'Afe_communication', 'Deep_discharge', 'Thermal_Runaway', 'open_wire_detection', 'DFET_failure', 'CFET_failure']
                ]
                for byte_idx, signals in enumerate(binary_signals):
                    for bit_idx, sig in enumerate(signals):
                        extracted[sig] = (data_bytes[byte_idx] >> bit_idx) & 0x01
            
            elif can_id == 0x09:
                signals = ['SoC', 'Cell_balancing_Status', 'Pack_cuurent', 'SoH']
                for i, sig in enumerate(signals):
                    val = int.from_bytes(data_bytes[i*2:i*2+2], 'little', signed=(sig == 'Pack_cuurent'))
                    extracted[sig] = val
            
            elif can_id == 0x10:
                signals = ['max_cell_temp', 'min_cell_temp', 'SOP', 'SW_Version']
                for i, sig in enumerate(signals):
                    val = int.from_bytes(data_bytes[i*2:i*2+2], 'little', signed=(i < 2))
                    extracted[sig] = val
        
        if len(data_bytes) >= 8:
            task_signals = {
                0x11: ['DataProccessing_taskPeriod', 'DataProccessing_PeakStackUsage'],
                0x12: ['StateProccessing_taskPeriod', 'StateProccessing_PeakStackUsage'],
                0x13: ['SoC_TaskPeriod', 'SoC_PeakStackUsage'],
                0x14: ['WDG_TaskPeriod', 'WDG_PeakStackUsage']
            }
            if can_id in task_signals:
                signals = task_signals[can_id]
                extracted[signals[0]] = int.from_bytes(data_bytes[0:4], 'little')
                extracted[signals[1]] = int.from_bytes(data_bytes[4:8], 'little')
        
        if can_id == 0x15 and len(data_bytes) >= 4:
            extracted['Reset_counter_value'] = int.from_bytes(data_bytes[0:2], 'little')
            extracted['AFE_Status'] = int.from_bytes(data_bytes[2:4], 'little')
        
        return extracted
    
    def process_log_file(self):
        """Process the entire log file"""
        template, all_signals = self.create_signal_template()
        
        current_set, current_timestamp, processed_sets = template.copy(), None, []
        
        with open(self.log_file, 'r') as file:
            for line in file:
                if not line.strip() or line.startswith('***'): continue
                
                message = self.parse_log_line(line)
                if message and message['tx_rx'] == 'Rx':
                    can_id = message['can_id']
                    
                    if can_id == 0x01 and current_timestamp:
                        current_set['timestamp'] = current_timestamp
                        processed_sets.append(current_set.copy())
                        current_set = template.copy()
                    
                    extracted = self.process_can_message(message)
                    current_set.update(extracted)
                    current_timestamp = message['timestamp']
        
        if current_timestamp:
            current_set['timestamp'] = current_timestamp
            processed_sets.append(current_set)
        
        self.processed_data = processed_sets
    
    def save_to_csv(self):
        """Save processed data to CSV file"""
        if not self.processed_data: return
        
        df = pd.DataFrame(self.processed_data)
        template, all_signals = self.create_signal_template()
        
        for signal in all_signals:
            if signal not in df.columns:
                df[signal] = 0
        
        df = df[['timestamp'] + all_signals]
        df.to_csv(self.output_csv, index=False)
        
        print(f"SUCCESS: {len(df)} records with {len(df.columns)-1} signals saved to {self.output_csv}")

def main():
    LOG_FILE = r"D:\log_file_data\24_hr_regressive_testing_01.log"
    OUTPUT_CSV = r"D:\log_file_data\BMS_FINAL_CORRECT.csv"
    
    if not os.path.exists(LOG_FILE):
        print(f"Error: File not found - {LOG_FILE}")
        return
    
    processor = CANLogProcessor(LOG_FILE, OUTPUT_CSV)
    processor.process_log_file()
    processor.save_to_csv()

if __name__ == "__main__":
    main()