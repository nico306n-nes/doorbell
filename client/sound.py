import pygame

pygame.init()
pygame.mixer.init()



class Sound:
    sound = "jingle.mp3"
    fade_in = 0
    fade_out = 0

    sound_sound = pygame.mixer.Sound(sound)

    def __init__(self, sound, fade_out):
        self.sound_sound = pygame.mixer.Sound(sound)
        self.fade_out = fade_out

    def play(self, loops=-1, max_time=0, fade_ms=0):
        self.sound_sound.play(loops, max_time, fade_ms)

    def get_length(self):
        return self.sound_sound.get_length()
    
    def fade_stop(self):
        self.sound_sound.fadeout(self.fade_out)
    




if __name__ == "__main__":
    jingle = Sound("jingle.mp3")
    jingle.play()

    pygame.time.wait(int(jingle.get_length() * 1000 * 2))