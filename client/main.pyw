import window
import sound
import networking
import time
import sys
from configloader import Config_loader

config = Config_loader("config.json").config
config_text = config["window"]["text"]
config_box = config["window"]["box"]


timeout = 10

text = window.Text(custom_text=config_text["text"], size=config_text["size"], color=config_text["color"], background_color=config_text["background"], margin_x=config_text["margin"][0], margin_y=config_text["margin"][1])
box = window.Box(width=config_box["width"], height=config_box["height"], timeout=config_box["timeout"], background=config_box["background"])
jingle = sound.Sound("jingle.mp3", fade_out=1000)





print("attempting connection")
klokke = networking.attempt_connection(config["connection"]["ips"], debug=False)

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
