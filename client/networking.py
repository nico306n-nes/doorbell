import requests
import json
import sys
import socket

class Site:
    site = "http://klokke.local"
    timeout = 5
    reason = None
    
    def __init__(self, site="klokke.local"):
        self.site = "http://" + site
        
    def check_connection(self, timeout=5):
        self.timeout = timeout

        try:
            requests.get(self.site, timeout=self.timeout)
            return True
        except requests.exceptions.Timeout:
            self.reason = "Timed out. Domain name is reqistered but couldnt reach site. Possibly error on esp"
            return False
        except requests.exceptions.ConnectionError:
            self.reason = "Connection error. Possibly wrong address, esp is not online or mdns is down for some reason"
            return False
        
    
    def get_reason(self):
        return self.reason
    
    def get_site(self, sub):
        
        with requests.get(self.site + sub, stream=False) as response:
            return response
        
        
        
    
    def get_diff(self):
        return json.loads(self.get_site("/klokke").text)["diff"]
    
    def get_status(self):
        return self.get_site().status_code

def attempt_connection(ips, debug=False):
    '''
    for ip in ips:
        try:
            response = requests.get("http://" + ip, timeout=10)
            if response.status_code == 200:
                return ip
        except requests.exceptions.ConnectionError:
            if debug == True:
                print("connection error")'''
        
        
    acquired_ip = acquire_ip()
    with requests.get("http://" + acquired_ip, stream=False, timeout=30):
        return acquired_ip
    
    return acquired_ip
    

def acquire_ip():
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
    sock.bind(('', 30499))

    while True:
        # Receive message from the ESP8266 broadcast
        data, addr = sock.recvfrom(1024)
        print(f"addr: {addr}, data: {data}")
        
        if data.decode()[0:6] == "unique":
            return addr[0]


if __name__ == "__main__":
    site = "klokke.local"
    klokke = Site(site)

    if klokke.check_connection() == True:
        print("Reached " + site + "!")
    else:
        print("Could not reach " + site)
        print("Caused by: " + str(klokke.get_reason()))
    