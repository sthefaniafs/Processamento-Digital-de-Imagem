import cv2
import numpy as np

retangulo = False
PosicaoROI = []

# função para escolher região em que a cor será detectada
def desenhar_retangulo(event, x, y, flags, param):
    global retangulo, PosicaoROI

    if event == cv2.EVENT_LBUTTONDOWN:
        retangulo = True
        PosicaoROI = [(x, y)]

    elif event == cv2.EVENT_LBUTTONUP:
        retangulo = False
        PosicaoROI.append((x, y))
        cv2.rectangle(frame, PosicaoROI[0], PosicaoROI[1], (0, 255, 0), 2)
        cv2.imshow('Webcam', frame)

# função para identificar a cor na região escolhida
def identificar_cor(roi):
    roi_hsv = cv2.cvtColor(roi, cv2.COLOR_BGR2HSV)

    # calcule a média da cor na região HSV
    # a cor será um valor médio da região identificada

    media_cor = np.mean(roi_hsv, axis=(0, 1))
    desvio_cor = np.std(roi_hsv, axis=(0, 1))

    valorH, valorS, valorV = media_cor[0] - desvio_cor[0], media_cor[1] - desvio_cor[1], media_cor[2] - desvio_cor[2]
 
    if valorH < 11:
        if valorV < 50:
            return "Preto"
        elif valorS < 50:
            return "Branco"
        else:
            return "Vermelho"
    elif valorH < 22:
        if valorV<50:
            return "Preto"
        elif valorS < 50:
            return "Branco"
        else:
            return "Laranja"
    elif valorH < 34:
        if valorV<50:
            return "Preto"
        elif valorS < 50:
            return "Branco"
        else:
            return "Amarelo"
    elif valorH < 86:
        if valorV<50:
            return "Preto"
        elif valorS < 50:
            return "Branco"
        else:
            return "Verde"
    elif valorH < 133:
        if valorV<50:
            return "Preto"
        elif valorS < 50:
            return "Branco"
        else:
            return "Azul"
    elif valorH < 150:
        if valorV<50:
            return "Preto"
        elif valorS < 50:
            return "Branco"
        else:
            return "Violeta"
    elif valorH < 170:
        if valorV<50:
            return "Preto"
        elif valorS < 50:
            return "Branco"
        else:
            return "Rosa"
    else:
        if valorV<50:
            return "Preto"
        elif valorS < 50:
            return "Branco"
        else:
            return "Vermelho"

#iniciar câmera
cap = cv2.VideoCapture(0)

#configura o callback do mouse
cv2.namedWindow('Webcam')
cv2.setMouseCallback('Webcam', desenhar_retangulo)

if not cap.isOpened():
    print("Câmera indisponível")
else:
    while True:
        ret, frame = cap.read()

        if len(PosicaoROI) == 2:
            roi = frame[PosicaoROI[0][1]:PosicaoROI[1][1], PosicaoROI[0][0]:PosicaoROI[1][0]]

            # realiza a detecção de cor na região de interesse (roi)
            cor = identificar_cor(roi)

            # escreve a cor identificada
            cv2.rectangle(frame, PosicaoROI[0], PosicaoROI[1], (0, 255, 0), 2)
            cv2.putText(frame, f'Cor: {cor}', (20, 60), 0, 0.7, (255, 255, 255), 2)

            # mostra a região de interesse de perto
            cv2.namedWindow('ROI')
            roi = cv2.resize(roi, (frame.shape[1], frame.shape[0]))
            cv2.resizeWindow('ROI', frame.shape[1], frame.shape[0])
            cv2.imshow('ROI', roi)


        cv2.imshow('Webcam', frame)
        key = cv2.waitKey(1)
        if key == 27: # se apertar esc fecha
            break

# liberar os recursos quando terminar
cap.release()
cv2.destroyAllWindows()