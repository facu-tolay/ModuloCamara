import cv2
import numpy as np

# Inicializa la captura de video
cap = cv2.VideoCapture('1.mp4')

# Obtiene el ancho y el alto del video de entrada
frame_width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
frame_height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))

# Define el codec y crea el objeto VideoWriter para guardar el video en formato MP4
fourcc = cv2.VideoWriter_fourcc(*'mp4v')  # Codec para MP4
out = cv2.VideoWriter('output.mp4', fourcc, 20.0, (frame_width, frame_height))

# Define el rango de colores amarillo en HSV
lower_yellow = np.array([20, 200, 150])
upper_yellow = np.array([30, 255, 255])

# Lista para almacenar las posiciones del centro del objeto
points = []

while True:
    # Captura frame-by-frame
    ret, frame = cap.read()
    
    if not ret:
        break

    # Aplica un desenfoque gaussiano al frame
    blurred_frame = cv2.GaussianBlur(frame, (15, 15), 0)
    
    # Convierte el frame de BGR a HSV
    hsv = cv2.cvtColor(blurred_frame, cv2.COLOR_BGR2HSV)

    # Crea una máscara para los colores en el rango especificado
    mask = cv2.inRange(hsv, lower_yellow, upper_yellow)

    # Encuentra los contornos en la máscara
    contours, _ = cv2.findContours(mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

    if contours:
        # Encuentra el contorno más grande por área
        largest_contour = max(contours, key=cv2.contourArea)

        # Calcula el rectángulo delimitador para el contorno más grande
        x, y, w, h = cv2.boundingRect(largest_contour)

        # Calcula el centro del rectángulo delimitador
        center = (x + w // 2, y + h // 2)

        # Añade el centro a la lista de puntos
        points.append(center)

        # Dibuja el rectángulo delimitador en el frame original
        cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)

    # Dibuja las líneas de trayectoria
    for i in range(1, len(points)):
        if points[i - 1] is None or points[i] is None:
            continue
        cv2.line(frame, points[i - 1], points[i], (0, 0, 255), 2)

    # Escribe el frame modificado en el archivo de video
    out.write(frame)

    # Muestra el frame modificado
    cv2.imshow('Original', frame)

    # Termina el programa al presionar la tecla 'q'
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Libera la captura de video y el objeto VideoWriter, y cierra todas las ventanas
cap.release()
out.release()
cv2.destroyAllWindows()
