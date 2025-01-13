import window
import sound
import networking
import time
import sys

timeout = 10

text = window.Text("Der er nogen ved klokken :)")
box = window.Box(400, 200, timeout, "background2.png")
jingle = sound.Sound("jingle.mp3", fade_out=1000)

ips = ["klokke.local", "192.168.50.200", "192.168.0.200", "10.0.50.200", "10.0.0.200", "127.0.0.1:8080"]


def attempt_connection(ips, debug=True):
    for ip in ips:
        klokke_ip = ip
        klokke = networking.Site(klokke_ip)
        if klokke.check_connection() == True:
            if debug == True:
                print("Reached site at " + klokke_ip + "!")
            
            return klokke
        else:
            if debug == True:
                print("Could not reach " + klokke_ip)
                print("Caused by: " + str(klokke.get_reason()))

    return False


print("attempting connection")
klokke = attempt_connection(ips, debug=True)

if (klokke == False):
    print("couldnt reach any ip. Exiting")
    sys.exit()


# main loop
while True:
    diff = klokke.get_diff()
    print(diff)
    if diff < 10000:
        # IMPORTANT: play the jingle before opening the window
        jingle.play(loops=-1, max_time=timeout * 1000)
        window.create_window(text, box, jingle)
        

    time.sleep(0.5)


