import socket

HOST = "127.0.0.1" 
PORT = 5000

a = input("Enter first number: ")
op = input("Enter operation (+ - * /): ")
b = input("Enter second number: ")

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))
s.send(f"{a} {op} {b}".encode())

data = s.recv(1024).decode()

s.close()
