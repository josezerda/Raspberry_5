#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    cv::VideoCapture cap(0); // Abre la cámara predeterminada
    if (!cap.isOpened()) {
        std::cerr << "No se pudo abrir la cámara" << std::endl;
        return -1;
    }

    cv::Mat frame, frameHSV, mask;
    while (true) {
        cap >> frame; // Obtiene un nuevo frame de la cámara
        if (frame.empty()) break; // Detiene el bucle si el frame está vacío

        // Convertir de BGR a HSV
        cv::cvtColor(frame, frameHSV, cv::COLOR_BGR2HSV);

        // Definir el rango del color azul en HSV
        cv::Scalar lower_blue(100, 150, 50); // HSV mínimos para el color azul
        cv::Scalar upper_blue(140, 255, 255); // HSV máximos para el color azul

        // Umbralizar la imagen HSV para obtener solo colores azules
        cv::inRange(frameHSV, lower_blue, upper_blue, mask);

        // Morfología para eliminar el ruido
        cv::erode(mask, mask, cv::Mat(), cv::Point(-1, -1), 2);
        cv::dilate(mask, mask, cv::Mat(), cv::Point(-1, -1), 2);

        // Encontrar contornos
        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(mask.clone(), contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        // Seguir el objeto más grande detectado
        double max_area = 0;
        int max_index = -1;
        for (int i = 0; i < contours.size(); i++) {
            double area = cv::contourArea(contours[i]);
            if (area > max_area) {
                max_area = area;
                max_index = i;
            }
        }

        if (max_index != -1) {
            // Dibujar un rectángulo alrededor del objeto más grande
            cv::Rect rect = cv::boundingRect(contours[max_index]);
            cv::rectangle(frame, rect, cv::Scalar(255, 0, 0), 2);
        }

        cv::imshow("Frame", frame);
        cv::imshow("Mask", mask);

        if (cv::waitKey(10) == 27) break; // Espera por la tecla ESC para salir
    }

    cap.release();
    cv::destroyAllWindows();
    return 0;
}
