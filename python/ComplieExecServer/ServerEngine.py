import socket
import sys
import ssl
import threading
import os

class NewServer:
	def __init__(self, host, port, runfunc, keypath):
		os.system("clear")

		self.host = host
		print("host registerd as: ", host)
		self.port = port
		print("port registerd as: ", port)
		self.runfunc = runfunc
		print("runfunc registerd")

		#packet registration
		self.close_packet = -111
		#packet registartion
		print("packets have been registered")

		self.thread_count = 0

		print("")
		print("")
		print("Have you generated keys[y/n]")
		haskeys = str(input("-> "))
		print("")
		print("")

		self.keypath = keypath
		if haskeys != "y":
			print("generating cert.pem and key.pem")
			print("---------------")

			os.system("./gen_keys.sh")

			print("--------------")
			print("Move these key's to a dir and make last var equal that path")
			self.keypath = ""
			print("")


		self.context = ssl.SSLContext(ssl.PROTOCOL_TLS_SERVER)
		self.context.load_cert_chain(self.keypath + "cert.pem",self.keypath + "key.pem")
		print("ssl context created")
		self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0)
		print("socket created")

	def send(self, data, conn):
		conn.send(data.encode())

	def recv(self, conn, israw):
		if israw == True:
			recv_data = conn.recv(1024)
			return recv_data
		if israw == False:
			recv_data = conn.recv(1024)
			return recv_data.decode()

	def Get(self):
		return self

	def client_thread(self, conn, addr, runfunc):
		runfunc(self.Get(), conn, addr)
		conn.close()

	def base_thread(self, conn, addr):
		recv_data = self.recv(conn, False)
		if recv_data == self.close_packet:
			pass

		if not recv_data:
			pass

	def recv_file(self, filename, conn):
		file = open(filename, 'w+')
		file_data = conn.recv(1024)
		file.write(file_data.decode())
		file.close()

	def send_file(self, filename, conn):
		try:
			file = open(filename, 'w+')
			file_data = file.read(1024)
			conn.send(file_data.encode())
			file.close()
		except:
			print("File dosent exist")
			pass


	def start_server(self):
		print("")
		print("")

		print("------------------------------")
		print("starting server")
		print("------------------------------")

		print("")
		self.server_socket.bind((self.host, self.port))
		print("binded host to port")
		self.server_socket.listen(5)
		self.wrapedsocket = self.context.wrap_socket(self.server_socket, server_side=True)
		print("created ssl wraped socket and listening for client ...")
		while True:
			conn, addr = self.wrapedsocket.accept()
			print("connected with " + addr[0] + ":" + str(addr[1]))

			try:
				tthread = threading.Thread(target=self.client_thread, args=(conn, addr, self.runfunc,))
				tthread.start()
				self.thread_count += 1
				print(self.thread_count)
			except:
				print("unable to start thread")

