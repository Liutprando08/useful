import cv2
import pytesseract
import tkinter as tk
from tkinter import filedialog


def preprocess_image(image_path):
    img = cv2.imread(image_path)
    if img is None:
        raise FileNotFoundError("Immagine non trovata")

    # conversione il scala di grigi
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    # riduzione del rumore
    gray = cv2.bilateralFilter(gray, 9, 75, 75)

    # BInarizzazione con Otsu
    _, thresh = cv2.threshold(
        gray,  # valore di soglia (da 0 a 255)
        127,
        255,  # valore massimo assegnato ai pixel "bianchi"
        cv2.THRESH_BINARY,  # tipo di threshold
    )

    # Morfologia per pulizia piccoli artefatti
    kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (2, 2))
    processed = cv2.morphologyEx(thresh, cv2.MORPH_OPEN, kernel)

    # allineamento testo
    coords = cv2.findNonZero(processed)
    if coords is not None:
        angle = cv2.minAreaRect(coords)[-1]
        if angle < -45:
            angle = -(90 + angle)
        else:
            angle = -angle

        (h, w) = processed.shape[:2]
        center = (w // 2, h // 2)
        M = cv2.getRotationMatrix2D(center, angle, 1.0)
        rotated = cv2.warpAffine(
            processed, M, (w, h), flags=cv2.INTER_CUBIC, borderMode=cv2.BORDER_REPLICATE
        )
    else:
        rotated = processed

    cv2.imshow("immagine", rotated)
    cv2.waitKey(0)
    cv2.destroyAllWindows()
    return rotated


def extract_text(image_path, lang="ita"):
    processed_img = preprocess_image(image_path)
    text = pytesseract.image_to_string(processed_img, lang=lang)
    return text


root = tk.Tk()
root.withdraw()
print("seleziona file  per opencv1")
file_path = filedialog.askopenfilename(
    title="seleziona un immagine",
    filetypes=[("Immagini", "*.png *.jpg *.jpeg *.bmp *.tiff")],
)
if not file_path:
    print("Nessun file selezionato. Uscita...")
    exit()
try:
    text = extract_text(file_path)
    print("\n--- Testo estratto ---\n")
    print(text)
except FileNotFoundError as e:
    print(e)
