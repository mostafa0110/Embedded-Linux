import socket

HOST = "127.0.0.1" 
PORT = 5000

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((HOST, PORT))
s.listen(1)
print(f"Server listening on {HOST}:{PORT}")

conn, addr = s.accept()
print("Connected by", addr)

data = conn.recv(1024).decode()
a, op, b = data.split()
a = float(a)
b = float(b)

if op == '+':
    result = a + b
elif op == '-':
    result = a - b
elif op == '*':
    result = a * b
elif op == '/':
    result = a / b
else:
    result = "invalid operation"

print("Result:", result)

conn.close()
s.close()
