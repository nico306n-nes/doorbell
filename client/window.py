import tkinter as tk
from tkinter import Label
from PIL import Image, ImageTk
import screeninfo  # Optional for multi-monitor environments'
import sound

class Text:
    custom_text = "None"
    size = 18
    background_color = "#fff"
    foreground_color = "#000"
    margin_x = 10
    margin_y = 10
    def __init__(self, custom_text, color="#000", background_color="#fff", margin_x=10, margin_y=10):
        self.custom_text = custom_text
        self.foreground_color = color
        self.background_color = background_color
        self.margin_x = margin_x
        self.margin_y = margin_y
    
class Box:
    width = 400
    height = 200
    timeout = 10
    background = "background.png"
    def __init__(self, width, height, timeout, background):
        self.width = width
        self.height = height
        self.timeout = timeout
        self.background = background
        
def create_window(text=Text("No Text set :("), box=Box(400, 200, 10, "background.png"), sound=sound.Sound("jingle.mp3", 0)):
    # Create the main window
    root = tk.Tk()
    
    # Set window to be borderless and always on top
    root.overrideredirect(True)
    root.attributes("-topmost", True)
    
    
    
    # Get screen width and height for positioning
    screen_width = root.winfo_screenwidth()
    screen_height = root.winfo_screenheight()
    
    # Calculate position for the lower right corner
    x_position = screen_width - box.width - 0  # 10 px margin from the right
    y_position = screen_height - box.height - 0  # 50 px margin from the bottom

    # Set window size and position
    root.geometry(f"{box.width}x{box.height}+{x_position}+{y_position}")
    
    # Load and resize the image to fit the window
    image = Image.open(box.background)
    image = image.resize((box.width, box.height), Image.LANCZOS)
    photo = ImageTk.PhotoImage(image)
    
    # Create label for image background
    image_label = Label(root, image=photo)
    image_label.place(x=0, y=0, relwidth=1, relheight=1)
    
    # Add custom text on top of the image
    text_label = Label(root, text=text.custom_text, fg=text.foreground_color, bg=text.background_color, font=("Arial", text.size))
    text_label.place(anchor="nw", x=text.margin_x, y=text.margin_y)
    
    
    root.after((box.timeout * 1000) - sound.fade_out, lambda: sound.fade_stop())
    root.after(box.timeout * 1000, lambda: root.destroy())
    
    # Run the main loop
    root.mainloop()


# Example usage
if __name__ == "__main__":
    image_path = "background2.png"  # Path to your image file
    custom_text = "Klokke ringet"
    create_window(Text(custom_text), Box(400, 200, 10, image_path))

