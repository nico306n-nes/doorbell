import requests
import json
import sys

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
        try:
            return requests.get(self.site + sub)
        except:
            sys.exit()
        
    
    def get_diff(self):
        return json.loads(self.get_site("/klokke").text)["diff"]
    
    def get_status(self):
        return self.get_site().status_code

def attempt_connection(ips, debug=False):
    for ip in ips:
        klokke_ip = ip
        klokke = Site(klokke_ip)
        if klokke.check_connection() == True:
            if debug == True:
                print("Reached site at " + klokke_ip + "!")
            
            return klokke
        else:
            if debug == True:
                print("Could not reach " + klokke_ip)
                print("Caused by: " + str(klokke.get_reason()))

    return False

if __name__ == "__main__":
    site = "klokke.local"
    klokke = Site(site)

    if klokke.check_connection() == True:
        print("Reached " + site + "!")
    else:
        print("Could not reach " + site)
        print("Caused by: " + str(klokke.get_reason()))
    