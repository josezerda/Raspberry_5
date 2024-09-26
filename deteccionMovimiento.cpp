#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    cv::VideoCapture cap(0); // Abre la cámara predeterminada
    if (!cap.isOpened()) {
        std::cerr << "No se pudo abrir la cámara" << std::endl;
        return -1;
    }

    cv::Mat frame, prevFrame, frameDelta, gray, thresh;
    std::vector<std::vector<cv::Point>> contours;

    // Lee el primer frame
    cap >> prevFrame;
    cv::cvtColor(prevFrame, prevFrame, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(prevFrame, prevFrame, cv::Size(21, 21), 0);

    while (true) {
        cap >> frame;
        if (frame.empty()) break;

        // Convertir a escala de grises y aplicar desenfoque
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        cv::GaussianBlur(gray, gray, cv::Size(21, 21), 0);

        // Calcular la diferencia entre el frame actual y el anterior
        cv::absdiff(prevFrame, gray, frameDelta);
        // Aplicar un umbral
        cv::threshold(frameDelta, thresh, 25, 255, cv::THRESH_BINARY);

        // Dilatar la imagen umbralizada para llenar los huecos
        cv::dilate(thresh, thresh, cv::Mat(), cv::Point(-1, -1), 2);
        // Encontrar contornos
        contours.clear();
        cv::findContours(thresh, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        for (int i = 0; i < contours.size(); i++) {
            if (cv::contourArea(contours[i]) < 500) {
                continue; // Ignorar contornos pequeños
            }
            // Dibujar un rectángulo alrededor del contorno
            cv::Rect rect = cv::boundingRect(contours[i]);
            cv::rectangle(frame, rect, cv::Scalar(0, 255, 0), 2);
        }

        cv::imshow("Frame", frame);
        cv::imshow("Threshold", thresh);
        cv::imshow("Frame Delta", frameDelta);

        prevFrame = gray.clone(); // Actualizar el frame anterior

        if (cv::waitKey(30) >= 0) break; // Espera por una tecla para salir
    }

    cap.release();
    cv::destroyAllWindows();
    return 0;
}
