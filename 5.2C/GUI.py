import tkinter as tk
import RPi.GPIO as GPIO


# ---------------- GPIO PIN DEFINITIONS ----------------

# Define GPIO pins for each room light
LIVING_ROOM = 18
BATHROOM = 27
CLOSET = 22


# ---------------- GPIO SETUP ----------------

# Use BCM numbering for Raspberry Pi GPIO pins
GPIO.setmode(GPIO.BCM)

# Set all three GPIO pins as output pins
GPIO.setup(LIVING_ROOM, GPIO.OUT)
GPIO.setup(BATHROOM, GPIO.OUT)
GPIO.setup(CLOSET, GPIO.OUT)

# Initially turn all LEDs OFF
GPIO.output(LIVING_ROOM, GPIO.LOW)
GPIO.output(BATHROOM, GPIO.LOW)
GPIO.output(CLOSET, GPIO.LOW)


# ---------------- PWM SETUP ----------------

# Create PWM for the Living Room LED
# Frequency = 1000 Hz
living_pwm = GPIO.PWM(LIVING_ROOM, 1000)

# Start PWM with 0% duty cycle, so the LED is OFF
living_pwm.start(0)


# ---------------- GUI SETUP ----------------

# Create the main Tkinter window
root = tk.Tk()

# Set the title of the GUI window
root.title("Smart Home Lights")

# Set the window size
root.geometry("450x500")

# Set the background colour
root.configure(bg="#F5E6CC")

# Prevent the user from resizing the window
root.resizable(False, False)


# ---------------- VARIABLES ----------------

# Store the currently selected room
selected_room = tk.StringVar(root, value="")

# Store the brightness value of the Living Room LED
# Initial brightness is 50%
brightness = tk.DoubleVar(root, value=50)


# ---------------- FUNCTIONS ----------------

# Turn OFF all room lights
def turn_off_all():
    living_pwm.ChangeDutyCycle(0)
    GPIO.output(BATHROOM, GPIO.LOW)
    GPIO.output(CLOSET, GPIO.LOW)


# Select the Living Room
def select_living_room():
    # Store the selected room
    selected_room.set("Living Room")

    # Turn OFF the other two LEDs
    GPIO.output(BATHROOM, GPIO.LOW)
    GPIO.output(CLOSET, GPIO.LOW)

    # Set Living Room brightness using the slider value
    living_pwm.ChangeDutyCycle(brightness.get())


# Select the Bathroom
def select_bathroom():
    # Store the selected room
    selected_room.set("Bathroom")

    # Turn OFF the Living Room PWM
    living_pwm.ChangeDutyCycle(0)

    # Turn ON the Bathroom LED
    GPIO.output(BATHROOM, GPIO.HIGH)

    # Keep the Closet LED OFF
    GPIO.output(CLOSET, GPIO.LOW)


# Select the Closet
def select_closet():
    # Store the selected room
    selected_room.set("Closet")

    # Turn OFF the Living Room PWM
    living_pwm.ChangeDutyCycle(0)

    # Keep the Bathroom LED OFF
    GPIO.output(BATHROOM, GPIO.LOW)

    # Turn ON the Closet LED
    GPIO.output(CLOSET, GPIO.HIGH)


# Change the Living Room brightness
def change_brightness(value):
    # Change brightness only when Living Room is selected
    if selected_room.get() == "Living Room":
        living_pwm.ChangeDutyCycle(float(value))


# Close the program safely
def exit_program():
    # Stop PWM
    living_pwm.stop()

    # Turn OFF all LEDs
    GPIO.output(LIVING_ROOM, GPIO.LOW)
    GPIO.output(BATHROOM, GPIO.LOW)
    GPIO.output(CLOSET, GPIO.LOW)

    # Release the GPIO pins
    GPIO.cleanup()

    # Close the GUI window
    root.destroy()


# ---------------- HEADING ----------------

# Create the main heading
heading = tk.Label(
    root,
    text="SMART HOME LIGHTS",
    font=("Arial", 22, "bold"),
    bg="#F5E6CC",
    fg="black"
)

# Display the heading
heading.pack(pady=(25, 25))


# ---------------- ROOM BUTTONS ----------------

# Create the Living Room button
living_button = tk.Button(
    root,
    text="LIVING ROOM",
    command=select_living_room,
    bg="yellow",
    fg="black",
    font=("Arial", 12, "bold"),
    width=23,
    height=2,
    relief="flat"
)

# Display the Living Room button
living_button.pack(pady=8)


# Create the Bathroom button
bathroom_button = tk.Button(
    root,
    text="BATHROOM",
    command=select_bathroom,
    bg="red",
    fg="white",
    font=("Arial", 12, "bold"),
    width=23,
    height=2,
    relief="flat"
)

# Display the Bathroom button
bathroom_button.pack(pady=8)


# Create the Closet button
closet_button = tk.Button(
    root,
    text="CLOSET",
    command=select_closet,
    bg="green",
    fg="white",
    font=("Arial", 12, "bold"),
    width=23,
    height=2,
    relief="flat"
)

# Display the Closet button
closet_button.pack(pady=8)


# ---------------- BRIGHTNESS SLIDER ----------------

# Create the brightness label
brightness_label = tk.Label(
    root,
    text="Living Room Brightness",
    font=("Arial", 12, "bold"),
    bg="#F5E6CC",
    fg="black"
)

# Display the brightness label
brightness_label.pack(pady=(18, 5))


# Create the brightness slider
brightness_slider = tk.Scale(
    root,
    from_=0,                    # Minimum brightness = 0%
    to=100,                     # Maximum brightness = 100%
    orient=tk.HORIZONTAL,
    variable=brightness,
    command=change_brightness,
    length=280,
    showvalue=True,
    resolution=1,
    bg="#F5E6CC",
    fg="black",
    highlightthickness=0,
    troughcolor="white"
)

# Display the brightness slider
brightness_slider.pack()


# ---------------- EXIT BUTTON ----------------

# Create the Exit button
exit_button = tk.Button(
    root,
    text="EXIT",
    command=exit_program,
    bg="black",
    fg="white",
    font=("Arial", 11, "bold"),
    width=12,
    height=2,
    relief="flat"
)

# Display the Exit button
exit_button.pack(pady=22)


# ---------------- START GUI ----------------

# Start the Tkinter GUI and keep it running
try:
    root.mainloop()

# If Ctrl+C is pressed in the terminal,
# safely close the program and clean up GPIO
except KeyboardInterrupt:
    exit_program()