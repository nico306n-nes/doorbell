import window
import sound

text = window.Text("Der er nogen ved klokken :)")
box = window.Box(400, 200, 10, "background2.png")
jingle = sound.Sound("jingle.mp3")


# IMPORTANT: play the jingle before opening the window
jingle.play()
window.create_window(text, box)