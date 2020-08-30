import socket
import sys
import ssl
import threading
import os
import time
import json

os.system("clear")
print("Using ServerEngine system for linux systems")
time.sleep(3)


class KeyPairContainer:
	def __init__(self):
		os.system("clear")
		print("checking if keystore dirs exist")

		if os.path.isdir("/keystore/") == False:
			print("dir dosent exist generating keystore dir press enter to continue")
			tmp = input("")
			os.system("mkdir /keystore")
			print("dir's made")
		else:
			print("dir exists")

	def RunKeyPair(self, keypair_name):
		print("")
		if os.path.isdir("/keystore/" + keypair_name) == False:
			print("This keypair dosen't exist do you want to generate this as a new keypair[y/n]")
			genkey = str(input("-> "))
			if genkey == "y":
				os.system("mkdir /keystore/" + keypair_name)
				os.system("./gen_keys.sh")
				os.system("cp cert.pem /keystore/" + keypair_name + "/" + "cert.pem")
				os.system("rm cert.pem")
				os.system("cp key.pem /keystore/" + keypair_name + "/" + "key.pem")
				os.system("rm key.pem")
				return "/keystore/" + keypair_name + "/"
			else:
				quit()
		else:
			return "/keystore/" + keypair_name + "/"


class User:
	def __init__(self):
		pass

class UserSys:
	def __init__(self, proj_name):
		os.system("clear")
		print("<User system init>")
		print("checking if usersys folder exists")

		self.USERS_PATH = "/server_users/" + proj_name + "/" + "users.json"
		self.user_list = []

		if os.path.isdir("/server_users/") == False:
			print("The necessary files don't exist do pess enter to generate them")
			tmp = input("")
			os.system("mkdir /server_users/")

		if os.path.isdir("/server_users/" + proj_name + "/") == False:
			print("This project dosen't exist")
			print("Press enter to generate this project")
			tmp = input("")
			os.system("mkdir /server_users/" + proj_name)
			print("creating users file")

			try:
				file = open(self.USERS_PATH, "w")
				tdict = {}
				tjobject = json.dumps(tdict, indent = 4)
				file.write(tjobject)
				file.close()

			except:
				print("error couldn't create file")
				time.sleep(4)

	def PreThread(self, selfobject, conn, addr):
		try:
			users = open(self.USERS_PATH, "r")
			ujdata = json.load(users)
			users.close()

			try:
				print("blacklisted: ", ujdata[addr[0]]["blacklisted"])

			except:
				#write data
				print("Registering new User")
				os.system("rm " + self.USERS_PATH)
				try:
					wtusers = open(self.USERS_PATH, "w")
					ujdata[addr[0]] = {}
					ujdata[addr[0]]["ip"] = addr[0]
					ujdata[addr[0]]["blacklisted"] = False
					tjobject = json.dumps(ujdata, indent = 4)
					wtusers.write(tjobject)
					wtusers.close()
				except:
					print("couldn't write data to file")

		except:
			print("failed opening json data")

class NewServer:
	def __init__(self, host, port, runfunc, keypath, multi_threaded):
		os.system("clear")

		print("<init sequence begining>")
		self.host = host
		print("host registerd")
		self.port = port
		print("port registerd")
		self.runfunc = runfunc
		print("runfunc registerd")

		self.HMaxthread = False
		self.Maxthreadc = 0
		print("thread settings have been registerd")

		self.HPrethread_bootup = False
		self.Prethread_bootupfunc = None
		print("Pre-thread bootup settings have been registerd")

		self.multi_threaded = multi_threaded
		print("multi threading setting has been registerd")

		#packet registration
		self.close_packet = -111
		#packet registartion
		print("packets have been registered")

		self.thread_count = 0

		self.keypath = keypath

		self.context = ssl.SSLContext(ssl.PROTOCOL_TLS_SERVER)
		self.context.load_cert_chain(self.keypath + "cert.pem",self.keypath + "key.pem")
		print("ssl context created")
		self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0)
		print("socket created")
		print("")
		print("")

	def show_settings(self):
		print("Do you want to be shown optional settings[y/n]")
		sops = str(input("-> "))

		print("")
		print("<----------------------- Settings ----------------------->")
		print("host registerd as: ", self.host)
		print("port registerd as: ", self.port)
		print("multi_threading registerd as: ", self.multi_threaded)
		print("")
		if sops != "n":
			print("Has max threads: ", self.HMaxthread)
			print("Max thread count: ", self.Maxthreadc)
			print("Has Pre-thread bootup: ", self.HPrethread_bootup)

		print("<-------------------------------------------------------->")
		print("")
		print("")

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
		self.thread_count -= 1
		conn.close()

	def end_thread(self, conn, addr):
		conn.close()

	def errored(self, recv_data):
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
		self.show_settings()

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

			if self.multi_threaded == True:
				try:
					if self.HPrethread_bootup == True:
						self.Prethread_bootupfunc(self, conn, addr)

					else:
						print("")

					if self.HMaxthread == True and self.Maxthreadc == self.thread_count:
						tthread = threading.Thread(target=self.end_thread, args=(conn, addr,))
						tthread.start()
					else:
						tthread = threading.Thread(target=self.client_thread, args=(conn, addr, self.runfunc,))
						tthread.start()
						self.thread_count += 1
				except:
					print("unable to start thread")
			else:
				self.runfunc(self.Get(), conn, addr)
				conn.close()
				break


		self.wrapedsocket.close()

