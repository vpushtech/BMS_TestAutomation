import pandas as pd

excel_path = r"C:\Users\PandurangaKarapothul\Desktop\python-workspace\VPUSH_TECH_BMS_CAN_MATRIX.xlsx"
dbc_path = r"C:\Users\PandurangaKarapothul\Desktop\python-workspace\VPUSH_TECH_BMS_CAN_MATRIX.dbc"
sender = "BMS"

df = pd.read_excel(excel_path, sheet_name='V_4', header=None)

header_row = df.index[df[1] == "STD ID"][0]
data = df.iloc[header_row + 2:].reset_index(drop=True)

with open(dbc_path, "w") as f:

    f.write('VERSION "Generated from Excel"\n\n')
    f.write("NS_ :\n\n")
    f.write("BS_:\n\n")
    f.write(f"BU_: {sender}\n\n")

    current_id = None

    for _, row in data.iterrows():

        can_id = row[1]
        bit_pos = row[3]
        sig_name = row[4]
        dtype = row[5]
        factor = row[6]
        offset = row[7]
        unit = row[8]

        if pd.isna(sig_name):
            continue

        if pd.notna(can_id):
            msg_id = int(str(can_id), 16)
            f.write(f"\nBO_ {msg_id} MSG_{msg_id:03X}: 8 {sender}\n")
            current_id = msg_id

        start_bit = int(bit_pos)

        if dtype == "U8":
            size, sign = 8, "+"
            min, max = 0, 255
        elif dtype == "U16":
            size, sign = 16, "+"
            min, max = 0, 65535
        elif dtype == "I16":
            size, sign = 16, "-"
            min, max = -32768, 32767
        elif dtype == "U32":
            size, sign = 32, "+"
            min, max = 0, 4294967295
        elif dtype == "I32":
            size, sign = 32, "-"
            min, max = -2147483648, 2147483647
        elif dtype == "F64":
            size, sign = 64, "-"
            min, max =  -9223372036854775808, 9223372036854775807
        elif dtype == "BIN":
            size, sign = 1,  "+"
            min, max = 0,1
        else:
            size, sign = 8, "+"
            min, max = 0, 0

        factor = factor if pd.notna(factor) else 1
        offset = offset if pd.notna(offset) else 0
        unit = unit if pd.notna(unit) else ""

        f.write(
            f" SG_ {sig_name} : "
            f"{start_bit}|{size}@1{sign} "
            f"({factor},{offset}) "
            f"[{min}|{max}] "
            f"\"{unit}\" "
            f"Vector__XXX\n"
        )

    f.write("\nBA_DEF_ \"BusType\" STRING ;\n")
    f.write("BA_DEF_DEF_ \"BusType\" \"CAN\";\n")

print("✅ DBC file generated successfully:", dbc_path)
