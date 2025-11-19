import pandas as pd
import struct
import re
from datetime import datetime
import os
from collections import defaultdict

class CANLogProcessor:
    def __init__(self, excel_file, log_file, output_csv):
        self.excel_file = excel_file
        self.log_file = log_file
        self.output_csv = output_csv
        self.can_matrix = None
        self.signals_dict = {}
        self.processed_data = []
        
    def load_and_parse_can_matrix(self):
        """Load and parse CAN matrix from Excel file"""
        try:
            df = pd.read_excel(self.excel_file, sheet_name='V_2', header=None)
            self.can_matrix = df
            print("CAN matrix loaded successfully")
            
            # Parse the CAN matrix to extract signal definitions
            self.parse_can_matrix_to_dict()
            
        except Exception as e:
            print(f"Error loading CAN matrix: {e}")
            raise
    
    def parse_can_matrix_to_dict(self):
        """Parse CAN matrix and create signals dictionary"""
        signals = {}
        current_id = None
        
        for idx, row in self.can_matrix.iterrows():
            # Check if this row contains a CAN ID
            if pd.notna(row[1]) and isinstance(row[1], str) and row[1].startswith('0x'):
                current_id = int(row[1], 16)
                signals[current_id] = []
                
            elif current_id is not None and pd.notna(row[4]) and row[4] != '':
                # Extract signal definition
                signal_def = {
                    'byte_position': row[2] if pd.notna(row[2]) else '',
                    'bit_position': row[3] if pd.notna(row[3]) else 0,
                    'signal_name': row[4] if pd.notna(row[4]) else '',
                    'data_type': row[5] if pd.notna(row[5]) else '',
                    'factor': float(row[6]) if pd.notna(row[6]) else 1.0,
                    'offset': float(row[7]) if pd.notna(row[7]) else 0.0,
                    'unit': row[8] if pd.notna(row[8]) else ''
                }
                
                # Clean signal name
                signal_name = signal_def['signal_name'].strip()
                signal_name = signal_name.replace(' ', '_')
                signal_name = signal_name.replace('__', '_')
                signal_def['signal_name'] = signal_name
                
                # Add ALL signals including Reserved
                if signal_name and signal_name != '':
                    signals[current_id].append(signal_def)
        
        self.signals_dict = signals
        print(f"Parsed {len(signals)} CAN IDs from matrix")
        
        # Debug: Print signals for each CAN ID
        for can_id in sorted(signals.keys()):
            print(f"CAN ID 0x{can_id:02X}: {len(signals[can_id])} signals")
            for sig in signals[can_id]:
                print(f"  - {sig['signal_name']}: {sig['data_type']}, bytes: {sig['byte_position']}, factor: {sig['factor']}, offset: {sig['offset']}")
        
    def create_complete_signal_list(self):
        """Create a complete list of ALL signals including Reserved"""
        all_signals = []
        
        # First add all non-Reserved signals
        for can_id, signal_list in self.signals_dict.items():
            for signal_def in signal_list:
                signal_name = signal_def['signal_name']
                if signal_name not in all_signals:
                    all_signals.append(signal_name)
        
        # Ensure we have exactly the signals you specified
        final_signals = [
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
        
        return final_signals
    
    def create_signal_template(self):
        """Create a template with ALL signals as columns"""
        all_signals = self.create_complete_signal_list()
        
        # Create empty template with all signals
        template = {signal: 0 for signal in all_signals}
        return template, all_signals
    
    def extract_bytes_from_range(self, byte_range, data_bytes):
        """Extract bytes based on byte position range with boundary checking"""
        try:
            if isinstance(byte_range, str) and '-' in byte_range:
                start, end = map(int, byte_range.split('-'))
                if start < len(data_bytes) and end < len(data_bytes):
                    return data_bytes[start:end+1]
                else:
                    return bytes([])
            elif byte_range is not None:
                pos = int(byte_range)
                if pos < len(data_bytes):
                    return bytes([data_bytes[pos]])
                else:
                    return bytes([])
            else:
                return bytes([])
        except Exception as e:
            return bytes([])
    
    def check_value_range(self, value, data_type):
        """Check if value is within valid range for its data type"""
        try:
            if data_type == 'U16':
                if value < 0 or value > 65535:
                    return 0
            elif data_type == 'I16':
                if value < -32768 or value > 32767:
                    return 0
            elif data_type == 'U32':
                if value < 0 or value > 4294967295:
                    return 0
            elif data_type == 'I32':
                if value < -2147483648 or value > 2147483647:
                    return 0
            elif data_type == 'U8':
                if value < 0 or value > 255:
                    return 0
            elif data_type == 'BIN':
                if value not in [0, 1]:
                    return 0
            return value
        except:
            return 0
    
    def convert_to_physical_value(self, raw_value, data_type, factor, offset, bit_position=None):
        """Convert raw value to physical value based on data type and scaling"""
        try:
            if len(raw_value) == 0:
                return 0
                
            if data_type == 'BIN':
                if bit_position is not None and len(raw_value) > 0:
                    byte_val = raw_value[0] if isinstance(raw_value, bytes) else raw_value
                    value = (byte_val >> int(bit_position)) & 0x01
                else:
                    value = 0
            else:
                if data_type == 'U16':
                    value = int.from_bytes(raw_value, byteorder='little', signed=False)
                elif data_type == 'I16':
                    value = int.from_bytes(raw_value, byteorder='little', signed=True)
                elif data_type == 'U32':
                    value = int.from_bytes(raw_value, byteorder='little', signed=False)
                elif data_type == 'I32':
                    value = int.from_bytes(raw_value, byteorder='little', signed=True)
                elif data_type == 'U8':
                    value = raw_value[0] if len(raw_value) > 0 else 0
                else:
                    value = int.from_bytes(raw_value, byteorder='little', signed=False)
            
            physical_value = (value * factor) + offset
            physical_value = self.check_value_range(physical_value, data_type)
            return round(physical_value, 4)
        except Exception as e:
            print(f"Error converting value: {e}, data_type: {data_type}, factor: {factor}, offset: {offset}")
            return 0
    
    def parse_log_line(self, line):
        """Parse a single line from the log file"""
        pattern = r'(\d+:\d+:\d+:\d+)\s+([RT]x)\s+(\d+)\s+(0x[0-9A-F]+)\s+(\w+)\s+(\d+)\s+([0-9A-F\s]+)'
        match = re.match(pattern, line.strip())
        
        if match:
            timestamp, tx_rx, channel, can_id, msg_type, dlc, data_str = match.groups()
            can_id_int = int(can_id, 16)
            data_bytes = bytes.fromhex(data_str.replace(' ', ''))
            
            return {
                'timestamp': timestamp,
                'tx_rx': tx_rx,
                'channel': int(channel),
                'can_id': can_id_int,
                'msg_type': msg_type,
                'dlc': int(dlc),
                'data_bytes': data_bytes
            }
        return None
    
    def process_can_message(self, message):
        """Process a single CAN message using signals from CAN matrix"""
        can_id = message['can_id']
        data_bytes = message['data_bytes']
        
        extracted_signals = {}
        
        if can_id not in self.signals_dict:
            return extracted_signals
        
        print(f"Processing CAN ID 0x{can_id:02X} with {len(data_bytes)} bytes")
        
        # Process each signal for this CAN ID
        for signal_def in self.signals_dict[can_id]:
            signal_name = signal_def['signal_name']
            
            # Skip Reserved signals (we'll handle them separately)
            if 'Reserved' in signal_name:
                continue
                
            # Extract bytes for this signal with boundary checking
            byte_data = self.extract_bytes_from_range(signal_def['byte_position'], data_bytes)
            
            # Convert to physical value using parameters from CAN matrix
            physical_value = self.convert_to_physical_value(
                byte_data, 
                signal_def['data_type'], 
                signal_def['factor'], 
                signal_def['offset'],
                signal_def['bit_position'] if signal_def['data_type'] == 'BIN' else None
            )
            
            extracted_signals[signal_name] = physical_value
            
            # Debug output for critical signals
            if signal_name in ['Reset_counter_value', 'temp_1', 'temp_2', 'DataProccessing_taskPeriod', 'StateProccessing_taskPeriod', 'SoC_TaskPeriod', 'WDG_TaskPeriod']:
                print(f"  {signal_name}: {physical_value} (bytes: {signal_def['byte_position']}, data_type: {signal_def['data_type']}, factor: {signal_def['factor']}, offset: {signal_def['offset']})")
        
        return extracted_signals
    
    def process_log_file(self):
        """Process the entire log file"""
        print("Loading and parsing CAN matrix...")
        self.load_and_parse_can_matrix()
        
        print("Creating signal template...")
        template, all_signals = self.create_signal_template()
        
        print(f"Total signals in template: {len(all_signals)}")
        
        print("Processing log file...")
        with open(self.log_file, 'r') as file:
            lines = file.readlines()
        
        # Create empty template for current set
        current_set = template.copy()
        current_timestamp = None
        processed_sets = []
        
        for line in lines:
            if not line.strip() or line.startswith('***'):
                continue
            
            message = self.parse_log_line(line)
            if message and message['tx_rx'] == 'Rx':
                can_id = message['can_id']
                
                # Process the CAN message using CAN matrix definitions
                extracted_signals = self.process_can_message(message)
                
                # If this is CAN ID 0x01, save previous set and start new one
                if can_id == 0x01 and current_timestamp is not None:
                    # Save current set
                    current_set['timestamp'] = current_timestamp
                    processed_sets.append(current_set.copy())
                    
                    # Start new set
                    current_set = template.copy()
                
                # Update current set with new data
                for signal, value in extracted_signals.items():
                    current_set[signal] = value
                
                # Update timestamp
                current_timestamp = message['timestamp']
        
        # Add the last set
        if current_timestamp is not None:
            current_set['timestamp'] = current_timestamp
            processed_sets.append(current_set)
        
        self.processed_data = processed_sets
        print(f"Complete data sets found: {len(self.processed_data)}")
        
        # Verify all signals are present
        if self.processed_data:
            first_row = self.processed_data[0]
            missing_in_output = [sig for sig in all_signals if sig not in first_row]
            if missing_in_output:
                print(f"WARNING: {len(missing_in_output)} signals missing in output")
            else:
                print("SUCCESS: All template signals are present in output!")
    
    def save_to_csv(self):
        """Save processed data to CSV file - GUARANTEE all signals are present"""
        if not self.processed_data:
            print("No data to save")
            return
        
        # Get the complete signal list
        template, all_signals = self.create_signal_template()
        
        # Create DataFrame with ALL signals
        df = pd.DataFrame(self.processed_data)
        
        # Ensure ALL signals are in the DataFrame
        for signal in all_signals:
            if signal not in df.columns:
                df[signal] = 0  # Fill missing signals with 0
        
        # Reorder columns: timestamp first, then all signals in exact order
        base_columns = ['timestamp']
        df_columns = base_columns + all_signals
        
        # Only keep columns that are in our defined list
        final_columns = [col for col in df_columns if col in df.columns]
        df = df[final_columns]
        
        # Add any missing columns with default value 0
        for col in all_signals:
            if col not in df.columns:
                df[col] = 0
        
        # Final column order
        df = df[base_columns + all_signals]
        
        # Save to CSV
        df.to_csv(self.output_csv, index=False)
        print(f"\nData saved to {self.output_csv}")
        print(f"Total records: {len(df)}")
        print(f"Total columns (signals): {len(df.columns)}")
        
        # Final verification
        csv_columns = set(df.columns) - {'timestamp'}
        template_columns = set(all_signals)
        
        missing_final = template_columns - csv_columns
        if missing_final:
            print(f"CRITICAL ERROR: {len(missing_final)} signals missing in final CSV:")
            for sig in sorted(missing_final):
                print(f"  - {sig}")
        else:
            print("SUCCESS: All signals are present in final CSV!")
            
        # Show sample values for critical signals
        print(f"\nSample values from first record:")
        critical_signals = [
            'Reset_counter_value', 'temp_1', 'temp_2', 'temp_3', 'temp_4', 
            'temp_5', 'temp_6', 'temp_7', 'temp_8',
            'DataProccessing_taskPeriod', 'StateProccessing_taskPeriod', 
            'SoC_TaskPeriod', 'WDG_TaskPeriod'
        ]
        for signal in critical_signals:
            if signal in df.columns:
                value = df[signal].iloc[0] if len(df) > 0 else 'N/A'
                print(f"  {signal}: {value}")

def main():
    # File paths
    EXCEL_FILE = r"D:\log_file_data\VPUSH_TECH_BMS_CAN_MATRIX.xlsx"
    LOG_FILE = r"D:\log_file_data\24_hr_regressive_testing_01.log"
    OUTPUT_CSV = r"D:\log_file_data\BMS_FINAL_CORRECT.csv"
    
    # Check if files exist
    for file_path in [EXCEL_FILE, LOG_FILE]:
        if not os.path.exists(file_path):
            print(f"Error: File not found - {file_path}")
            return
    
    # Process the data
    processor = CANLogProcessor(EXCEL_FILE, LOG_FILE, OUTPUT_CSV)
    
    try:
        processor.process_log_file()
        processor.save_to_csv()
        
    except Exception as e:
        print(f"Error during processing: {e}")

if __name__ == "__main__":
    main()