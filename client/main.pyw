import window
import sound
import networking
import time
import sys
from configloader import Config_loader

config = Config_loader("config.json").config
config_text = config["window"]["text"]
config_box = config["window"]["box"]
config_sound = config["sound"]

text = window.Text(custom_text=config_text["text"], size=config_text["size"], color=config_text["color"], background_color=config_text["background"], margin_x=config_text["margin"][0], margin_y=config_text["margin"][1])
box = window.Box(width=config_box["width"], height=config_box["height"], timeout=config_box["timeout"], background=config_box["background"])
jingle = sound.Sound(sound="jingle.mp3", fade_out=config_sound["fade_out"])

info_text = window.Text("Klokken er registreret")
info_box = window.Box(400, 200, 15, "background2.png")


print("attempting connection")
klokke_ip = networking.attempt_connection(config["connection"]["ips"], debug=True)

klokke = networking.Site(klokke_ip)

if (klokke == False):
    print("couldnt reach any ip. Exiting")
    sys.exit()

print("connection succesful")
print(klokke.site)

#info_text.custom_text = f"Klokkens ip er blevet fundet\n{klokke_ip}"
window.create_window(info_text, info_box)

# main loop
while True:
    diff = klokke.get_diff()
    if diff < 10000:
        # IMPORTANT: play the jingle before opening the window
        jingle.play(loops=config_sound["loops"], max_time=config_sound["max_time"], fade_ms=config_sound["fade_in"])
        window.create_window(text, box, jingle)

    time.sleep(0.5)
