#include <opencv2/opencv.hpp>
#include <iostream>

int main(int, char**) {
    cv::VideoCapture cap(0); // Abre la cámara predeterminada
    if(!cap.isOpened()) { // Chequea si se ha logrado abrir
        std::cerr << "No se pudo abrir la cámara" << std::endl;
        return -1;
    }

    cv::Mat frame;
    while (true) {
        cap >> frame; // Obtiene un nuevo frame de la cámara
        if (frame.empty()) break; // Detiene el bucle si el frame está vacío

        cv::imshow("Captura - Presiona ESC para salir", frame);
        if (cv::waitKey(10) == 27) break; // Espera por la tecla ESC para salir
    }

    // Libera la cámara
    cap.release();
    cv::destroyAllWindows();
    return 0;
}
