import tkinter as tk
from tkinter import Label
from PIL import Image, ImageTk
import screeninfo  # Optional for multi-monitor environments

def create_custom_window(image_path, text="Klokke ringet", window_width=400, window_height=200, window_timeout=10):
    # Create the main window
    root = tk.Tk()
    
    # Set window to be borderless and always on top
    root.overrideredirect(True)
    root.attributes("-topmost", True)
    
    
    
    # Get screen width and height for positioning
    screen_width = root.winfo_screenwidth()
    screen_height = root.winfo_screenheight()
    
    # Calculate position for the lower right corner
    x_position = screen_width - window_width - 0  # 10 px margin from the right
    y_position = screen_height - window_height - 0  # 50 px margin from the bottom

    # Set window size and position
    root.geometry(f"{window_width}x{window_height}+{x_position}+{y_position}")
    
    # Load and resize the image to fit the window
    image = Image.open(image_path)
    image = image.resize((window_width, window_height), Image.LANCZOS)
    photo = ImageTk.PhotoImage(image)
    
    # Create label for image background
    image_label = Label(root, image=photo)
    image_label.place(x=0, y=0, relwidth=1, relheight=1)
    
    # Add custom text on top of the image
    text_label = Label(root, text=text, fg="white", bg="black", font=("Arial", 18))
    text_label.place(anchor="nw", x=10, y=10)
    
    # Allow window to be closed with the Escape key
    root.after(window_timeout * 1000, lambda: root.destroy())
    
    # Run the main loop
    root.mainloop()

# Example usage
if __name__ == "__main__":
    image_path = "background.png"  # Path to your image file
    custom_text = "Klokke ringet"
    width = 400
    height = 200
    timeout = 10
    create_custom_window(image_path, custom_text, width, height, timeout)

