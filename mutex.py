import time
import threading

# Shared resource
counter = 0
lock = threading.Lock()

# Function for incrementing counter
def increment():
    global counter
    for _ in range(100000):
        # Lock before accessing the shared resource
        with lock:
            counter += 1

# Creating two threads
thread1 = threading.Thread(target=increment)
thread2 = threading.Thread(target=increment)

# Start threads
thread1.start()
thread2.start()

# Wait for threads to complete
thread1.join()
thread2.join()

# Print the final value of counter
print(f"Final counter value: {counter}")