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
        self.processed_data = []
        
    def load_can_matrix(self):
        """Load CAN matrix from Excel file"""
        try:
            df = pd.read_excel(self.excel_file, sheet_name='V_2', header=None)
            self.can_matrix = df
            print("CAN matrix loaded successfully")
        except Exception as e:
            print(f"Error loading CAN matrix: {e}")
            raise
    
    def parse_can_matrix(self):
        """Parse CAN matrix to extract signal definitions"""
        signals = {}
        current_id = None
        
        for idx, row in self.can_matrix.iterrows():
            if pd.notna(row[1]) and isinstance(row[1], str) and row[1].startswith('0x'):
                current_id = int(row[1], 16)
                signals[current_id] = []
            elif current_id is not None and pd.notna(row[2]):
                signal_def = {
                    'byte_position': row[2],
                    'bit_position': row[3] if pd.notna(row[3]) else 0,
                    'signal_name': row[4] if pd.notna(row[4]) else '',
                    'data_type': row[5] if pd.notna(row[5]) else '',
                    'factor': float(row[6]) if pd.notna(row[6]) else 1.0,
                    'offset': float(row[7]) if pd.notna(row[7]) else 0.0,
                    'unit': row[8] if pd.notna(row[8]) else ''
                }
                # Clean signal name
                signal_def['signal_name'] = signal_def['signal_name'].strip().replace(' ', '_')
                signals[current_id].append(signal_def)
        
        return signals
    
    def extract_bytes_from_range(self, byte_range, data_bytes):
        """Extract bytes based on byte position range"""
        if '-' in str(byte_range):
            start, end = map(int, str(byte_range).split('-'))
            return data_bytes[start:end+1]
        else:
            pos = int(byte_range)
            return data_bytes[pos:pos+1]
    
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
    
    def convert_to_physical_value(self, raw_value, data_type, factor, offset):
        """Convert raw value to physical value based on data type and scaling"""
        try:
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
            elif data_type == 'BIN':
                value = raw_value[0] if len(raw_value) > 0 else 0
            else:
                value = int.from_bytes(raw_value, byteorder='little', signed=False)
            
            # Apply factor and offset
            physical_value = (value * factor) + offset
            
            # Check range and set to 0 if beyond valid range
            physical_value = self.check_value_range(physical_value, data_type)
            
            return round(physical_value, 4)
        except Exception as e:
            print(f"Error converting value: {e}")
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
    
    def process_can_message(self, message, signals):
        """Process a single CAN message and extract all signals"""
        can_id = message['can_id']
        data_bytes = message['data_bytes']
        
        if can_id not in signals:
            return None
        
        row_data = {}
        
        for signal_def in signals[can_id]:
            signal_name = signal_def['signal_name']
            if not signal_name or signal_name == 'Reserved':
                continue
                
            # Extract bytes for this signal
            byte_data = self.extract_bytes_from_range(signal_def['byte_position'], data_bytes)
            
            # Convert to physical value
            if signal_def['data_type'] == 'BIN' and signal_def['bit_position'] is not None:
                # Handle binary signals
                bit_pos = int(signal_def['bit_position'])
                byte_val = byte_data[0] if len(byte_data) > 0 else 0
                physical_value = (byte_val >> bit_pos) & 0x01
                # Check binary range
                physical_value = self.check_value_range(physical_value, 'BIN')
            else:
                # Handle numeric signals
                physical_value = self.convert_to_physical_value(
                    byte_data, 
                    signal_def['data_type'], 
                    signal_def['factor'], 
                    signal_def['offset']
                )
            
            row_data[signal_name] = physical_value
        
        return row_data
    
    def process_log_file(self):
        """Process the entire log file and group by complete sets of CAN IDs 0x01-0x15"""
        print("Loading CAN matrix...")
        self.load_can_matrix()
        signals = self.parse_can_matrix()
        
        print("Processing log file...")
        with open(self.log_file, 'r') as file:
            lines = file.readlines()
        
        # Define all expected CAN IDs (0x01 to 0x15)
        expected_ids = set(range(0x01, 0x16))
        
        # Let's first analyze the pattern in the log file
        can_id_sequence = []
        for line in lines:
            if not line.strip() or line.startswith('***'):
                continue
            message = self.parse_log_line(line)
            if message and message['tx_rx'] == 'Rx' and message['can_id'] in expected_ids:
                can_id_sequence.append(message['can_id'])
        
        print(f"CAN IDs found in sequence: {[hex(x) for x in can_id_sequence[:30]]}...")
        
        # Process messages with flexible grouping
        current_set = {}
        current_timestamp = None
        line_count = 0
        complete_sets = 0
        
        for line in lines:
            if not line.strip() or line.startswith('***'):
                continue
            
            message = self.parse_log_line(line)
            if message and message['tx_rx'] == 'Rx' and message['can_id'] in expected_ids:
                can_id = message['can_id']
                
                # Process the message
                processed_message = self.process_can_message(message, signals)
                if processed_message:
                    # If we encounter CAN ID 0x01, start a new set
                    if can_id == 0x01:
                        # If we have a previous incomplete set, save it anyway
                        if current_set:
                            combined_row = {'timestamp': current_timestamp}
                            for can_id in sorted(current_set.keys()):
                                combined_row.update(current_set[can_id])
                            self.processed_data.append(combined_row)
                            complete_sets += 1
                        
                        # Start new set
                        current_set = {can_id: processed_message}
                        current_timestamp = message['timestamp']
                    else:
                        # Add to current set
                        current_set[can_id] = processed_message
                        if current_timestamp is None:
                            current_timestamp = message['timestamp']
                    
                    line_count += 1
                    
                    # If we reach CAN ID 0x15, complete the current set
                    if can_id == 0x15 and current_set:
                        combined_row = {'timestamp': current_timestamp}
                        for can_id in sorted(current_set.keys()):
                            combined_row.update(current_set[can_id])
                        self.processed_data.append(combined_row)
                        complete_sets += 1
                        
                        # Reset for next set
                        current_set = {}
                        current_timestamp = None
        
        # Process any remaining incomplete set at the end
        if current_set:
            combined_row = {'timestamp': current_timestamp}
            for can_id in sorted(current_set.keys()):
                combined_row.update(current_set[can_id])
            self.processed_data.append(combined_row)
            complete_sets += 1
        
        print(f"Processed {line_count} CAN messages")
        print(f"Complete sets found: {complete_sets}")
        
        # Print what CAN IDs we actually found
        found_ids = set()
        for line in lines:
            message = self.parse_log_line(line)
            if message and message['tx_rx'] == 'Rx':
                found_ids.add(message['can_id'])
        
        print(f"CAN IDs found in log: {sorted([hex(x) for x in found_ids if x in expected_ids])}")
        
        # Print formatted table
        print("\n" + "="*100)
        print("PROCESSED CAN DATA - Grouped Sets")
        print("="*100)
        self.print_formatted_table(self.processed_data)
    
    def print_formatted_table(self, data):
        """Print formatted table in terminal"""
        if not data:
            print("No data sets found")
            return
        
        # Get all column names
        all_columns = set()
        for row in data:
            all_columns.update(row.keys())
        
        # Remove timestamp for signal display
        base_columns = {'timestamp'}
        signal_columns = sorted([col for col in all_columns if col not in base_columns])
        
        # Define column widths
        col_widths = {
            'timestamp': 15
        }
        for col in signal_columns:
            col_widths[col] = max(len(col), 10)
        
        # Print header
        header = "Timestamp".ljust(col_widths['timestamp']) + " | "
        for col in signal_columns[:10]:  # Show first 10 signals to avoid overflow
            header += col.ljust(col_widths[col]) + " | "
        if len(signal_columns) > 10:
            header += f"... (+{len(signal_columns)-10} more)"
        print(header)
        
        # Print separator
        separator = "-" * col_widths['timestamp'] + "-+-"
        for col in signal_columns[:10]:
            separator += "-" * col_widths[col] + "-+-"
        print(separator)
        
        # Print data rows
        for row in data[:10]:  # Show first 10 rows
            line = row['timestamp'].ljust(col_widths['timestamp']) + " | "
            for col in signal_columns[:10]:
                value = row.get(col, '')
                if isinstance(value, float):
                    value_str = f"{value:.4f}"
                else:
                    value_str = str(value)
                line += value_str.ljust(col_widths[col]) + " | "
            print(line)
        
        if len(data) > 10:
            print(f"\n... and {len(data) - 10} more rows")
    
    def save_to_csv(self):
        """Save processed data to CSV file"""
        if not self.processed_data:
            print("No data to save")
            return
        
        # Create DataFrame
        df = pd.DataFrame(self.processed_data)
        
        # Reorder columns for better readability
        base_columns = ['timestamp']
        signal_columns = [col for col in df.columns if col not in base_columns]
        df = df[base_columns + sorted(signal_columns)]
        
        # Save to CSV
        df.to_csv(self.output_csv, index=False)
        print(f"\nData saved to {self.output_csv}")
        print(f"Total records: {len(df)}")
        print(f"Columns: {len(df.columns)} total")
        
        # Show sample of columns
        print(f"Sample columns: {list(df.columns[:15])}...")
    
    def generate_summary(self):
        """Generate a summary of the processed data"""
        if not self.processed_data:
            print("No data available for summary")
            return
        
        df = pd.DataFrame(self.processed_data)
        print("\n" + "="*60)
        print("PROCESSING SUMMARY")
        print("="*60)
        print(f"Total data sets: {len(df)}")
        print(f"Time range: {df['timestamp'].min()} to {df['timestamp'].max()}")
        
        # Show signal statistics
        signal_columns = [col for col in df.columns if col not in ['timestamp']]
        print(f"\nSignals extracted: {len(signal_columns)}")

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
        processor.generate_summary()
        
    except Exception as e:
        print(f"Error during processing: {e}")

if __name__ == "__main__":
    main()