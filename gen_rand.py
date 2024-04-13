import random

def generate_address():
    return hex(random.randint(0, 2**32 - 1))

def generate_trace_file(filename, num_operations):
    with open(filename, 'w') as file:
        for _ in range(num_operations):
            operation = random.choice(['l', 's']) 
            address = generate_address()  
            address = str(address).upper()
            n = len(address)
            for i in range (n,10):
                address=address[:2]+'0'+address[2:]
            size = random.randint(1, 8)  
            file.write(f"{operation} {address} {size}\n")

generate_trace_file('sample.txt', 100)