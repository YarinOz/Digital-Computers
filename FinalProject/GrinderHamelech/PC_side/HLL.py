# Instruction mapping with their corresponding opcode
instruction_map = {
    "inc_lcd": "01",
    "dec_lcd": "02",
    "rra_lcd": "03",
    "set_delay": "04",
    "clear_lcd": "05",
    "stepper_deg": "06",
    "stepper_scan": "07",
    "sleep": "08"
}

def translate_line(line):
    parts = line.strip().split()
    instruction = parts[0]
    operands = parts[1:]  # Capture operands after the instruction

    # Fetch the opcode for the instruction
    opcode = instruction_map[instruction]

    # Handle the specific operand requirements
    if instruction == "rra_lcd" and operands:
        operand = f'{ord(operands[0]):02X}'  # Convert to uppercase
    elif instruction == "stepper_scan" and operands:
        # Split the operands on the comma to get two separate values
        left_operand, right_operand = operands[0].split(',')
        operand = f'{int(left_operand):02X}{int(right_operand):02X}'  # Convert to uppercase
    elif operands:
        operand = f'{int(operands[0]):02X}'  # Convert to uppercase
    else:
        operand = ""

    # Combine opcode and operand
    return opcode + operand

def translate_script(input_file, output_file):
    with open(input_file, 'r') as infile, open(output_file, 'w') as outfile:
        for line in infile:
            encoded_line = translate_line(line)
            outfile.write(encoded_line + '\n')

# Usage example
translate_script('script_data.txt', 'script.txt')
