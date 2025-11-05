import cv2
import pytesseract
import tkinter as tk
from tkinter import filedialog

root = tk.Tk()
root.withdraw()
print("seleziona file  per opencv1")
file_path = filedialog.askopenfilename(
    title="seleziona un immagine",
    filetypes=[("Immagini", "*.png *.jpg *.jpeg *.bmp *.tiff")],
)
if file_path:
    print(f"hai selezionato: {file_path}")
else:
    print("nessun file selezionato")
